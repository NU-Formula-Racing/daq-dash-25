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
        4 * sizeof(float) +    // wheel speeds
        1 * sizeof(uint8_t) +  // drive state, bmsState, imdState, bmsSOC, inverterStatus
        7 * sizeof(float) +    // HVVoltage, LVVoltage, batteryTemp, maxCellTemp, minCellTemp, maxCellVoltage, minCellVoltage
        BMS_FAULT_COUNT * sizeof(bool) +
        ECU_FAULT_COUNT * sizeof(bool);

    _lineBuffer = ByteBuffer(timeSize + dataDataSize + driveDataSize);
}

void Logger::initialize() {
    Resources::instance().driveBus.initialize();
    Resources::instance().dataBus.initialize();
    setSyncProvider(getTeensy3Time);

    if (!SD.begin(chipSelect))
        Serial.println("SD initalization failed.");
    else
        Serial.println("SD card initialized.");

    // open up the file here
    // log_dd_mm_yy_time ->>> CORRECTION ":" is not supported in filenames so changed all instances with "_"

    std::string filename = "log_" + std::to_string(day()) + "_" + std::to_string(month()) + "_" + std::to_string(year()) + ".csv";

    this->_file = SD.open(filename.c_str(), FILE_WRITE);

    // write the header
    // time, wheelspeed_FL, wheelspeed_fr ....

    std::string header =
        "hour,minute,second,wheelspeed_FL,wheelspeed_FR,wheelspeed_BL,wheelspeed_BR,driveState,"
        "HVVoltage,LVVoltage,batteryTemp,maxCellTemp,minCellTemp,maxCellVoltage,minCellVoltage,Fault_Summary,Undervoltage_Fault,Overvoltage_Fault,Undertemperature_Fault,"
        "Overtemperature_Fault,Overcurrent_Fault,External_Kill_Fault,Open_Wire_Fault";

    for (int i = 0; i < 140; i++) {
        header = header + ",cell_v_" + std::to_string(i);
    }

    for (int j = 0; j < 80; j++) {
        header = header + ",cell_t_" + std::to_string(j);
    }

    header = header + "\n";

    _file.write(header.c_str());
}

void Logger::log() {
    Resources::instance().driveBus.update();
    Resources::instance().dataBus.update();

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

    this->_file.write((const char*)_lineBuffer.buffer.data(), _lineBuffer.size());
}

void Logger::close() {
    this->_file.close();
}