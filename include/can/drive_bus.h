#ifndef __DRIVE_BUS_H__
#define __DRIVE_BUS_H__

#include <Arduino.h>
#include <CAN.h>

#include <string>

#include "define.h"
#include "sound.h"
#include "virtualTimer.h"

#define NUM_TEMP_CELLS 80
#define NUM_VOLT_CELLS 140

enum BMSFault
{
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

enum ECUFault
{
    ECU_FAULT_PRESENT,
    ECU_FAULT_APPSS_DISAGREEMENT,
    ECU_FAULT_BPPC,
    ECU_FAULT_BRAKE_INVALID,
    ECU_FAULT_APPPS_INVALID,
    ECU_FAULT_COUNT
};

enum DriveState : uint8_t
{
    DS_OFF = 0,
    DS_NEUTRAL = 1,
    DS_ON = 2
};

struct DriveBusData
{
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
    float genAmps;
    float fanAmps;
    float pumpAmps;

    uint16_t bmsFaultsRaw;
    int16_t motorRPM;
    int16_t motorCurrent;
    int16_t motorDCVoltage;
    int16_t motorDCCurrent;
    int16_t frontBrakePressure;
    int16_t rearBreakPressure;
    int16_t apps1;
    int16_t apps2;
    uint16_t inverterIGBTTemp;
    uint16_t inverterMotorTemp;

    uint8_t driveState;
    uint8_t bmsState;
    uint8_t imdState = 1; // healthy when high
    uint8_t inverterStatus;
    uint8_t bmsCommand;

    bool brakePressed;
    bool lvVoltageWarning;

    bool pdmGenEfuseTriggered;
    bool pdmACEfuseTriggered;

    uint32_t inverterAhDrawn;
    uint32_t inverterAhCharged;

    uint32_t inverterWhDrawn;
    uint32_t inverterWhCharged;

    int32_t EcuSetCurrent;
    int32_t EcuSetCurrentBrake;

    uint8_t pump_duty_cycle;
    int16_t fan_duty_cycle;

    bool active_aero_state;
    int16_t active_aero_position;

    uint8_t accel_lut_id_response;

    bool reset_gen_efuse;
    bool reset_ac_efuse;

    bool igbt_temp_limiting;
    bool battery_temp_limiting;
    bool motor_temp_limiting;

    uint8_t torque_status;

    float flo_temperature_0;
    float flo_temperature_1;
    float flo_temperature_2;
    float flo_temperature_3;

    float fli_temperature_4;
    float fli_temperature_5;
    float fli_temperature_6;
    float fli_temperature_7;

    float fro_temperature_0;
    float fro_temperature_1;
    float fro_temperature_2;
    float fro_temperature_3;

    float fri_temperature_4;
    float fri_temperature_5;
    float fri_temperature_6;
    float fri_temperature_7;

    float blo_temperature_0;
    float blo_temperature_1;
    float blo_temperature_2;
    float blo_temperature_3;

    float cellTemperatures[NUM_TEMP_CELLS];
    float cellVoltages[NUM_VOLT_CELLS];

    DriveBusData()
    {
        memset(bmsFaults, 0, sizeof(bool) * BMS_FAULT_COUNT);
        memset(ecuFaults, 0, sizeof(bool) * ECU_FAULT_COUNT);
        imdState = 1;
    }

    bool faultPresent() const
    {
        if (driveState != 0) // if not in off, don't ignore undervoltage
            return bmsFaults[BMS_FAULT_SUMMARY] || ecuFaults[ECU_FAULT_PRESENT] || inverterStatus != 0 || imdState == 0;

        // if we are off, ignore the undervoltage
        return bmsFaults[BMS_FAULT_SUMMARY] || ecuFaults[ECU_FAULT_PRESENT] || (inverterStatus != 0 && inverterStatus != 2) || imdState == 0;
    }

    float averageWheelRPM() const
    {
        // return std::max({wheelSpeeds[0], wheelSpeeds[1], wheelSpeeds[2], wheelSpeeds[3]});
        return ((wheelSpeeds[0] + wheelSpeeds[1] + wheelSpeeds[2] + wheelSpeeds[3])) / 2; // only by two rn cause only two wheel speeds
    }

    float vehicleSpeedMPH() const;
};

class DriveBus
{
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

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) flo_temperature_0_signal;
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) flo_temperature_1_signal;
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) flo_temperature_2_signal;
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) flo_temperature_3_signal;
    CANRXMessage<4> rx_flo_temps{_driveBus, 0x241, flo_temperature_0_signal, flo_temperature_1_signal, flo_temperature_2_signal, flo_temperature_3_signal};

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) fli_temperature_4_signal;
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) fli_temperature_5_signal;
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) fli_temperature_6_signal;
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) fli_temperature_7_signal;
    CANRXMessage<4> rx_daq_wheel_front_left_inner_temps{_driveBus, 0x242, fli_temperature_4_signal, fli_temperature_5_signal, fli_temperature_6_signal, fli_temperature_7_signal};

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) fro_temperature_0_signal;
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) fro_temperature_1_signal;
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) fro_temperature_2_signal;
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) fro_temperature_3_signal;
    CANRXMessage<4> rx_daq_wheel_front_right_outer_temps{_driveBus, 0x243, fro_temperature_0_signal, fro_temperature_1_signal, fro_temperature_2_signal, fro_temperature_3_signal};

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) fri_temperature_4_signal;
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) fri_temperature_5_signal;
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) fri_temperature_6_signal;
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) fri_temperature_7_signal;
    CANRXMessage<4> rx_daq_wheel_front_right_inner_temps{_driveBus, 0x244, fri_temperature_4_signal, fri_temperature_5_signal, fri_temperature_6_signal, fri_temperature_7_signal};

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) blo_temperature_0_signal;
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) blo_temperature_1_signal;
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) blo_temperature_2_signal;
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) blo_temperature_3_signal;
    CANRXMessage<4> rx_daq_wheel_back_left_outer_temps{_driveBus, 0x245, blo_temperature_0_signal, blo_temperature_1_signal, blo_temperature_2_signal, blo_temperature_3_signal};




    // ECU Stuff
    MakeUnsignedCANSignal(uint8_t, 0, 8, 1, 0) drive_state_signal;
    CANRXMessage<1> rx_drive_state{_driveBus, 0x206,
                                   [this]()
                                   {
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

    MakeUnsignedCANSignal(uint8_t, 0, 8, 1, 0) pump_duty_cycle_signal;
    MakeUnsignedCANSignal(int16_t, 8, 8, 1, 0) fan_duty_cycle_signal;
    CANRXMessage<2> rx_ecu_pump_fan_command{_driveBus, 0x209, pump_duty_cycle_signal, fan_duty_cycle_signal};

    MakeUnsignedCANSignal(bool, 0, 1, 1, 0) active_aero_state_signal;
    MakeUnsignedCANSignal(int16_t, 1, 16, 1, 0) active_aero_position_signal;
    CANRXMessage<2> rx_ecu_active_aero_command{_driveBus, 0x208, active_aero_state_signal, active_aero_position_signal};

    MakeUnsignedCANSignal(uint8_t, 0, 8, 1, 0) accel_lut_id_response_signal;
    CANRXMessage<1> rx_accel_lut_id_response{_driveBus, 0x20A, accel_lut_id_response_signal};

    MakeUnsignedCANSignal(bool, 0, 1, 1, 0) igbt_temp_limiting_signal;
    MakeUnsignedCANSignal(bool, 1, 1, 1, 0) battery_temp_limiting_signal;
    MakeUnsignedCANSignal(bool, 2, 1, 1, 0) motor_temp_limiting_signal;
    CANRXMessage<3> rx_ecu_temp_limiting_status{_driveBus, 0x20B, igbt_temp_limiting_signal, battery_temp_limiting_signal, motor_temp_limiting_signal};

    MakeUnsignedCANSignal(uint8_t, 0, 8, 1, 0) torque_status_signal;
    CANRXMessage<1> rx_ecu_torque_status{_driveBus, 0x20C, torque_status_signal};



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

    MakeUnsignedCANSignal(bool, 0, 8, 1, 0) gen_efuse_triggered;
    MakeUnsignedCANSignal(bool, 8, 8, 1, 0) ac_efuse_triggered;
    CANRXMessage<2> rx_pdm_efuse{_driveBus, 0x2A3, gen_efuse_triggered, ac_efuse_triggered};

    MakeUnsignedCANSignal(bool, 0, 8, 1, 0) reset_gen_efuse_signal;
    MakeUnsignedCANSignal(bool, 8, 8, 1, 0) reset_ac_efuse_signal;
    CANRXMessage<2> rx_pdm_efuse_reset{_driveBus, 0x2A4, reset_gen_efuse_signal, reset_ac_efuse_signal};

    // inverter stuff
    MakeUnsignedCANSignal(uint8_t, 0, 8, 1.0, 0.0) inverter_fault_status_fault_code_signal;
    CANRXMessage<1> rx_inverter_fault_status{_driveBus, 0x280, inverter_fault_status_fault_code_signal};

    MakeUnsignedCANSignal(uint32_t, 0, 32, 0.0001, 0) ah_drawn_signal;
    MakeUnsignedCANSignal(uint32_t, 32, 32, 0.0001, 0) ah_charged_signal;
    CANRXMessage<2> rx_inverter_current_draw{_driveBus, 0x283, ah_drawn_signal, ah_charged_signal};

    MakeUnsignedCANSignal(uint32_t, 0, 32, 0.001, 0) wh_drawn_signal;
    MakeUnsignedCANSignal(uint32_t, 32, 32, 0.001, 0) wh_charged_signal;
    CANRXMessage<2> rx_inverter_power_draw{_driveBus, 0x284, wh_drawn_signal, wh_charged_signal};

    MakeSignedCANSignal(int16_t, 0, 16, 1.0, 0.0) inverter_motor_status_rpm;
    MakeSignedCANSignal(int16_t, 16, 16, 0.1, 0.0) inverter_motor_status_motor_current;
    MakeSignedCANSignal(int16_t, 32, 16, 0.1, 0.0) inverter_motor_status_dc_voltage;
    MakeSignedCANSignal(int16_t, 48, 16, 0.1, 0.0) inverter_motor_status_dc_current;

    CANRXMessage<4> rx_inverter_motor_status{_driveBus, 0x281, inverter_motor_status_rpm, inverter_motor_status_motor_current, inverter_motor_status_dc_voltage, inverter_motor_status_dc_current};

    MakeUnsignedCANSignal(uint8_t, 0, 8, 1.0, 0.0) ecu_bms_command_message_bms_command {};
    CANRXMessage<1> ecu_bms_command_message{_driveBus, 0x205, ecu_bms_command_message_bms_command};

    MakeSignedCANSignal(int16_t, 0, 16, 1.0, 0.0) ecu_brake_front_brake_pressure {};
    MakeSignedCANSignal(int16_t, 16, 16, 1.0, 0.0) ecu_brake_rear_brake_pressure {};
    MakeSignedCANSignal(bool, 32, 8, 1.0, 0.0) ecu_brake_brake_pressed {};
    CANRXMessage<3> ecu_brake{_driveBus, 0x203, ecu_brake_front_brake_pressure, ecu_brake_rear_brake_pressure, ecu_brake_brake_pressed};

    MakeSignedCANSignal(int16_t, 0, 16, 1.0, 0.0) ecu_throttle_apps1_throttle {};
    MakeSignedCANSignal(int16_t, 16, 16, 1.0, 0.0) ecu_throttle_apps2_throttle {};
    CANRXMessage<2> ecu_throttle{_driveBus, 0x202, ecu_throttle_apps1_throttle, ecu_throttle_apps2_throttle};

    MakeUnsignedCANSignal(uint16_t, 0, 16, 0.1, 0.0) inverter_temp_status_igbt_temp {};
    MakeUnsignedCANSignal(uint16_t, 16, 16, 0.1, 0.0) inverter_temp_status_motor_temp {};
    CANRXMessage<2> inverter_temp_status{_driveBus, 0x282, inverter_temp_status_igbt_temp, inverter_temp_status_motor_temp};

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) pdm_current_gen_amps {};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) pdm_current_fan_amps {};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) pdm_current_pump_amps {};
    CANRXMessage<3> pdm_current{_driveBus, 0x2A1, pdm_current_gen_amps, pdm_current_fan_amps, pdm_current_pump_amps};

    MakeSignedCANSignal(int32_t, 0, 32, 0.001, 0.0) ecu_set_current_signal;
    CANRXMessage<1> rx_ecu_set_current{_driveBus, 0x200, ecu_set_current_signal};

    MakeSignedCANSignal(int32_t, 0, 32, 0.001, 0.0) ecu_set_current_brake_signal;
    CANRXMessage<1> rx_ecu_set_current_brake{_driveBus, 0x201, ecu_set_current_brake_signal};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_0;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_1;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_2;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_3;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_4;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_5;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_6;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_0;
    CANRXMessage<8> rx_bms_voltages_0{_driveBus, 0x153, cell_v_0, cell_v_1, cell_v_2, cell_v_3, cell_v_4, cell_v_5, cell_v_6, cell_ocv_offset_0};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_7;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_8;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_9;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_10;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_11;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_12;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_13;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_1;
    CANRXMessage<8> rx_bms_voltages_1{_driveBus, 0x154, cell_v_7, cell_v_8, cell_v_9, cell_v_10, cell_v_11, cell_v_12, cell_v_13, cell_ocv_offset_1};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_14;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_15;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_16;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_17;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_18;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_19;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_20;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_2;
    CANRXMessage<8> rx_bms_voltages_2{_driveBus, 0x155, cell_v_14, cell_v_15, cell_v_16, cell_v_17, cell_v_18, cell_v_19, cell_v_20, cell_ocv_offset_2};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_21;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_22;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_23;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_24;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_25;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_26;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_27;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_3;
    CANRXMessage<8> rx_bms_voltages_3{_driveBus, 0x156, cell_v_21, cell_v_22, cell_v_23, cell_v_24, cell_v_25, cell_v_26, cell_v_27, cell_ocv_offset_3};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_28;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_29;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_30;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_31;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_32;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_33;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_34;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_4;
    CANRXMessage<8> rx_bms_voltages_4{_driveBus, 0x157, cell_v_28, cell_v_29, cell_v_30, cell_v_31, cell_v_32, cell_v_33, cell_v_34, cell_ocv_offset_4};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_35;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_36;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_37;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_38;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_39;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_40;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_41;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_5;
    CANRXMessage<8> rx_bms_voltages_5{_driveBus, 0x158, cell_v_35, cell_v_36, cell_v_37, cell_v_38, cell_v_39, cell_v_40, cell_v_41, cell_ocv_offset_5};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_42;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_43;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_44;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_45;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_46;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_47;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_48;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_6;
    CANRXMessage<8> rx_bms_voltages_6{_driveBus, 0x159, cell_v_42, cell_v_43, cell_v_44, cell_v_45, cell_v_46, cell_v_47, cell_v_48, cell_ocv_offset_6};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_49;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_50;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_51;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_52;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_53;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_54;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_55;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_7;
    CANRXMessage<8> rx_bms_voltages_7{_driveBus, 0x15A, cell_v_49, cell_v_50, cell_v_51, cell_v_52, cell_v_53, cell_v_54, cell_v_55, cell_ocv_offset_7};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_56;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_57;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_58;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_59;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_60;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_61;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_62;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_8;
    CANRXMessage<8> rx_bms_voltages_8{_driveBus, 0x15B, cell_v_56, cell_v_57, cell_v_58, cell_v_59, cell_v_60, cell_v_61, cell_v_62, cell_ocv_offset_8};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_63;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_64;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_65;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_66;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_67;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_68;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_69;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_9;
    CANRXMessage<8> rx_bms_voltages_9{_driveBus, 0x15C, cell_v_63, cell_v_64, cell_v_65, cell_v_66, cell_v_67, cell_v_68, cell_v_69, cell_ocv_offset_9};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_70;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_71;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_72;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_73;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_74;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_75;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_76;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_10;
    CANRXMessage<8> rx_bms_voltages_10{_driveBus, 0x15D, cell_v_70, cell_v_71, cell_v_72, cell_v_73, cell_v_74, cell_v_75, cell_v_76, cell_ocv_offset_10};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_77;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_78;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_79;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_80;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_81;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_82;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_83;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_11;
    CANRXMessage<8> rx_bms_voltages_11{_driveBus, 0x15E, cell_v_77, cell_v_78, cell_v_79, cell_v_80, cell_v_81, cell_v_82, cell_v_83, cell_ocv_offset_11};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_84;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_85;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_86;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_87;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_88;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_89;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_90;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_12;
    CANRXMessage<8> rx_bms_voltages_12{_driveBus, 0x15F, cell_v_84, cell_v_85, cell_v_86, cell_v_87, cell_v_88, cell_v_89, cell_v_90, cell_ocv_offset_12};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_91;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_92;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_93;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_94;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_95;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_96;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_97;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_13;
    CANRXMessage<8> rx_bms_voltages_13{_driveBus, 0x160, cell_v_91, cell_v_92, cell_v_93, cell_v_94, cell_v_95, cell_v_96, cell_v_97, cell_ocv_offset_13};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_98;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_99;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_100;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_101;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_102;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_103;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_104;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_14;
    CANRXMessage<8> rx_bms_voltages_14{_driveBus, 0x161, cell_v_98, cell_v_99, cell_v_100, cell_v_101, cell_v_102, cell_v_103, cell_v_104, cell_ocv_offset_14};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_105;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_106;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_107;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_108;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_109;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_110;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_111;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_15;
    CANRXMessage<8> rx_bms_voltages_15{_driveBus, 0x162, cell_v_105, cell_v_106, cell_v_107, cell_v_108, cell_v_109, cell_v_110, cell_v_111, cell_ocv_offset_15};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_112;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_113;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_114;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_115;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_116;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_117;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_118;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_16;
    CANRXMessage<8> rx_bms_voltages_16{_driveBus, 0x163, cell_v_112, cell_v_113, cell_v_114, cell_v_115, cell_v_116, cell_v_117, cell_v_118, cell_ocv_offset_16};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_119;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_120;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_121;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_122;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_123;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_124;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_125;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_17;
    CANRXMessage<8> rx_bms_voltages_17{_driveBus, 0x164, cell_v_119, cell_v_120, cell_v_121, cell_v_122, cell_v_123, cell_v_124, cell_v_125, cell_ocv_offset_17};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_126;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_127;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_128;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_129;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_130;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_131;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_132;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_18;
    CANRXMessage<8> rx_bms_voltages_18{_driveBus, 0x165, cell_v_126, cell_v_127, cell_v_128, cell_v_129, cell_v_130, cell_v_131, cell_v_132, cell_ocv_offset_18};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_133;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_134;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_135;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_136;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_137;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_138;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_139;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_19;
    CANRXMessage<8> rx_bms_voltages_19{_driveBus, 0x166, cell_v_133, cell_v_134, cell_v_135, cell_v_136, cell_v_137, cell_v_138, cell_v_139, cell_ocv_offset_19};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_0;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_1;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_2;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_3;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_4;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_5;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_6;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_7;
    CANRXMessage<8> rx_bms_temps_0{_driveBus, 0x167, cell_t_0, cell_t_1, cell_t_2, cell_t_3, cell_t_4, cell_t_5, cell_t_6, cell_t_7};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_8;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_9;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_10;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_11;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_12;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_13;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_14;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_15;
    CANRXMessage<8> rx_bms_temps_1{_driveBus, 0x168, cell_t_8, cell_t_9, cell_t_10, cell_t_11, cell_t_12, cell_t_13, cell_t_14, cell_t_15};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_16;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_17;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_18;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_19;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_20;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_21;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_22;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_23;
    CANRXMessage<8> rx_bms_temps_2{_driveBus, 0x169, cell_t_16, cell_t_17, cell_t_18, cell_t_19, cell_t_20, cell_t_21, cell_t_22, cell_t_23};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_24;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_25;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_26;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_27;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_28;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_29;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_30;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_31;
    CANRXMessage<8> rx_bms_temps_3{_driveBus, 0x16A, cell_t_24, cell_t_25, cell_t_26, cell_t_27, cell_t_28, cell_t_29, cell_t_30, cell_t_31};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_32;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_33;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_34;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_35;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_36;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_37;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_38;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_39;
    CANRXMessage<8> rx_bms_temps_4{_driveBus, 0x16B, cell_t_32, cell_t_33, cell_t_34, cell_t_35, cell_t_36, cell_t_37, cell_t_38, cell_t_39};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_40;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_41;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_42;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_43;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_44;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_45;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_46;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_47;
    CANRXMessage<8> rx_bms_temps_5{_driveBus, 0x16C, cell_t_40, cell_t_41, cell_t_42, cell_t_43, cell_t_44, cell_t_45, cell_t_46, cell_t_47};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_48;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_49;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_50;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_51;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_52;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_53;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_54;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_55;
    CANRXMessage<8> rx_bms_temps_6{_driveBus, 0x16D, cell_t_48, cell_t_49, cell_t_50, cell_t_51, cell_t_52, cell_t_53, cell_t_54, cell_t_55};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_56;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_57;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_58;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_59;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_60;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_61;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_62;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_63;
    CANRXMessage<8> rx_bms_temps_7{_driveBus, 0x16E, cell_t_56, cell_t_57, cell_t_58, cell_t_59, cell_t_60, cell_t_61, cell_t_62, cell_t_63};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_64;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_65;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_66;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_67;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_68;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_69;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_70;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_71;
    CANRXMessage<8> rx_bms_temps_8{_driveBus, 0x16F, cell_t_64, cell_t_65, cell_t_66, cell_t_67, cell_t_68, cell_t_69, cell_t_70, cell_t_71};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_72;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_73;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_74;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_75;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_76;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_77;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_78;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_79;
    CANRXMessage<8> rx_bms_temps_9{_driveBus, 0x170, cell_t_72, cell_t_73, cell_t_74, cell_t_75, cell_t_76, cell_t_77, cell_t_78, cell_t_79};

#ifdef DRIVE_DEBUG
    uint64_t _debugStartTime = 0;
    uint64_t _debugLastFaultUpdate = 0;
#endif
};

#endif // __DRIVE_BUS_H__