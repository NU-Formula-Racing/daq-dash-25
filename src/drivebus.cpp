#include "drivebus.h"

void DriveBus::GetCAN() {
    this->_driveBus.Tick();
}

void DriveBus::Initialize() {
    _driveBus.Initialize(ICAN::BaudRate::kBaud500K);

    _driveBus.RegisterRXMessage(rx_fl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_fr_wheel_speed);
    _driveBus.RegisterRXMessage(rx_bl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_br_wheel_speed);
    _driveBus.RegisterRXMessage(rx_drive_state);
    _driveBus.RegisterRXMessage(rx_hv_battery);
    _driveBus.RegisterRXMessage(rx_lv_voltage);
    _driveBus.RegisterRXMessage(rx_bms_faults);
}

void DriveBus::Update() {
    this->_data.wheelSpeeds[0] = fl_wheel_speed_signal;
    this->_data.wheelSpeeds[1] = fr_wheel_speed_signal;
    this->_data.wheelSpeeds[2] = bl_wheel_speed_signal;
    this->_data.wheelSpeeds[3] = br_wheel_speed_signal;

    this->_data.driveState = drive_state_signal;

    this->_data.HVVoltage = hv_voltage_signal;

    this->_data.LVVoltage = lv_voltage_signal;

    this->_data.faults[0] = bms_fault_summary_signal;
    this->_data.faults[1] = bms_fault_under_voltage_signal;
    this->_data.faults[2] = bms_fault_over_voltage_signal;
    this->_data.faults[3] = bms_fault_under_temperature_signal;
    this->_data.faults[4] = bms_fault_over_temperature_signal;
    this->_data.faults[5] = bms_fault_over_current_signal;
    this->_data.faults[6] = bms_fault_external_kill_signal;
    this->_data.faults[7] = bms_fault_open_wire_signal;
}

const DriveBusData& DriveBus::getData() const {
    return this->_data;
}