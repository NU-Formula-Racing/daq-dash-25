#include "logger.h"

#include <TimeLib.h>

#include <memory>
#include <string>

#include "resources.h"
// #include <iostream>
#include <Arduino.h>

#include "dash/dash.h"

#include "define.h"

#include <string>

const int chipSelect = BUILTIN_SDCARD;

time_t getTeensy3Time() {
    return Teensy3Clock.get();
}

Logger::Logger() {
    // calculate the size of the data
    size_t timeSize = 3 * sizeof(int);
    size_t dataDataSize = NUM_TEMP_CELLS * sizeof(float) + NUM_VOLT_CELLS * sizeof(float);
    size_t driveDataSize =
        4 * sizeof(float) +    // wheel speeds
        1 * sizeof(uint8_t) +  // drive state, bmsState, imdState, bmsSOC, inverterStatus
        7 * sizeof(float) +    // HVVoltage, LVVoltage, batteryTemp, maxCellTemp, minCellTemp, maxCellVoltage, minCellVoltage
        BMS_FAULT_COUNT * sizeof(bool);
    // ECU_FAULT_COUNT * sizeof(bool); // NOT USED RIGHT NOW

    _lineBuffer = ByteBuffer(timeSize + dataDataSize + driveDataSize);
}

void Logger::initialize() {
    Resources::instance().driveBus.initialize();
    Resources::instance().dataBus.initialize();
    setSyncProvider(getTeensy3Time);

    Serial.println("Initializing Logger!");

    if (!SD.begin(chipSelect)) {
        Serial.println("SD initalization failed.");
        _status = LoggerStatus::UNABLE_TO_LOG;
        return;
    }

    Serial.println("SD card initialized.");
    _status = LoggerStatus::LOGGING;

    char fname[16];  // "log_" + 3-digits + ".bin" + '\0'
    uint16_t index = 0;

    do {
        snprintf(fname, sizeof(fname), "log_%03u.bin", index);
        ++index;
    } while (SD.exists(fname) && index < 1000);  // stop at 999 just in case

    loggingFileName = fname;
    Serial.print("Next log file: ");
    Serial.println(loggingFileName.c_str());

    _status = LoggerStatus::LOGGING;
}

void Logger::log() {
    if (_status == LoggerStatus::UNABLE_TO_LOG) return;

    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);

    this->loggingFile.seek(this->loggingFile.size());

    // Serial.println("Beginning to log!");

    DriveBusData driveData = Resources::instance().driveBus.getData();
    DataBusData dataData = Resources::instance().dataBus.getData();

    _lineBuffer.reset();
    // write the time
    _lineBuffer.write(hour());
    _lineBuffer.write(minute());
    _lineBuffer.write(second());

    for (const auto& wheel : driveData.wheelSpeeds) {
        _lineBuffer.write(wheel);
    }

    _lineBuffer.write(driveData.driveState);
    _lineBuffer.write(driveData.HVVoltage);
    _lineBuffer.write(driveData.LVVoltage);
    _lineBuffer.write(driveData.batteryTemp);
    _lineBuffer.write(driveData.maxCellTemp);
    _lineBuffer.write(driveData.minCellTemp);
    _lineBuffer.write(driveData.maxCellVoltage);
    _lineBuffer.write(driveData.minCellVoltage);

    for (const auto& fault : driveData.bmsFaults) {
        _lineBuffer.write(fault);
    }

    for (const auto& voltage : dataData.cellVoltages) {
        _lineBuffer.write(voltage);
    }

    for (const auto& temp : dataData.cellTemperatures) {
        _lineBuffer.write(temp);
    }

    // Serial.println("Finished logging!");
    this->loggingFile.write(_lineBuffer.buffer.data(), _lineBuffer.size());

    this->loggingFile.close();
}

    
LoggerStatus Logger::status() const {
    return _status;
}

std::string Logger::logFileName() const {
    return loggingFileName;
}

void Logger::writeMileCounter() {
    if (_status == LoggerStatus::UNABLE_TO_LOG) return;

    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE_BEGIN);
    String counter = "";
    counter.append(Resources::instance().milageCounter);
    this->milageFile.write(counter.c_str());
    this->milageFile.close();
    // reopen old logger file
    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);
}

// returns current mileage
float Logger::readMileCounter() {
    if (_status == LoggerStatus::UNABLE_TO_LOG) return 0;

    // open mileage file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_READ);
    float miles = 0;
    // if empty, read as 0?
    if (this->milageFile && this->milageFile.size() == 0) {
        return 0;
    } else {                                                           // else, read
        this->milageFile.seek(0);                                      // Go to the start of the file
        String numberString = this->milageFile.readStringUntil('\n');  // or '\r' or any delimiter
        miles = numberString.toFloat();                                // or .toInt() for integers
    }
    // close mileage file
    this->milageFile.close();
    // reopen old logger file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE);
    // returns current mileage
    return miles;
}

ThrottleLUT Logger::readThrottleLUT() {
    ThrottleLUT throttleLUT;

    // if file exists, open it an populate
    if (_status == LoggerStatus::LOGGING && SD.exists(LUTFileName.c_str())) {
        throttleLUT.filesPresent = true;

        // open LUT file
        this->LUTFile = SD.open(LUTFileName.c_str(), FILE_READ);

        // parse file and populate struct fields
        // String numPairsString = this->LUTFile.readStringUntil(',');
        // uint8_t numPairs = numPairsString.toInt();
        // throttleLUT.numPairs = numPairs;
        String interpTypeString = this->LUTFile.readStringUntil('\n');
        throttleLUT.interpType = static_cast<InterpType>(interpTypeString.toInt());
        throttleLUT.LUTId = 7;

        // parse xy pairs and add them to the map
        std::vector<int16_t> xVals;
        std::vector<float> yVals;
        uint8_t numPairs = 0;
        while (LUTFile.available() && numPairs < MAX_THROTTLE_LUT_PAIRS) {
            String key_string = this->LUTFile.readStringUntil(',');
            String val_string = this->LUTFile.readStringUntil('\n');
            key_string.trim();
            val_string.trim();
            xVals.push_back(key_string.toInt());
            yVals.push_back(val_string.toFloat());
            numPairs++;
        }
        for (int j = 0 ; j < MAX_THROTTLE_LUT_PAIRS - numPairs ; j++) {
            xVals.push_back(0);
            yVals.push_back(0.0f);
        }
        throttleLUT.xVals = xVals;
        throttleLUT.yVals = yVals;
        throttleLUT.numPairs = numPairs;

    // if file not present, return empty struct with file_present field set to false
    } else {
        throttleLUT.filesPresent = false;
        for (uint8_t i = 0 ; i < MAX_THROTTLE_LUT_PAIRS ; i++) {
            throttleLUT.xVals.push_back(0);
            throttleLUT.yVals.push_back(0.0f);
        }
    }

    // close file and return struct
    this->LUTFile.close();
    return throttleLUT;

    // example LUT file layout
    // numPairs,interpType
    // xPair0,yPair0
    // xPair1,yPair1
    // ...
}

// every two seconds, update mileage counter
