#include "logger.h"

#include <TimeLib.h>

#include <string>

#include "resources.h"

const int chipSelect = BUILTIN_SDCARD;

time_t getTeensy3Time() {
    return Teensy3Clock.get();
}

Logger::Logger() {
    // calculate the size of the data
    size_t timeSize = 3 * sizeof(int);
    size_t dataDataSize = NUM_TEMP_CELLS * sizeof(float) + NUM_VOLT_CELLS * sizeof(float);
    size_t driveDataSize =
        4 * sizeof(float) +              // wheel speeds
        1 * sizeof(uint8_t) +            // drive state, bmsState, imdState, bmsSOC, inverterStatus
        7 * sizeof(float) +              // HVVoltage, LVVoltage, batteryTemp, maxCellTemp, minCellTemp, maxCellVoltage, minCellVoltage
        BMS_FAULT_COUNT * sizeof(bool);  //+
    // ECU_FAULT_COUNT * sizeof(bool); // NOT USED RIGHT NOW

    _lineBuffer = ByteBuffer(timeSize + dataDataSize + driveDataSize);
}

void Logger::initialize() {
    Resources::instance().driveBus.initialize();
    Resources::instance().dataBus.initialize();
    setSyncProvider(getTeensy3Time);

    if (!SD.begin(chipSelect)) {
        Serial.println("SD initalization failed.");
        _loggerGood = true;
    } else {
        Serial.println("SD card initialized.");
        _loggerGood = false;
        return;
    }

    // open up the file here
    // log_dd_mm_yy_time ->>> CORRECTION ":" is not supported in filenames so changed all instances with "_"
    this->loggingFileName = "log_" + std::to_string(day()) + "_" + std::to_string(month()) + "_" + std::to_string(year()) + ".csv";
    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);

    // write the header
    // time, wheelspeed_FL, wheelspeed_fr ....

    // we don't need the header
    // std::string header =
    //     "hour,minute,second,wheelspeed_FL,wheelspeed_FR,wheelspeed_BL,wheelspeed_BR,driveState,"
    //     "HVVoltage,LVVoltage,batteryTemp,maxCellTemp,minCellTemp,maxCellVoltage,minCellVoltage,Fault_Summary,Undervoltage_Fault,Overvoltage_Fault,Undertemperature_Fault,"
    //     "Overtemperature_Fault,Overcurrent_Fault,External_Kill_Fault,Open_Wire_Fault";

    // for (int i = 0; i < 140; i++) {
    //     header = header + ",cell_v_" + std::to_string(i);
    // }

    // for (int j = 0; j < 80; j++) {
    //     header = header + ",cell_t_" + std::to_string(j);
    // }

    // header = header + "\n";

    // loggingFile.write(header.c_str());
}

void Logger::log() {
    if (!this->_loggerGood) return;

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

    this->loggingFile.write((const char*)_lineBuffer.buffer.data(), _lineBuffer.size());
}

void Logger::close() {
    if (!this->_loggerGood) return;
    this->loggingFile.close();
}

void Logger::writeMileCounter(long long deltaT) {
    if (!this->_loggerGood) return;
    // currently stored mileage in file
    float prev_mileage = readMileCounter();  // add this number to current mileage

    close();
    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE);

    // ****** TBC: write - calculates current mileage
    // this->_file_mileage.write();

    // close mileage file
    this->milageFile.close();
    // reopen old logger file
    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);
}

// returns current mileage
float Logger::readMileCounter() {
    if (!this->_loggerGood) return 0;
    close();  // closes old logger file
    // open mileage file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_READ);
    float currentMilage;
    // if empty, read as 0?
    if (this->milageFile && this->milageFile.size() == 0) {
        currentMilage = 0.0;
    } else {                       // else, read
        this->milageFile.seek(0);  // Go to the start of the file

        String numberString = this->milageFile.readStringUntil('\n');  // or '\r' or any delimiter
        currentMilage = numberString.toFloat();                        // or .toInt() for integers
    }
    // close mileage file
    this->milageFile.close();
    // reopen old logger file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE);
    // returns current mileage
    return currentMilage;
}

// every two seconds, update mileage counter
