#ifndef __DRIVE_BUS_H__
#define __DRIVE_BUS_H__

#include <Arduino.h>
#include <CAN.h>

#include <string>

#include "virtualTimer.h"

struct DriveBusData {
    float wheelSpeeds[4];
    uint8_t driveState;
    float HVVoltage;
    float LVVoltage;
    float batteryTemp;
    bool faults[8];
};

class DriveBus {
   public:
    // returns imuatable reference to _data
    const DriveBusData &getData() const;

    // does any of the initialization stuff
    void initialize();

    // takes all of the can signals, and populates the DriveBusData
    void update();

   private:
    DriveBusData _data;
    TeensyCAN<1> _driveBus;
    VirtualTimerGroup timer_group{};
    // all of the CAN message stuff and setup

    MakeSignedCANSignal(float, 0, 16, 1, 0) fl_wheel_speed_signal;
    MakeSignedCANSignal(float, 16, 16, 1, 0) fl_wheel_displacement_signal;
    MakeSignedCANSignal(float, 32, 16, 1, 0) fl_wheel_load_signal;
    CANRXMessage<3> rx_fl_wheel_speed{_driveBus, 0x249, fl_wheel_speed_signal, fl_wheel_displacement_signal, fl_wheel_load_signal};

    MakeSignedCANSignal(float, 0, 16, 1, 0) fr_wheel_speed_signal;
    MakeSignedCANSignal(float, 16, 16, 1, 0) fr_wheel_displacement_signal;
    MakeSignedCANSignal(float, 32, 16, 1, 0) fr_wheel_load_signal;
    CANRXMessage<3> rx_fr_wheel_speed{_driveBus, 0x24A, fr_wheel_speed_signal, fr_wheel_displacement_signal, fr_wheel_load_signal};

    MakeSignedCANSignal(float, 0, 16, 1, 0) bl_wheel_speed_signal;
    MakeSignedCANSignal(float, 16, 16, 1, 0) bl_wheel_displacement_signal;
    MakeSignedCANSignal(float, 32, 16, 1, 0) bl_wheel_load_signal;
    CANRXMessage<3> rx_bl_wheel_speed{_driveBus, 0x24B, bl_wheel_speed_signal, bl_wheel_displacement_signal, bl_wheel_load_signal};

    MakeSignedCANSignal(float, 0, 16, 1, 0) br_wheel_speed_signal;
    MakeSignedCANSignal(float, 16, 16, 1, 0) br_wheel_displacement_signal;
    MakeSignedCANSignal(float, 32, 16, 1, 0) br_wheel_load_signal;
    CANRXMessage<3> rx_br_wheel_speed{_driveBus, 0x24C, br_wheel_speed_signal, br_wheel_displacement_signal, br_wheel_load_signal};

    MakeSignedCANSignal(uint8_t, 0, 8, 1, 0) drive_state_signal;
    CANRXMessage<1> rx_drive_state{_driveBus, 0x206, drive_state_signal};

    MakeSignedCANSignal(float, 0, 12, 0.1, 0) max_discharge_current_signal;
    MakeSignedCANSignal(float, 12, 12, 0.1, 0) max_regen_current_signal;
    MakeSignedCANSignal(float, 24, 16, 0.01, 0) hv_voltage_signal;
    MakeSignedCANSignal(float, 40, 8, 1, -40) battery_temp_signal;
    MakeSignedCANSignal(float, 48, 16, 0.01, 0) battery_current_signal;
    CANRXMessage<5> rx_hv_battery{_driveBus, 0x150, max_discharge_current_signal, max_regen_current_signal, hv_voltage_signal, battery_temp_signal, battery_current_signal};

    MakeSignedCANSignal(float, 0, 16, 1.141643059, 0) lv_voltage_signal;
    MakeSignedCANSignal(bool, 16, 8, 1, 0) lv_voltage_warning_signal;
    CANRXMessage<2> rx_lv_voltage{_driveBus, 0x291, lv_voltage_signal, lv_voltage_warning_signal};

    MakeSignedCANSignal(bool, 0, 1, 1, 0) bms_fault_summary_signal;
    MakeSignedCANSignal(bool, 1, 1, 1, 0) bms_fault_under_voltage_signal;
    MakeSignedCANSignal(bool, 2, 1, 1, 0) bms_fault_over_voltage_signal;
    MakeSignedCANSignal(bool, 3, 1, 1, 0) bms_fault_under_temperature_signal;
    MakeSignedCANSignal(bool, 4, 1, 1, 0) bms_fault_over_temperature_signal;
    MakeSignedCANSignal(bool, 5, 1, 1, 0) bms_fault_over_current_signal;
    MakeSignedCANSignal(bool, 6, 1, 1, 0) bms_fault_external_kill_signal;
    MakeSignedCANSignal(bool, 7, 1, 1, 0) bms_fault_open_wire_signal;
    CANRXMessage<8> rx_bms_faults{
        _driveBus, 0x151, bms_fault_summary_signal, bms_fault_under_voltage_signal, bms_fault_over_voltage_signal, bms_fault_under_temperature_signal, bms_fault_over_temperature_signal, bms_fault_over_current_signal, bms_fault_external_kill_signal, bms_fault_open_wire_signal};
};

#endif  // __DRIVE_BUS_H__