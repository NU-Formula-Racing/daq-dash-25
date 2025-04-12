#include "logger.h"
#include <TimeLib.h>
#include <string>

const int chipSelect = BUILTIN_SDCARD;

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void Logger::initialize() {
    this->drive_bus.initialize();
    this->data_bus.initialize();
    setSyncProvider(getTeensy3Time);

    if (!SD.begin(chipSelect)) Serial.println ("SD initalization failed.");
    else Serial.println ("SD card initialized.");

    // open up the file here
    // log_dd_mm_yy_time ->>> CORRECTION ":" is not supported in filenames so changed all instances with "_"

    std::string filename = "log_" + std::to_string(day()) + "_" + std::to_string(month()) + "_" + std::to_string(year()) + ".csv";

    this->file = SD.open(filename.c_str(), FILE_WRITE);

    // write the header
    // time, wheelspeed_FL, wheelspeed_fr ....

    std::string header = "hour,minute,second,wheelspeed_FL,wheelspeed_FR,wheelspeed_BL,wheelspeed_BR,driveState," 
                         "HVVoltage,LVVoltage,batteryTemp,Fault_Summary,Undervoltage_Fault,Overvoltage_Fault,Undertemperature_Fault,"
                         "Overtemperature_Fault,Overcurrent_Fault,External_Kill_Fault,Open_Wire_Fault";

    for (int i = 0; i < 140; i++) {
      header = header + ",cell_v_" + std::to_string(i);
    }

    for (int j = 0; j < 80; j++) {
      header = header + ",cell_t_" + std::to_string(j);
    }

    header = header + "\n";
    
    file.write(header.c_str());

}

void Logger::log() {
    this->drive_bus.update();
    this->data_bus.update();
    DriveBusData drivebus_data = this->drive_bus.getData();
    DataBusData databus_data = this->data_bus.getData();

    std::string line = "";


    line = line + std::to_string(hour()) + "," + std::to_string(minute()) + "," + std::to_string(second()) + ",";

    for (const auto& wheel : drivebus_data.wheelSpeeds) {
        line = line + std::to_string(wheel) + ",";
    }

    line = line + std::to_string(drivebus_data.driveState) + "," + std::to_string(drivebus_data.HVVoltage) + "," + std::to_string(drivebus_data.LVVoltage) + "," + std::to_string(drivebus_data.batteryTemp);

    for (const auto& fault : drivebus_data.faults) {
        line = line + "," + std::to_string(fault);
    }

    for (const auto& voltage : databus_data.cellVoltages) {
      line = line + "," + std::to_string(voltage);
    }

    for (const auto& temp : databus_data.cellTemperatures) {
      line = line + "," + std::to_string(temp);
    }

    line = line + "\n";

    this->file.write(line.c_str());


}

void Logger::close() {
  this->file.close();
}