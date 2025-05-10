#ifndef __DRIVE_BUS_H__
#define __DRIVE_BUS_H__

#include <Arduino.h>
#include <CAN.h>

#include <string>

#include "define.h"
#include "sound.h"
#include "virtualTimer.h"

enum BMSFault {
    BMS_FAULT_SUMMARY,
    BMS_FAULT_UNDER_VOLTAGE,
    BMS_FAULT_OVER_VOLTAGE,
    BMS_FAULT_UNDER_TEMP,
    BMS_FAULT_OVER_TEMP,
    BMS_FAULT_OVER_CURRENT,
    BMS_FAULT_EXTERNAL_KILL,
    BMS_FAULT_OPEN_WIRE,
    BMS_FAULT_COUNT,
};

enum ECUFault {
    ECU_FAULT_PRESENT,
    ECU_FAULT_APPSS_DISAGREEMENT,
    ECU_FAULT_BPPC,
    ECU_FAULT_BRAKE_INVALID,
    ECU_FAULT_APPPS_INVALID,
    ECU_FAULT_COUNT
};

enum DriveState : uint8_t {
    DS_OFF = 0,
    DS_NEUTRAL = 1,
    DS_ON = 2
};

struct DriveBusData {
    bool bmsFaults[BMS_FAULT_COUNT];
    bool ecuFaults[ECU_FAULT_COUNT];

    float hvVoltage;
    float lvVoltage;
    float batteryTemp;
    float maxCellTemp;
    float minCellTemp;
    float maxCellVoltage;
    float minCellVoltage;
    float maxDischargeCurrent;
    float maxRegenCurrent;
    float bmsSOC;

    float wheelSpeeds[4];
    float wheelDisplacement[4];
    float prStrain[4];

    uint16_t bmsFaultsRaw;
    int16_t motorRPM;
    int16_t motorCurrent;
    int16_t motorDCVoltage;
    int16_t motorDCCurrent;

    uint8_t driveState;
    uint8_t bmsState;
    uint8_t imdState = 1;  // healthy when high
    uint8_t inverterStatus;
    bool lvVoltageWarning;

    DriveBusData() {
        memset(bmsFaults, 0, sizeof(bool) * BMS_FAULT_COUNT);
        memset(ecuFaults, 0, sizeof(bool) * ECU_FAULT_COUNT);
        imdState = 1;
    }

    bool faultPresent() const {
        if (driveState != 0)  // if not in off, don't ignore undervoltage
            return bmsFaults[BMS_FAULT_SUMMARY] || ecuFaults[ECU_FAULT_PRESENT] || inverterStatus != 0 || imdState == 0;

        // if we are off, ignore the undervoltage
        return bmsFaults[BMS_FAULT_SUMMARY] || ecuFaults[ECU_FAULT_PRESENT] || (inverterStatus != 0 && inverterStatus != 2) || imdState == 0;
    }

    float averageWheelRPM() const {
        // return std::max({wheelSpeeds[0], wheelSpeeds[1], wheelSpeeds[2], wheelSpeeds[3]});
        return ((wheelSpeeds[0] + wheelSpeeds[1] + wheelSpeeds[2] + wheelSpeeds[3])) / 2;  // only by two rn cause only two wheel speeds
    }

    float vehicleSpeedMPH() const;
};

class DriveBus {
   public:
    DriveBus() {}

    // returns imuatable reference to _data
    const DriveBusData &getData() const;
    const DriveBusData &getPrevData() const;

    // does any of the initialization stuff
    void initialize();

    // takes all of the can signals, and populates the DriveBusData
    void update();

   private:
    DriveBusData _data;
    DriveBusData _prevData;
    TeensyCAN<3> _driveBus;
    VirtualTimerGroup timer_group{};

    // workaround for cyclical dependency
    void playReadyToDriveSound();

    // all of the CAN message stuff and setup

    // Wheel speeds
    MakeUnsignedCANSignal(float, 0, 16, 1.0, 0) fl_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) fl_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 0.01, 0) fl_wheel_load_signal;
    CANRXMessage<3> rx_fl_wheel_speed{_driveBus, 0x249, fl_wheel_speed_signal, fl_wheel_displacement_signal, fl_wheel_load_signal};

    MakeUnsignedCANSignal(float, 0, 16, 1.0, 0) fr_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) fr_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 0.01, 0) fr_wheel_load_signal;
    CANRXMessage<3> rx_fr_wheel_speed{_driveBus, 0x24A, fr_wheel_speed_signal, fr_wheel_displacement_signal, fr_wheel_load_signal};

    MakeUnsignedCANSignal(float, 0, 16, 1.0, 0) bl_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) bl_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 0.01, 0) bl_wheel_load_signal;
    CANRXMessage<3> rx_bl_wheel_speed{_driveBus, 0x24B, bl_wheel_speed_signal, bl_wheel_displacement_signal, bl_wheel_load_signal};

    MakeUnsignedCANSignal(float, 0, 16, 1.0, 0) br_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) br_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 0.01, 0) br_wheel_load_signal;
    CANRXMessage<3> rx_br_wheel_speed{_driveBus, 0x24C, br_wheel_speed_signal, br_wheel_displacement_signal, br_wheel_load_signal};

    // ECU Stuff
    MakeUnsignedCANSignal(uint8_t, 0, 8, 1, 0) drive_state_signal;
    CANRXMessage<1> rx_drive_state{_driveBus, 0x206,
                                   [this]() {
                                       this->playReadyToDriveSound();
                                   },
                                   drive_state_signal};

    // ECU Implausibility
    CANSignal<bool, 0, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_present_signal;
    CANSignal<bool, 8, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_appss_disagreement_imp_signal;
    CANSignal<bool, 16, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_bppc_imp_signal;
    CANSignal<bool, 24, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_brake_invalid_imp_signal;
    CANSignal<bool, 32, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_appss_invalid_imp_signal;

    CANRXMessage<5> rx_ecu_implausibility{
        _driveBus, 0x204, ecu_implausibility_present_signal, ecu_implausibility_appss_disagreement_imp_signal,
        ecu_implausibility_bppc_imp_signal, ecu_implausibility_brake_invalid_imp_signal, ecu_implausibility_appss_invalid_imp_signal};

    // BMS
    MakeUnsignedCANSignal(float, 0, 12, 0.1, 0) max_discharge_current_signal;
    MakeUnsignedCANSignal(float, 12, 12, 0.1, 0) max_regen_current_signal;
    MakeUnsignedCANSignal(float, 24, 16, 0.01, 0) hv_voltage_signal;
    MakeUnsignedCANSignal(float, 40, 8, 1, -40) battery_temp_signal;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) battery_current_signal;
    CANRXMessage<5> rx_hv_battery{_driveBus, 0x150, max_discharge_current_signal, max_regen_current_signal, hv_voltage_signal, battery_temp_signal, battery_current_signal};

    MakeSignedCANSignal(uint8_t, 0, 8, 1.0, 0.0) bms_status_bms_state;
    MakeSignedCANSignal(uint8_t, 8, 8, 1.0, 0.0) bms_status_imd_state;
    MakeUnsignedCANSignal(float, 16, 8, 1.0, -40.0) bms_status_max_cell_temp;
    MakeUnsignedCANSignal(float, 24, 8, 1.0, -40.0) bms_status_min_cell_temp;
    MakeUnsignedCANSignal(float, 32, 8, 0.012, 2.0) bms_status_max_cell_voltage;
    MakeUnsignedCANSignal(float, 40, 8, 0.012, 2.0) bms_status_min_cell_voltage;
    MakeUnsignedCANSignal(float, 48, 8, 0.004, 0.0) bms_status_bms_soc;

    CANRXMessage<7> rx_bms_status{_driveBus, 0x152, bms_status_bms_state, bms_status_imd_state, bms_status_max_cell_temp, bms_status_min_cell_temp, bms_status_max_cell_voltage, bms_status_min_cell_voltage, bms_status_bms_soc};

    MakeUnsignedCANSignal(bool, 0, 1, 1, 0) bms_fault_summary_signal;
    MakeUnsignedCANSignal(bool, 1, 1, 1, 0) bms_fault_under_voltage_signal;
    MakeUnsignedCANSignal(bool, 2, 1, 1, 0) bms_fault_over_voltage_signal;
    MakeUnsignedCANSignal(bool, 3, 1, 1, 0) bms_fault_under_temperature_signal;
    MakeUnsignedCANSignal(bool, 4, 1, 1, 0) bms_fault_over_temperature_signal;
    MakeUnsignedCANSignal(bool, 5, 1, 1, 0) bms_fault_over_current_signal;
    MakeUnsignedCANSignal(bool, 6, 1, 1, 0) bms_fault_external_kill_signal;
    MakeUnsignedCANSignal(bool, 7, 1, 1, 0) bms_fault_open_wire_signal;
    CANRXMessage<8> rx_bms_faults{
        _driveBus, 0x151, bms_fault_summary_signal, bms_fault_under_voltage_signal, bms_fault_over_voltage_signal, bms_fault_under_temperature_signal, bms_fault_over_temperature_signal, bms_fault_over_current_signal, bms_fault_external_kill_signal, bms_fault_open_wire_signal};

    // PDM
    MakeUnsignedCANSignal(float, 0, 16, 0.01, 0) lv_voltage_signal;
    MakeUnsignedCANSignal(bool, 16, 8, 1, 0) lv_voltage_warning_signal;
    CANRXMessage<2> rx_lv_voltage{_driveBus, 0x2A2, lv_voltage_signal, lv_voltage_warning_signal};

    // inverter stuff
    MakeUnsignedCANSignal(uint8_t, 0, 8, 1.0, 0.0) inverter_fault_status_fault_code_signal;
    CANRXMessage<1> rx_inverter_fault_status{_driveBus, 0x280, inverter_fault_status_fault_code_signal};

    MakeSignedCANSignal(int16_t, 0, 16, 1.0, 0.0) inverter_motor_status_rpm;
    MakeSignedCANSignal(int16_t, 16, 16, 0.1, 0.0) inverter_motor_status_motor_current;
    MakeSignedCANSignal(int16_t, 32, 16, 0.1, 0.0) inverter_motor_status_dc_voltage;
    MakeSignedCANSignal(int16_t, 48, 16, 0.1, 0.0) inverter_motor_status_dc_current;

    CANRXMessage<4> rx_inverter_motor_status{_driveBus, 0x281, inverter_motor_status_rpm, inverter_motor_status_motor_current, inverter_motor_status_dc_voltage, inverter_motor_status_dc_current};

#ifdef DRIVE_DEBUG
    uint64_t _debugStartTime = 0;
    uint64_t _debugLastFaultUpdate = 0;
#endif
};

#endif  // __DRIVE_BUS_H__