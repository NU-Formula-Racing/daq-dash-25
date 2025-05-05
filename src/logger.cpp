#include "logger.h"

#include <TimeLib.h>

#include <memory>
#include <string>

#include "resources.h"
// #include <iostream>
#include <Arduino.h>

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
        _loggerGood = false;
        return;
    }

    Serial.println("SD card initialized.");
    _loggerGood = true;

    // open up the file here
    // log_dd_mm_yy_time ->>> CORRECTION ":" is not supported in filenames so changed all instances with "_"
    this->loggingFileName = "log_" + std::to_string(day()) + "_" + std::to_string(month()) + "_" + std::to_string(year()) +
                            std::to_string(hour()) + "_" + std::to_string(minute()) + "_" + std::to_string(second()) + ".bin";
}

void Logger::log() {
    if (!this->_loggerGood) return;

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

void Logger::writeMileCounter() {
    // currently stored mileage in file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE_BEGIN);
    String counter = "";
    counter.append(Resources::instance().milageCounter);
    this->milageFile.write(counter.c_str());

    // close mileage file
    this->milageFile.close();
    // reopen old logger file
    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);
}

// returns current mileage
float Logger::readMileCounter() {
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
// every two seconds, update mileage counter

ThrottleLUT Logger::readThrottleLUT() {
    ThrottleLUT throttleLUT;

    // if file exists, open it an populate
    if (SD.exists(LUTMetadataFileName.c_str()) && SD.exists(LUTPairsFileName.c_str())) {
        throttleLUT.filesPresent = true;

        // open LUT file
        this->LUTMetadataFile = SD.open(LUTMetadataFileName.c_str(), FILE_READ);
        this->LUTPairsFile = SD.open(LUTPairsFileName.c_str(), FILE_READ);

        // parse config file and populate struct fields
        String LUTIdString = this->LUTMetadataFile.readStringUntil('\n');
        throttleLUT.LUTId = LUTIdString.toInt();
        String interpTypeString = this->LUTMetadataFile.readStringUntil('\n');
        throttleLUT.interpType = static_cast<InterpType>(interpTypeString.toInt());

        // parse xy pairs and add them to the map
        std::vector<int16_t> xVals;
        std::vector<float> yVals;
        uint8_t numPairs = 0;

        while (LUTPairsFile.available()) {
            String keyString = this->LUTPairsFile.readStringUntil(',');
            if (!LUTPairsFile.available()) { break; }

            String valString = this->LUTPairsFile.readStringUntil('\n');
            keyString = keyString.trim();
            valString = valString.trim();

            if (keyString.length() > 0 && valString.length() > 0) {
                throttleLUT.xVals.push_back(keyString.toInt());
                throttleLUT.yVals.push_back(valString.toFloat());
                numPairs++;
            }
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
    }

    // close file and return struct
    this->LUTPairsFile.close();
    this->LUTMetadataFile.close();
    return throttleLUT;

    // example LUT config file layout
    // 5 - LUTId
    // 1 - interpType (0: linear, 1: smooth_step)

    // example LUT pairs file layout
    // 0,0
    // 10,0.4
    // ...
}
