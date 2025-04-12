#include "can/drive_bus.h"

const DriveBusData& DriveBus::getData() const {
    return _data;
}

const DriveBusData& DriveBus::getPrevData() const {
    return _prevData;
}

void DriveBus::initialize() {
    _driveBus.Initialize(ICAN::BaudRate::kBaud500K);

    _driveBus.RegisterRXMessage(rx_fl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_fr_wheel_speed);
    _driveBus.RegisterRXMessage(rx_bl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_br_wheel_speed);
    _driveBus.RegisterRXMessage(rx_drive_state);
    _driveBus.RegisterRXMessage(rx_hv_battery);
    _driveBus.RegisterRXMessage(rx_lv_voltage);
    _driveBus.RegisterRXMessage(rx_bms_faults);
    _driveBus.RegisterRXMessage(rx_ecu_implausibility);
}

void DriveBus::update() {
    this->_driveBus.Tick();
    // update the previous
    this->_prevData = this->_data;

    this->_data.wheelSpeeds[0] = fl_wheel_speed_signal;
    this->_data.wheelSpeeds[1] = fr_wheel_speed_signal;
    this->_data.wheelSpeeds[2] = bl_wheel_speed_signal;
    this->_data.wheelSpeeds[3] = br_wheel_speed_signal;

    this->_data.driveState = drive_state_signal;

    this->_data.HVVoltage = hv_voltage_signal;

    this->_data.LVVoltage = lv_voltage_signal;

    this->_data.bmsFaults[BMS_FAULT_SUMMARY] = bms_fault_summary_signal;
    this->_data.bmsFaults[BMS_FAULT_UNDER_VOLTAGE] = bms_fault_under_voltage_signal;
    this->_data.bmsFaults[BMS_FAULT_OVER_VOLTAGE] = bms_fault_over_voltage_signal;
    this->_data.bmsFaults[BMS_FAULT_UNDER_TEMP] = bms_fault_under_temperature_signal;
    this->_data.bmsFaults[BMS_FAULT_OVER_TEMP] = bms_fault_over_temperature_signal;
    this->_data.bmsFaults[BMS_FAULT_OVER_CURRENT] = bms_fault_over_current_signal;
    this->_data.bmsFaults[BMS_FAULT_EXTERNAL_KILL] = bms_fault_external_kill_signal;
    this->_data.bmsFaults[BMS_FAULT_OPEN_WIRE] = bms_fault_open_wire_signal;

    this->_data.ecuFaults[ECU_FAULT_PRESENT] = ecu_implausibility_present_signal;
    this->_data.ecuFaults[ECU_FAULT_APPSS_DISAGREEMENT] = ecu_implausibility_appss_disagreement_imp_signal;
    this->_data.ecuFaults[ECU_FAULT_BPPC] = ecu_implausibility_bppc_imp_signal;
    this->_data.ecuFaults[ECU_FAULT_BRAKE_INVALID] = ecu_implausibility_brake_invalid_imp_signal;
    this->_data.ecuFaults[ECU_FAULT_APPPS_INVALID] = ecu_implausibility_appss_invalid_imp_signal;
}