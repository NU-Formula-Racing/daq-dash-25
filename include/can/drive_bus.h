#ifndef __DRIVE_BUS_H__
#define __DRIVE_BUS_H__

#include <Arduino.h>
#include <CAN.h>

#include <string>
#include <map>

#include "define.h"
#include "sound.h"
#include "virtualTimer.h"
#include "throttle_lut.h"

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
    float wheelSpeeds[4];

    uint8_t driveState;
    uint8_t bmsState;
    uint8_t imdState = 1; // healthy when high
    uint8_t bmsSOC;
    uint8_t inverterStatus;

    float HVVoltage;
    float LVVoltage;
    float batteryTemp;
    float maxCellTemp;
    float minCellTemp;
    float maxCellVoltage;
    float minCellVoltage;

    bool bmsFaults[BMS_FAULT_COUNT];
    bool ecuFaults[ECU_FAULT_COUNT];

    DriveBusData()
    {
        memset(bmsFaults, 0, sizeof(bool) * BMS_FAULT_COUNT);
        memset(ecuFaults, 0, sizeof(bool) * ECU_FAULT_COUNT);
        imdState = 1;
    }

    bool faultPresent() const
    {
        // return inverterStatus != 0;;
        return bmsFaults[BMS_FAULT_SUMMARY] || ecuFaults[ECU_FAULT_PRESENT] || (inverterStatus != 0 && inverterStatus != 0x02) || imdState == 0;
        // return bmsFaults[BMS_FAULT_SUMMARY] || ecuFaults[ECU_FAULT_PRESENT] || imdState == 0;
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

    // updates can tx signals for sending throttle map over CAN
    void sendLUTCan(ThrottleLUT throttleLUT);

    // tick can tx timer
    void tickTimerCANTx();

private:
    DriveBusData _data;
    DriveBusData _prevData;
    TeensyCAN<3> _driveBus;
    VirtualTimerGroup _timerGroup{};

    // workaround for cyclical dependency
    void playReadyToDriveSound();

    // all of the CAN message stuff and setup

    // Wheel speeds
    MakeUnsignedCANSignal(float, 0, 16, 1, 0) fl_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 1, 0) fl_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 1, 0) fl_wheel_load_signal;
    CANRXMessage<3> rx_fl_wheel_speed{_driveBus, 0x249, fl_wheel_speed_signal, fl_wheel_displacement_signal, fl_wheel_load_signal};

    MakeUnsignedCANSignal(float, 0, 16, 1, 0) fr_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 1, 0) fr_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 1, 0) fr_wheel_load_signal;
    CANRXMessage<3> rx_fr_wheel_speed{_driveBus, 0x24A, fr_wheel_speed_signal, fr_wheel_displacement_signal, fr_wheel_load_signal};

    MakeUnsignedCANSignal(float, 0, 16, 1, 0) bl_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 1, 0) bl_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 1, 0) bl_wheel_load_signal;
    CANRXMessage<3> rx_bl_wheel_speed{_driveBus, 0x24B, bl_wheel_speed_signal, bl_wheel_displacement_signal, bl_wheel_load_signal};

    MakeUnsignedCANSignal(float, 0, 16, 1, 0) br_wheel_speed_signal;
    MakeUnsignedCANSignal(float, 16, 16, 1, 0) br_wheel_displacement_signal;
    MakeUnsignedCANSignal(float, 32, 16, 1, 0) br_wheel_load_signal;
    CANRXMessage<3> rx_br_wheel_speed{_driveBus, 0x24C, br_wheel_speed_signal, br_wheel_displacement_signal, br_wheel_load_signal};

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

    // ECU LUT Tx Messages - 15 messages containing xy pairs + metadata message
    // address space: 0x2B0 - 0x2BF
    MakeUnsignedCANSignal(uint8_t, 0, 8, 1, 0) file_status;
    MakeUnsignedCANSignal(uint8_t, 8, 8, 1, 0) num_lut_pairs;
    MakeUnsignedCANSignal(uint8_t, 16, 8, 1, 0) interp_type;
    MakeUnsignedCANSignal(uint8_t, 24, 8, 1, 0) lut_id;
    CANTXMessage<4> tx_daq_lut_metadata{_driveBus, 0x2B0, 4, 100, _timerGroup, num_lut_pairs, interp_type, lut_id, file_status};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_zero;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_zero;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_one;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_one;
    CANTXMessage<4> tx_daq_lut_pair_zero_one{_driveBus, 0x2B1, 8, 100, _timerGroup, x_zero, y_zero, x_one, y_one};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_two;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_two;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_three;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_three;
    CANTXMessage<4> tx_daq_lut_pair_two_three{_driveBus, 0x2B2, 8, 100, _timerGroup, x_two, y_two, x_three, y_three};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_four;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_four;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_five;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_five;
    CANTXMessage<4> tx_daq_lut_pair_four_five{_driveBus, 0x2B3, 8, 100, _timerGroup, x_four, y_four, x_five, y_five};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_six;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_six;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_seven;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_seven;
    CANTXMessage<4> tx_daq_lut_pair_six_seven{_driveBus, 0x2B4, 8, 100, _timerGroup, x_six, y_six, x_seven, y_seven};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_eight;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_eight;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_nine;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_nine;
    CANTXMessage<4> tx_daq_lut_pair_eight_nine{_driveBus, 0x2B5, 8, 100, _timerGroup, x_eight, y_eight, x_nine, y_nine};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_ten;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_ten;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_eleven;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_eleven;
    CANTXMessage<4> tx_daq_lut_pair_ten_eleven{_driveBus, 0x2B6, 8, 100, _timerGroup, x_ten, y_ten, x_eleven, y_eleven};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_twelve;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_twelve;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_thirteen;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_thirteen;
    CANTXMessage<4> tx_daq_lut_pair_twelve_thirteen{_driveBus, 0x2B7, 8, 100, _timerGroup, x_twelve, y_twelve, x_thirteen, y_thirteen};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_fourteen;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_fourteen;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_fifteen;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_fifteen;
    CANTXMessage<4> tx_daq_lut_pair_fourteen_fifteen{_driveBus, 0x2B8, 8, 100, _timerGroup, x_fourteen, y_fourteen, x_fifteen, y_fifteen};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_sixteen;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_sixteen;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_seventeen;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_seventeen;
    CANTXMessage<4> tx_daq_lut_pair_sixteen_seventeen{_driveBus, 0x2B9, 8, 100, _timerGroup, x_sixteen, y_sixteen, x_seventeen, y_seventeen};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_eighteen;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_eighteen;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_nineteen;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_nineteen;
    CANTXMessage<4> tx_daq_lut_pair_eighteen_nineteen{_driveBus, 0x2BA, 8, 100, _timerGroup, x_eighteen, y_eighteen, x_nineteen, y_nineteen};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_twenty;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_twenty;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_twenty_one;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_twenty_one;
    CANTXMessage<4> tx_daq_lut_pair_twenty_twenty_one{_driveBus, 0x2BB, 8, 100, _timerGroup, x_twenty, y_twenty, x_twenty_one, y_twenty_one};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_twenty_two;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_twenty_two;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_twenty_three;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_twenty_three;
    CANTXMessage<4> tx_daq_lut_pair_twenty_two_twenty_three{_driveBus, 0x2BC, 8, 100, _timerGroup, x_twenty_two, y_twenty_two, x_twenty_three, y_twenty_three};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_twenty_four;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_twenty_four;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_twenty_five;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_twenty_five;
    CANTXMessage<4> tx_daq_lut_pair_twenty_four_twenty_five{_driveBus, 0x2BD, 8, 100, _timerGroup, x_twenty_four, y_twenty_four, x_twenty_five, y_twenty_five};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_twenty_six;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_twenty_six;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_twenty_seven;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_twenty_seven;
    CANTXMessage<4> tx_daq_lut_pair_twenty_six_twenty_seven{_driveBus, 0x2BE, 8, 100, _timerGroup, x_twenty_six, y_twenty_six, x_twenty_seven, y_twenty_seven};

    MakeUnsignedCANSignal(int16_t, 0, 16, 1, 0) x_twenty_eight;
    MakeUnsignedCANSignal(float, 16, 16, 0.01, 0) y_twenty_eight;
    MakeUnsignedCANSignal(int16_t, 32, 16, 1, 0) x_twenty_nine;
    MakeUnsignedCANSignal(float, 48, 16, 0.01, 0) y_twenty_nine;
    CANTXMessage<4> tx_daq_lut_pair_twenty_eight_twenty_nine{_driveBus, 0x2BF, 8, 100, _timerGroup, x_twenty_eight, y_twenty_eight, x_twenty_nine, y_twenty_nine};

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
    MakeUnsignedCANSignal(float, 48, 8, 0.5, 0.0) bms_status_bms_soc;

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
    CANRXMessage<1> rx_inverter_fault_status{_driveBus, 0x208, inverter_fault_status_fault_code_signal};

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

#endif // __DRIVE_BUS_H__