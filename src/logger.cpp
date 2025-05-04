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

ThrottleLut Logger::readThrottleLUT() {
    ThrottleLut throttleLut;

    // if file exists, open it an populate
    if (SD.exists(lutMetadataFileName.c_str()) && SD.exists(lutPairsFileName.c_str())) {
        throttleLut.files_present = true;

        // open lut file
        this->lutMetadataFile = SD.open(lutMetadataFileName.c_str(), FILE_READ);
        this->lutPairsFile = SD.open(lutPairsFileName.c_str(), FILE_READ);

        // parse file and populate struct fields
        String num_pairs_string = this->lutMetadataFile.readStringUntil('\n');
        uint8_t numPairs = num_pairs_string.toInt();
        throttleLut.num_pairs = numPairs;
        String interp_type_string = this->lutMetadataFile.readStringUntil('\n');
        throttleLut.interp_type = static_cast<InterpType>(interp_type_string.toInt());
        String lut_id_string = this->lutMetadataFile.readStringUntil('\n');
        throttleLut.lut_id = lut_id_string.toInt();

        // parse xy pairs and add them to the map
        std::vector<int16_t> xVals;
        std::vector<float> yVals;
        for (int i = 0 ; i < throttleLut.num_pairs ; i++) {
            String key_string = this->lutPairsFile.readStringUntil(',');
            String val_string = this->lutPairsFile.readStringUntil('\n');
            key_string.trim();
            val_string.trim();
            xVals.push_back(key_string.toInt());
            yVals.push_back(val_string.toFloat());
            // lut.insert({key_string.toInt(), val_string.toFloat()});
            // Serial.print("key: ");
            // Serial.print(key_string);
            // Serial.print(" value: ");
            // Serial.println(val_string);
            // std::cout << "key: " << key_string.toInt() << "value: " << val_string.toFloat();

        }
        for (int j = 0 ; j < MAX_THROTTLE_LUT_PAIRS - numPairs ; j++) {
            xVals.push_back(0);
            yVals.push_back(0.0f);
        }
        throttleLut.x_vals = xVals;
        throttleLut.y_vals = yVals;

    // if file not present, return empty struct with file_present field set to false
    } else {
        throttleLut.files_present = false;
    }

    // close file and return struct
    this->lutPairsFile.close();
    this->lutMetadataFile.close();
    return throttleLut;

    // example lut file layout
    // 20 - num_pairs
    // 1 - interp_type (0: linear, 1: smooth_step)
    // 5 - lut_id
    // 0 0 - pairs
    // 10 0.4
    // ...
}

// every two seconds, update mileage counter
