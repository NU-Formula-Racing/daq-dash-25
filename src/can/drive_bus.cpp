#include "can/drive_bus.h"

#include "define.h"
#include "resources.h"

float DriveBusData::vehicleSpeedMPH() const
{
    return (averageWheelRPM() * M_PI * WHEEL_DIAMETER * 60) / (12 * 5280);
}

DriveBusData &DriveBus::getData()
{
    return _data;
}

DriveBusData &DriveBus::getPrevData()
{
    return _prevData;
}

void DriveBus::initialize()
{
    _driveBus.Initialize(ICAN::BaudRate::kBaud500K);

    _driveBus.RegisterRXMessage(rx_bms_voltages_0);
    _driveBus.RegisterRXMessage(rx_bms_voltages_1);
    _driveBus.RegisterRXMessage(rx_bms_voltages_2);
    _driveBus.RegisterRXMessage(rx_bms_voltages_3);
    _driveBus.RegisterRXMessage(rx_bms_voltages_4);
    _driveBus.RegisterRXMessage(rx_bms_voltages_5);
    _driveBus.RegisterRXMessage(rx_bms_voltages_6);
    _driveBus.RegisterRXMessage(rx_bms_voltages_7);
    _driveBus.RegisterRXMessage(rx_bms_voltages_8);
    _driveBus.RegisterRXMessage(rx_bms_voltages_9);
    _driveBus.RegisterRXMessage(rx_bms_voltages_10);
    _driveBus.RegisterRXMessage(rx_bms_voltages_11);
    _driveBus.RegisterRXMessage(rx_bms_voltages_12);
    _driveBus.RegisterRXMessage(rx_bms_voltages_13);
    _driveBus.RegisterRXMessage(rx_bms_voltages_14);
    _driveBus.RegisterRXMessage(rx_bms_voltages_15);
    _driveBus.RegisterRXMessage(rx_bms_voltages_16);
    _driveBus.RegisterRXMessage(rx_bms_voltages_17);
    _driveBus.RegisterRXMessage(rx_bms_voltages_18);
    _driveBus.RegisterRXMessage(rx_bms_voltages_19);

    _driveBus.RegisterRXMessage(rx_bms_temps_0);
    _driveBus.RegisterRXMessage(rx_bms_temps_1);
    _driveBus.RegisterRXMessage(rx_bms_temps_2);
    _driveBus.RegisterRXMessage(rx_bms_temps_3);
    _driveBus.RegisterRXMessage(rx_bms_temps_4);
    _driveBus.RegisterRXMessage(rx_bms_temps_5);
    _driveBus.RegisterRXMessage(rx_bms_temps_6);
    _driveBus.RegisterRXMessage(rx_bms_temps_7);
    _driveBus.RegisterRXMessage(rx_bms_temps_8);
    _driveBus.RegisterRXMessage(rx_bms_temps_9);

    _driveBus.RegisterRXMessage(rx_fl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_fr_wheel_speed);
    _driveBus.RegisterRXMessage(rx_bl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_br_wheel_speed);
    _driveBus.RegisterRXMessage(rx_drive_state);
    _driveBus.RegisterRXMessage(rx_hv_battery);
    _driveBus.RegisterRXMessage(rx_lv_voltage);
    _driveBus.RegisterRXMessage(rx_bms_faults);
    _driveBus.RegisterRXMessage(rx_ecu_implausibility);
    _driveBus.RegisterRXMessage(rx_bms_status);
    _driveBus.RegisterRXMessage(rx_inverter_fault_status);
    _driveBus.RegisterRXMessage(rx_inverter_motor_status);
    _driveBus.RegisterRXMessage(ecu_bms_command_message);
    _driveBus.RegisterRXMessage(ecu_brake);
    _driveBus.RegisterRXMessage(ecu_throttle);
    _driveBus.RegisterRXMessage(inverter_temp_status);
    _driveBus.RegisterRXMessage(pdm_current);

    _driveBus.RegisterRXMessage(rx_pdm_efuse);

    _driveBus.RegisterRXMessage(rx_inverter_current_draw);
    _driveBus.RegisterRXMessage(rx_inverter_power_draw);

    _driveBus.RegisterRXMessage(rx_ecu_set_current);
    _driveBus.RegisterRXMessage(rx_ecu_set_current_brake);
    _driveBus.RegisterRXMessage(rx_pdm_efuse_reset);

    _driveBus.RegisterRXMessage(rx_ecu_pump_fan_command);
    _driveBus.RegisterRXMessage(rx_ecu_active_aero_command);
    _driveBus.RegisterRXMessage(rx_accel_lut_id_response);

    _driveBus.RegisterRXMessage(rx_ecu_temp_limiting_status);
    _driveBus.RegisterRXMessage(rx_ecu_torque_status);
    _driveBus.RegisterRXMessage(rx_flo_temps);

    _driveBus.RegisterRXMessage(rx_daq_wheel_front_left_inner_temps);
    _driveBus.RegisterRXMessage(rx_daq_wheel_front_right_outer_temps);
    _driveBus.RegisterRXMessage(rx_daq_wheel_front_right_inner_temps);
    _driveBus.RegisterRXMessage(rx_daq_wheel_back_left_outer_temps);

    _driveBus.RegisterRXMessage(rx_daq_wheel_back_left_inner_temps);
    _driveBus.RegisterRXMessage(rx_daq_wheel_back_right_outer_temps);
    _driveBus.RegisterRXMessage(rx_daq_wheel_back_right_inner_temps);
    _driveBus.RegisterRXMessage(rx_daq_wheel_fl);
    _driveBus.RegisterRXMessage(rx_daq_wheel_fr);

    _driveBus.RegisterRXMessage(rx_daq_wheel_bl);
    _driveBus.RegisterRXMessage(rx_daq_wheel_br);
    _driveBus.RegisterRXMessage(rx_daq_lut_metadata);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_zero_one);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_two_three);

    _driveBus.RegisterRXMessage(rx_daq_lut_pair_four_five);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_six_seven);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_eight_nine);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_ten_eleven);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_twelve_thirteen);

    _driveBus.RegisterRXMessage(rx_daq_lut_pair_fourteen_fifteen);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_sixteen_seventeen);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_eighteen_nineteen);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_twenty_twenty_one);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_twenty_two_twenty_three);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_twenty_four_twenty_five);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_twenty_six_twenty_seven);
    _driveBus.RegisterRXMessage(rx_daq_lut_pair_twenty_eight_twenty_nine);

    _driveBus.RegisterRXMessage(rx_daq_dynamics_imu_acceleration);
    _driveBus.RegisterRXMessage(rx_daq_dynamics_imu_gryo);
    _driveBus.RegisterRXMessage(rx_daq_dynamics_pitot_lower);
    _driveBus.RegisterRXMessage(rx_daq_dynamics_pitot_upper);
    _driveBus.RegisterRXMessage(rx_daq_coolant_flow_rates);
    _driveBus.RegisterRXMessage(rx_daq_coolant_temps);

    _driveBus.RegisterRXMessage(rx_daq_rtc);
    _driveBus.RegisterRXMessage(rx_daq_gps);
    _driveBus.RegisterRXMessage(rx_daq_telemetry_status);

    _driveBus.RegisterRXMessage(rx_cm_request_drivetrain);
    _driveBus.RegisterRXMessage(rx_cm_request_daq);
    _driveBus.RegisterRXMessage(rx_cm_request_wheel);
    
    _driveBus.RegisterRXMessage(rx_cm_node_status_drivetrain);
    _driveBus.RegisterRXMessage(rx_cm_node_status_daq);
    _driveBus.RegisterRXMessage(rx_cm_node_status_wheel);

    // lowkey mad annoying but we gotta pull the imd status to be high
    bms_status_imd_state = 1; // drake why can't you be normal
    inverter_fault_status_fault_code_signal = 0;
}

// Helper: Generate a random float between min and max.
static float randomFloat(float min, float max)
{
    // Generate a value between 0 and 9999, then scale
    long r = random(0, 10000);
    return min + (max - min) * (r / 10000.0);
}

void DriveBus::update()
{
    this->_driveBus.Tick();
    this->timer_group.Tick(millis());
    // update the previous data
    this->_prevData = this->_data;

#ifdef DRIVE_DEBUG
    // Initialize debug timers if not set
    unsigned long now = millis();

    if (this->_debugStartTime == 0)
    {
        this->_debugStartTime = now;
        this->_debugLastFaultUpdate = now;
    }

    // Use random values for all numeric fields
    for (int i = 0; i < 4; i++)
    {
        // Set each wheel speed to a random value (for example, 0 to 100 RPM)
        this->_data.wheelSpeeds[i] = randomFloat(0.0f, 100.0f);
    }
    this->_data.driveState = random(0, 3);
    this->_data.HVVoltage = randomFloat(300.0f, 400.0f);
    this->_data.LVVoltage = randomFloat(12.0f, 15.0f);
    this->_data.batteryTemp = randomFloat(20.0f, 40.0f);
    this->_data.maxCellTemp = randomFloat(25.0f, 50.0f);
    this->_data.minCellTemp = randomFloat(15.0f, 30.0f);

    // Now update the fault arrays.
    // For the first 5 seconds, leave all fault flags false.
    if (now - this->_debugStartTime < 500UL)
    {
        for (int i = 0; i < BMS_FAULT_COUNT; i++)
        {
            this->_data.bmsFaults[i] = false;
        }
        for (int i = 0; i < ECU_FAULT_COUNT; i++)
        {
            this->_data.ecuFaults[i] = false;
        }
    }
    // After 5 seconds, every 10 seconds randomly update the fault flags.
    else if (now - this->_debugLastFaultUpdate >= 10000UL)
    {
        for (int i = 0; i < BMS_FAULT_COUNT; i++)
        {
            // Randomly set each BMS fault flag (true or false)
            this->_data.bmsFaults[i] = (random(0, 2) == 1);
        }
        for (int i = 0; i < ECU_FAULT_COUNT; i++)
        {
            // Randomly set each ECU fault flag
            this->_data.ecuFaults[i] = (random(0, 2) == 1);
        }
        this->_debugLastFaultUpdate = now;
    }

#else
    // Regular non-debug update: assign all signals as provided.

    this->_data.cellVoltages[0] = static_cast<float>(cell_v_0);
    this->_data.cellVoltages[1] = static_cast<float>(cell_v_1);
    this->_data.cellVoltages[2] = static_cast<float>(cell_v_2);
    this->_data.cellVoltages[3] = static_cast<float>(cell_v_3);
    this->_data.cellVoltages[4] = static_cast<float>(cell_v_4);
    this->_data.cellVoltages[5] = static_cast<float>(cell_v_5);
    this->_data.cellVoltages[6] = static_cast<float>(cell_v_6);
    this->_data.cellVoltages[7] = static_cast<float>(cell_v_7);
    this->_data.cellVoltages[8] = static_cast<float>(cell_v_8);
    this->_data.cellVoltages[9] = static_cast<float>(cell_v_9);
    this->_data.cellVoltages[10] = static_cast<float>(cell_v_10);
    this->_data.cellVoltages[11] = static_cast<float>(cell_v_11);
    this->_data.cellVoltages[12] = static_cast<float>(cell_v_12);
    this->_data.cellVoltages[13] = static_cast<float>(cell_v_13);
    this->_data.cellVoltages[14] = static_cast<float>(cell_v_14);
    this->_data.cellVoltages[15] = static_cast<float>(cell_v_15);
    this->_data.cellVoltages[16] = static_cast<float>(cell_v_16);
    this->_data.cellVoltages[17] = static_cast<float>(cell_v_17);
    this->_data.cellVoltages[18] = static_cast<float>(cell_v_18);
    this->_data.cellVoltages[19] = static_cast<float>(cell_v_19);
    this->_data.cellVoltages[20] = static_cast<float>(cell_v_20);
    this->_data.cellVoltages[21] = static_cast<float>(cell_v_21);
    this->_data.cellVoltages[22] = static_cast<float>(cell_v_22);
    this->_data.cellVoltages[23] = static_cast<float>(cell_v_23);
    this->_data.cellVoltages[24] = static_cast<float>(cell_v_24);
    this->_data.cellVoltages[25] = static_cast<float>(cell_v_25);
    this->_data.cellVoltages[26] = static_cast<float>(cell_v_26);
    this->_data.cellVoltages[27] = static_cast<float>(cell_v_27);
    this->_data.cellVoltages[28] = static_cast<float>(cell_v_28);
    this->_data.cellVoltages[29] = static_cast<float>(cell_v_29);
    this->_data.cellVoltages[30] = static_cast<float>(cell_v_30);
    this->_data.cellVoltages[31] = static_cast<float>(cell_v_31);
    this->_data.cellVoltages[32] = static_cast<float>(cell_v_32);
    this->_data.cellVoltages[33] = static_cast<float>(cell_v_33);
    this->_data.cellVoltages[34] = static_cast<float>(cell_v_34);
    this->_data.cellVoltages[35] = static_cast<float>(cell_v_35);
    this->_data.cellVoltages[36] = static_cast<float>(cell_v_36);
    this->_data.cellVoltages[37] = static_cast<float>(cell_v_37);
    this->_data.cellVoltages[38] = static_cast<float>(cell_v_38);
    this->_data.cellVoltages[39] = static_cast<float>(cell_v_39);
    this->_data.cellVoltages[40] = static_cast<float>(cell_v_40);
    this->_data.cellVoltages[41] = static_cast<float>(cell_v_41);
    this->_data.cellVoltages[42] = static_cast<float>(cell_v_42);
    this->_data.cellVoltages[43] = static_cast<float>(cell_v_43);
    this->_data.cellVoltages[44] = static_cast<float>(cell_v_44);
    this->_data.cellVoltages[45] = static_cast<float>(cell_v_45);
    this->_data.cellVoltages[46] = static_cast<float>(cell_v_46);
    this->_data.cellVoltages[47] = static_cast<float>(cell_v_47);
    this->_data.cellVoltages[48] = static_cast<float>(cell_v_48);
    this->_data.cellVoltages[49] = static_cast<float>(cell_v_49);
    this->_data.cellVoltages[50] = static_cast<float>(cell_v_50);
    this->_data.cellVoltages[51] = static_cast<float>(cell_v_51);
    this->_data.cellVoltages[52] = static_cast<float>(cell_v_52);
    this->_data.cellVoltages[53] = static_cast<float>(cell_v_53);
    this->_data.cellVoltages[54] = static_cast<float>(cell_v_54);
    this->_data.cellVoltages[55] = static_cast<float>(cell_v_55);
    this->_data.cellVoltages[56] = static_cast<float>(cell_v_56);
    this->_data.cellVoltages[57] = static_cast<float>(cell_v_57);
    this->_data.cellVoltages[58] = static_cast<float>(cell_v_58);
    this->_data.cellVoltages[59] = static_cast<float>(cell_v_59);
    this->_data.cellVoltages[60] = static_cast<float>(cell_v_60);
    this->_data.cellVoltages[61] = static_cast<float>(cell_v_61);
    this->_data.cellVoltages[62] = static_cast<float>(cell_v_62);
    this->_data.cellVoltages[63] = static_cast<float>(cell_v_63);
    this->_data.cellVoltages[64] = static_cast<float>(cell_v_64);
    this->_data.cellVoltages[65] = static_cast<float>(cell_v_65);
    this->_data.cellVoltages[66] = static_cast<float>(cell_v_66);
    this->_data.cellVoltages[67] = static_cast<float>(cell_v_67);
    this->_data.cellVoltages[68] = static_cast<float>(cell_v_68);
    this->_data.cellVoltages[69] = static_cast<float>(cell_v_69);
    this->_data.cellVoltages[70] = static_cast<float>(cell_v_70);
    this->_data.cellVoltages[71] = static_cast<float>(cell_v_71);
    this->_data.cellVoltages[72] = static_cast<float>(cell_v_72);
    this->_data.cellVoltages[73] = static_cast<float>(cell_v_73);
    this->_data.cellVoltages[74] = static_cast<float>(cell_v_74);
    this->_data.cellVoltages[75] = static_cast<float>(cell_v_75);
    this->_data.cellVoltages[76] = static_cast<float>(cell_v_76);
    this->_data.cellVoltages[77] = static_cast<float>(cell_v_77);
    this->_data.cellVoltages[78] = static_cast<float>(cell_v_78);
    this->_data.cellVoltages[79] = static_cast<float>(cell_v_79);
    this->_data.cellVoltages[80] = static_cast<float>(cell_v_80);
    this->_data.cellVoltages[81] = static_cast<float>(cell_v_81);
    this->_data.cellVoltages[82] = static_cast<float>(cell_v_82);
    this->_data.cellVoltages[83] = static_cast<float>(cell_v_83);
    this->_data.cellVoltages[84] = static_cast<float>(cell_v_84);
    this->_data.cellVoltages[85] = static_cast<float>(cell_v_85);
    this->_data.cellVoltages[86] = static_cast<float>(cell_v_86);
    this->_data.cellVoltages[87] = static_cast<float>(cell_v_87);
    this->_data.cellVoltages[88] = static_cast<float>(cell_v_88);
    this->_data.cellVoltages[89] = static_cast<float>(cell_v_89);
    this->_data.cellVoltages[90] = static_cast<float>(cell_v_90);
    this->_data.cellVoltages[91] = static_cast<float>(cell_v_91);
    this->_data.cellVoltages[92] = static_cast<float>(cell_v_92);
    this->_data.cellVoltages[93] = static_cast<float>(cell_v_93);
    this->_data.cellVoltages[94] = static_cast<float>(cell_v_94);
    this->_data.cellVoltages[95] = static_cast<float>(cell_v_95);
    this->_data.cellVoltages[96] = static_cast<float>(cell_v_96);
    this->_data.cellVoltages[97] = static_cast<float>(cell_v_97);
    this->_data.cellVoltages[98] = static_cast<float>(cell_v_98);
    this->_data.cellVoltages[99] = static_cast<float>(cell_v_99);
    this->_data.cellVoltages[100] = static_cast<float>(cell_v_100);
    this->_data.cellVoltages[101] = static_cast<float>(cell_v_101);
    this->_data.cellVoltages[102] = static_cast<float>(cell_v_102);
    this->_data.cellVoltages[103] = static_cast<float>(cell_v_103);
    this->_data.cellVoltages[104] = static_cast<float>(cell_v_104);
    this->_data.cellVoltages[105] = static_cast<float>(cell_v_105);
    this->_data.cellVoltages[106] = static_cast<float>(cell_v_106);
    this->_data.cellVoltages[107] = static_cast<float>(cell_v_107);
    this->_data.cellVoltages[108] = static_cast<float>(cell_v_108);
    this->_data.cellVoltages[109] = static_cast<float>(cell_v_109);
    this->_data.cellVoltages[110] = static_cast<float>(cell_v_110);
    this->_data.cellVoltages[111] = static_cast<float>(cell_v_111);
    this->_data.cellVoltages[112] = static_cast<float>(cell_v_112);
    this->_data.cellVoltages[113] = static_cast<float>(cell_v_113);
    this->_data.cellVoltages[114] = static_cast<float>(cell_v_114);
    this->_data.cellVoltages[115] = static_cast<float>(cell_v_115);
    this->_data.cellVoltages[116] = static_cast<float>(cell_v_116);
    this->_data.cellVoltages[117] = static_cast<float>(cell_v_117);
    this->_data.cellVoltages[118] = static_cast<float>(cell_v_118);
    this->_data.cellVoltages[119] = static_cast<float>(cell_v_119);
    this->_data.cellVoltages[120] = static_cast<float>(cell_v_120);
    this->_data.cellVoltages[121] = static_cast<float>(cell_v_121);
    this->_data.cellVoltages[122] = static_cast<float>(cell_v_122);
    this->_data.cellVoltages[123] = static_cast<float>(cell_v_123);
    this->_data.cellVoltages[124] = static_cast<float>(cell_v_124);
    this->_data.cellVoltages[125] = static_cast<float>(cell_v_125);
    this->_data.cellVoltages[126] = static_cast<float>(cell_v_126);
    this->_data.cellVoltages[127] = static_cast<float>(cell_v_127);
    this->_data.cellVoltages[128] = static_cast<float>(cell_v_128);
    this->_data.cellVoltages[129] = static_cast<float>(cell_v_129);
    this->_data.cellVoltages[130] = static_cast<float>(cell_v_130);
    this->_data.cellVoltages[131] = static_cast<float>(cell_v_131);
    this->_data.cellVoltages[132] = static_cast<float>(cell_v_132);
    this->_data.cellVoltages[133] = static_cast<float>(cell_v_133);
    this->_data.cellVoltages[134] = static_cast<float>(cell_v_134);
    this->_data.cellVoltages[135] = static_cast<float>(cell_v_135);
    this->_data.cellVoltages[136] = static_cast<float>(cell_v_136);
    this->_data.cellVoltages[137] = static_cast<float>(cell_v_137);
    this->_data.cellVoltages[138] = static_cast<float>(cell_v_138);
    this->_data.cellVoltages[139] = static_cast<float>(cell_v_139);

    this->_data.cellTemperatures[0] = static_cast<float>(cell_t_0);
    this->_data.cellTemperatures[1] = static_cast<float>(cell_t_1);
    this->_data.cellTemperatures[2] = static_cast<float>(cell_t_2);
    this->_data.cellTemperatures[3] = static_cast<float>(cell_t_3);
    this->_data.cellTemperatures[4] = static_cast<float>(cell_t_4);
    this->_data.cellTemperatures[5] = static_cast<float>(cell_t_5);
    this->_data.cellTemperatures[6] = static_cast<float>(cell_t_6);
    this->_data.cellTemperatures[7] = static_cast<float>(cell_t_7);
    this->_data.cellTemperatures[8] = static_cast<float>(cell_t_8);
    this->_data.cellTemperatures[9] = static_cast<float>(cell_t_9);
    this->_data.cellTemperatures[10] = static_cast<float>(cell_t_10);
    this->_data.cellTemperatures[11] = static_cast<float>(cell_t_11);
    this->_data.cellTemperatures[12] = static_cast<float>(cell_t_12);
    this->_data.cellTemperatures[13] = static_cast<float>(cell_t_13);
    this->_data.cellTemperatures[14] = static_cast<float>(cell_t_14);
    this->_data.cellTemperatures[15] = static_cast<float>(cell_t_15);
    this->_data.cellTemperatures[16] = static_cast<float>(cell_t_16);
    this->_data.cellTemperatures[17] = static_cast<float>(cell_t_17);
    this->_data.cellTemperatures[18] = static_cast<float>(cell_t_18);
    this->_data.cellTemperatures[19] = static_cast<float>(cell_t_19);
    this->_data.cellTemperatures[20] = static_cast<float>(cell_t_20);
    this->_data.cellTemperatures[21] = static_cast<float>(cell_t_21);
    this->_data.cellTemperatures[22] = static_cast<float>(cell_t_22);
    this->_data.cellTemperatures[23] = static_cast<float>(cell_t_23);
    this->_data.cellTemperatures[24] = static_cast<float>(cell_t_24);
    this->_data.cellTemperatures[25] = static_cast<float>(cell_t_25);
    this->_data.cellTemperatures[26] = static_cast<float>(cell_t_26);
    this->_data.cellTemperatures[27] = static_cast<float>(cell_t_27);
    this->_data.cellTemperatures[28] = static_cast<float>(cell_t_28);
    this->_data.cellTemperatures[29] = static_cast<float>(cell_t_29);
    this->_data.cellTemperatures[30] = static_cast<float>(cell_t_30);
    this->_data.cellTemperatures[31] = static_cast<float>(cell_t_31);
    this->_data.cellTemperatures[32] = static_cast<float>(cell_t_32);
    this->_data.cellTemperatures[33] = static_cast<float>(cell_t_33);
    this->_data.cellTemperatures[34] = static_cast<float>(cell_t_34);
    this->_data.cellTemperatures[35] = static_cast<float>(cell_t_35);
    this->_data.cellTemperatures[36] = static_cast<float>(cell_t_36);
    this->_data.cellTemperatures[37] = static_cast<float>(cell_t_37);
    this->_data.cellTemperatures[38] = static_cast<float>(cell_t_38);
    this->_data.cellTemperatures[39] = static_cast<float>(cell_t_39);
    this->_data.cellTemperatures[40] = static_cast<float>(cell_t_40);
    this->_data.cellTemperatures[41] = static_cast<float>(cell_t_41);
    this->_data.cellTemperatures[42] = static_cast<float>(cell_t_42);
    this->_data.cellTemperatures[43] = static_cast<float>(cell_t_43);
    this->_data.cellTemperatures[44] = static_cast<float>(cell_t_44);
    this->_data.cellTemperatures[45] = static_cast<float>(cell_t_45);
    this->_data.cellTemperatures[46] = static_cast<float>(cell_t_46);
    this->_data.cellTemperatures[47] = static_cast<float>(cell_t_47);
    this->_data.cellTemperatures[48] = static_cast<float>(cell_t_48);
    this->_data.cellTemperatures[49] = static_cast<float>(cell_t_49);
    this->_data.cellTemperatures[50] = static_cast<float>(cell_t_50);
    this->_data.cellTemperatures[51] = static_cast<float>(cell_t_51);
    this->_data.cellTemperatures[52] = static_cast<float>(cell_t_52);
    this->_data.cellTemperatures[53] = static_cast<float>(cell_t_53);
    this->_data.cellTemperatures[54] = static_cast<float>(cell_t_54);
    this->_data.cellTemperatures[55] = static_cast<float>(cell_t_55);
    this->_data.cellTemperatures[56] = static_cast<float>(cell_t_56);
    this->_data.cellTemperatures[57] = static_cast<float>(cell_t_57);
    this->_data.cellTemperatures[58] = static_cast<float>(cell_t_58);
    this->_data.cellTemperatures[59] = static_cast<float>(cell_t_59);
    this->_data.cellTemperatures[60] = static_cast<float>(cell_t_60);
    this->_data.cellTemperatures[61] = static_cast<float>(cell_t_61);
    this->_data.cellTemperatures[62] = static_cast<float>(cell_t_62);
    this->_data.cellTemperatures[63] = static_cast<float>(cell_t_63);
    this->_data.cellTemperatures[64] = static_cast<float>(cell_t_64);
    this->_data.cellTemperatures[65] = static_cast<float>(cell_t_65);
    this->_data.cellTemperatures[66] = static_cast<float>(cell_t_66);
    this->_data.cellTemperatures[67] = static_cast<float>(cell_t_67);
    this->_data.cellTemperatures[68] = static_cast<float>(cell_t_68);
    this->_data.cellTemperatures[69] = static_cast<float>(cell_t_69);
    this->_data.cellTemperatures[70] = static_cast<float>(cell_t_70);
    this->_data.cellTemperatures[71] = static_cast<float>(cell_t_71);
    this->_data.cellTemperatures[72] = static_cast<float>(cell_t_72);
    this->_data.cellTemperatures[73] = static_cast<float>(cell_t_73);
    this->_data.cellTemperatures[74] = static_cast<float>(cell_t_74);
    this->_data.cellTemperatures[75] = static_cast<float>(cell_t_75);
    this->_data.cellTemperatures[76] = static_cast<float>(cell_t_76);
    this->_data.cellTemperatures[77] = static_cast<float>(cell_t_77);
    this->_data.cellTemperatures[78] = static_cast<float>(cell_t_78);
    this->_data.cellTemperatures[79] = static_cast<float>(cell_t_79);

    this->_data.wheelSpeeds[0] = fl_wheel_speed_signal;
    this->_data.wheelSpeeds[1] = fr_wheel_speed_signal;
    this->_data.wheelSpeeds[2] = bl_wheel_speed_signal;
    this->_data.wheelSpeeds[3] = br_wheel_speed_signal;

    this->_data.wheelDisplacement[0] = fl_wheel_displacement_signal;
    this->_data.wheelDisplacement[1] = fr_wheel_displacement_signal;
    this->_data.wheelDisplacement[2] = bl_wheel_displacement_signal;
    this->_data.wheelDisplacement[3] = br_wheel_displacement_signal;

    this->_data.prStrain[0] = fl_wheel_load_signal;
    this->_data.prStrain[1] = fr_wheel_load_signal;
    this->_data.prStrain[2] = bl_wheel_load_signal;
    this->_data.prStrain[3] = br_wheel_load_signal;

    this->_data.driveState = drive_state_signal;
    this->_data.hvVoltage = hv_voltage_signal;
    this->_data.lvVoltage = lv_voltage_signal;
    this->_data.bmsState = bms_status_bms_state;

    this->_data.imdState = bms_status_imd_state;

    this->_data.maxCellTemp = bms_status_max_cell_temp;
    this->_data.minCellTemp = bms_status_min_cell_temp;
    this->_data.maxCellVoltage = bms_status_max_cell_voltage;
    this->_data.minCellVoltage = bms_status_min_cell_voltage;
    this->_data.bmsSOC = bms_status_bms_soc;

    this->_data.inverterStatus = (static_cast<uint8_t>(inverter_fault_status_fault_code_signal));

    this->_data.bmsFaults[BMS_FAULT_SUMMARY] = (static_cast<bool>(bms_fault_summary_signal));
    this->_data.bmsFaults[BMS_FAULT_UNDER_VOLTAGE] = static_cast<bool>(bms_fault_under_voltage_signal);
    this->_data.bmsFaults[BMS_FAULT_OVER_VOLTAGE] = static_cast<bool>(bms_fault_over_voltage_signal);
    this->_data.bmsFaults[BMS_FAULT_UNDER_TEMP] = static_cast<bool>(bms_fault_under_temperature_signal);
    this->_data.bmsFaults[BMS_FAULT_OVER_TEMP] = static_cast<bool>(bms_fault_over_temperature_signal);
    this->_data.bmsFaults[BMS_FAULT_OVER_CURRENT] = static_cast<bool>(bms_fault_over_current_signal);
    this->_data.bmsFaults[BMS_FAULT_EXTERNAL_KILL] = static_cast<bool>(bms_fault_external_kill_signal);
    this->_data.bmsFaults[BMS_FAULT_OPEN_WIRE] = static_cast<bool>(bms_fault_open_wire_signal);

    uint16_t bmsFaultsRaw = 0;
    for (int i = 0; i < BMS_FAULT_COUNT; i++)
    {
        bmsFaultsRaw |= ((uint16_t)(this->_data.bmsFaults) << i);
    }

    this->_data.ecuFaults[ECU_FAULT_PRESENT] = static_cast<bool>(ecu_implausibility_present_signal);
    this->_data.ecuFaults[ECU_FAULT_APPSS_DISAGREEMENT] = static_cast<bool>(ecu_implausibility_appss_disagreement_imp_signal);
    this->_data.ecuFaults[ECU_FAULT_BPPC] = static_cast<bool>(ecu_implausibility_bppc_imp_signal);
    this->_data.ecuFaults[ECU_FAULT_BRAKE_INVALID] = static_cast<bool>(ecu_implausibility_brake_invalid_imp_signal);
    this->_data.ecuFaults[ECU_FAULT_APPPS_INVALID] = static_cast<bool>(ecu_implausibility_appss_invalid_imp_signal);

    this->_data.lvVoltage = static_cast<float>(lv_voltage_signal);

    this->_data.maxDischargeCurrent = max_discharge_current_signal;
    this->_data.maxRegenCurrent = max_regen_current_signal;
    this->_data.motorCurrent = inverter_motor_status_rpm;
    this->_data.motorCurrent = inverter_motor_status_motor_current;
    this->_data.motorDCVoltage = inverter_motor_status_dc_voltage;
    this->_data.motorDCCurrent = inverter_motor_status_dc_current;
    this->_data.motorRPM = inverter_motor_status_rpm;

    this->_data.bmsCommand = ecu_bms_command_message_bms_command;
    this->_data.frontBrakePressure = ecu_brake_front_brake_pressure;
    this->_data.rearBreakPressure = ecu_brake_rear_brake_pressure;

    this->_data.apps1 = ecu_throttle_apps1_throttle;
    this->_data.apps2 = ecu_throttle_apps2_throttle;

    this->_data.inverterIGBTTemp = inverter_temp_status_igbt_temp;
    this->_data.inverterMotorTemp = inverter_temp_status_motor_temp;

    this->_data.genAmps = pdm_current_gen_amps;
    this->_data.fanAmps = pdm_current_fan_amps;
    this->_data.pumpAmps = pdm_current_pump_amps;

    this->_data.pdmGenEfuseTriggered = gen_efuse_triggered;
    this->_data.pdmACEfuseTriggered = ac_efuse_triggered;

    this->_data.inverterAhDrawn = ah_drawn_signal;
    this->_data.inverterAhCharged = ah_charged_signal;

    this->_data.inverterWhDrawn = wh_drawn_signal;
    this->_data.inverterWhCharged = wh_charged_signal;

    this->_data.EcuSetCurrent = ecu_set_current_signal;
    this->_data.EcuSetCurrentBrake = ecu_set_current_brake_signal;
    this->_data.reset_gen_efuse = reset_gen_efuse_signal;
    this->_data.reset_ac_efuse = reset_ac_efuse_signal;

    this->_data.pump_duty_cycle = pump_duty_cycle_signal;
    this->_data.fan_duty_cycle = fan_duty_cycle_signal;
    this->_data.active_aero_state = active_aero_state_signal;
    this->_data.active_aero_position = active_aero_position_signal;
    this->_data.accel_lut_id_response = accel_lut_id_response_signal;

    this->_data.igbt_temp_limiting = igbt_temp_limiting_signal;
    this->_data.battery_temp_limiting = battery_temp_limiting_signal;
    this->_data.motor_temp_limiting = motor_temp_limiting_signal;
    this->_data.torque_status = torque_status_signal;

    this->_data.accel_lut_id_response = accel_lut_id_response_signal;

    this->_data.flo_temperature_0 = flo_temperature_0_signal;
    this->_data.flo_temperature_1 = flo_temperature_1_signal;
    this->_data.flo_temperature_2 = flo_temperature_2_signal;
    this->_data.flo_temperature_3 = flo_temperature_3_signal;

    this->_data.fli_temperature_4 = fli_temperature_4_signal;
    this->_data.fli_temperature_5 = fli_temperature_5_signal;
    this->_data.fli_temperature_6 = fli_temperature_6_signal;
    this->_data.fli_temperature_7 = fli_temperature_7_signal;

    this->_data.fro_temperature_0 = fro_temperature_0_signal;
    this->_data.fro_temperature_1 = fro_temperature_1_signal;
    this->_data.fro_temperature_2 = fro_temperature_2_signal;
    this->_data.fro_temperature_3 = fro_temperature_3_signal;

    this->_data.fri_temperature_4 = fri_temperature_4_signal;
    this->_data.fri_temperature_5 = fri_temperature_5_signal;
    this->_data.fri_temperature_6 = fri_temperature_6_signal;
    this->_data.fri_temperature_7 = fri_temperature_7_signal;

    this->_data.blo_temperature_0 = blo_temperature_0_signal;
    this->_data.blo_temperature_1 = blo_temperature_1_signal;
    this->_data.blo_temperature_2 = blo_temperature_2_signal;
    this->_data.blo_temperature_3 = blo_temperature_3_signal;

    this->_data.bli_temperature_4 = bli_temperature_4_signal;
    this->_data.bli_temperature_5 = bli_temperature_5_signal;
    this->_data.bli_temperature_6 = bli_temperature_6_signal;
    this->_data.bli_temperature_7 = bli_temperature_7_signal;

    this->_data.bro_temperature_0 = bro_temperature_0_signal;
    this->_data.bro_temperature_1 = bro_temperature_1_signal;
    this->_data.bro_temperature_2 = bro_temperature_2_signal;
    this->_data.bro_temperature_3 = bro_temperature_3_signal;

    this->_data.bri_temperature_4 = bri_temperature_4_signal;
    this->_data.bri_temperature_5 = bri_temperature_5_signal;
    this->_data.bri_temperature_6 = bri_temperature_6_signal;
    this->_data.bri_temperature_7 = bri_temperature_7_signal;

    this->_data.fl_speed = fl_speed_signal;
    this->_data.fl_displacement = fl_displacement_signal;
    this->_data.fl_load = fl_load_signal;

    this->_data.fr_speed = fr_speed_signal;
    this->_data.fr_displacement = fr_displacement_signal;
    this->_data.fr_load = fr_load_signal;

    this->_data.bl_speed = bl_speed_signal;
    this->_data.bl_displacement = bl_displacement_signal;
    this->_data.bl_load = bl_load_signal;

    this->_data.br_speed = br_speed_signal;
    this->_data.br_displacement = br_displacement_signal;
    this->_data.br_load = br_load_signal;

    this->_data.file_status = file_status_signal;
    this->_data.num_lut_pairs = num_lut_pairs_signal;
    this->_data.interp_type = interp_type_signal;
    this->_data.lut_id = lut_id_signal;

    this->_data.x_zero = x_zero_signal;
    this->_data.y_zero = y_zero_signal;
    this->_data.x_one = x_one_signal;
    this->_data.y_one = y_one_signal;

    this->_data.x_two = x_two_signal;
    this->_data.y_two = y_two_signal;
    this->_data.x_three = x_three_signal;
    this->_data.y_three = y_three_signal;

    this->_data.x_four = x_four_signal;
    this->_data.y_four = y_four_signal;
    this->_data.x_five = x_five_signal;
    this->_data.y_five = y_five_signal;

    this->_data.x_six = x_six_signal;
    this->_data.y_six = y_six_signal;
    this->_data.x_seven = x_seven_signal;
    this->_data.y_seven = y_seven_signal;

    this->_data.x_eight = x_eight_signal;
    this->_data.y_eight = y_eight_signal;
    this->_data.x_nine = x_nine_signal;
    this->_data.y_nine = y_nine_signal;

    this->_data.x_ten = x_ten_signal;
    this->_data.y_ten = y_ten_signal;
    this->_data.x_eleven = x_eleven_signal;
    this->_data.y_eleven = y_eleven_signal;

    this->_data.x_twelve = x_twelve_signal;
    this->_data.y_twelve = y_twelve_signal;
    this->_data.x_thirteen = x_thirteen_signal;
    this->_data.y_thirteen = y_thirteen_signal;

    this->_data.x_fourteen = x_fourteen_signal;
    this->_data.y_fourteen = y_fourteen_signal;
    this->_data.x_fifteen = x_fifteen_signal;
    this->_data.y_fifteen = y_fifteen_signal;

    this->_data.x_sixteen = x_sixteen_signal;
    this->_data.y_sixteen = y_sixteen_signal;
    this->_data.x_seventeen = x_seventeen_signal;
    this->_data.y_seventeen = y_seventeen_signal;

    this->_data.x_eighteen = x_eighteen_signal;
    this->_data.y_eighteen = y_eighteen_signal;
    this->_data.x_nineteen = x_nineteen_signal;
    this->_data.y_nineteen = y_nineteen_signal;

    this->_data.x_twenty = x_twenty_signal;
    this->_data.y_twenty = y_twenty_signal;
    this->_data.x_twenty_one = x_twenty_one_signal;
    this->_data.y_twenty_one = y_twenty_one_signal;

    this->_data.x_twenty_two = x_twenty_two_signal;
    this->_data.y_twenty_two = y_twenty_two_signal;
    this->_data.x_twenty_three = x_twenty_three_signal;
    this->_data.y_twenty_three = y_twenty_three_signal;

    this->_data.x_twenty_four = x_twenty_four_signal;
    this->_data.y_twenty_four = y_twenty_four_signal;
    this->_data.x_twenty_five = x_twenty_five_signal;
    this->_data.y_twenty_five = y_twenty_five_signal;

    this->_data.x_twenty_six = x_twenty_six_signal;
    this->_data.y_twenty_six = y_twenty_six_signal;
    this->_data.x_twenty_seven = x_twenty_seven_signal;
    this->_data.y_twenty_seven = y_twenty_seven_signal;

    this->_data.x_twenty_eight = x_twenty_eight_signal;
    this->_data.y_twenty_eight = y_twenty_eight_signal;
    this->_data.x_twenty_nine = x_twenty_nine_signal;
    this->_data.y_twenty_nine = y_twenty_nine_signal;

    this->_data.x_acceleration = x_acceleration_signal;
    this->_data.y_acceleration = y_acceleration_signal;
    this->_data.z_acceleration = z_acceleration_signal;

    this->_data.x_angular_speed = x_angular_speed_signal;
    this->_data.y_angular_speed = y_angular_speed_signal;
    this->_data.z_angular_speed = z_angular_speed_signal;

    this->_data.air_speed_0 = air_speed_0_signal;
    this->_data.air_speed_1 = air_speed_1_signal;
    this->_data.air_speed_2 = air_speed_2_signal;
    this->_data.air_speed_3 = air_speed_3_signal;

    this->_data.air_speed_4 = air_speed_4_signal;
    this->_data.air_speed_5 = air_speed_5_signal;
    this->_data.air_speed_6 = air_speed_6_signal;
    this->_data.air_speed_7 = air_speed_7_signal;

    this->_data.before_motor_flow_rate = before_motor_flow_rate_signal;
    this->_data.before_accumulator_flow_rate = before_accumulator_flow_rate_signal;

    this->_data.before_motor_temperature = before_motor_temperature_signal;
    this->_data.before_accumulator_temperature = before_accumulator_temperature_signal;

    this->_data.time_since_1970 = time_since_1970_signal;
    this->_data.longitude = longitude_signal;
    this->_data.latitude = latitude_signal;
    this->_data.wireless_status = wireless_status_signal;
    this->_data.logger_status = logger_status_signal;

    this->_data.ecu_enable_response = ecu_enable_response_signal;
    this->_data.bms_enable_response = bms_enable_response_signal;
    this->_data.pdm_enable_response = pdm_enable_response_signal;

    this->_data.dynamics_enable_response = dynamics_enable_response_signal;
    this->_data.front_enable_response = front_enable_response_signal;
    this->_data.telemetry_enable_response = telemetry_enable_response_signal;

    this->_data.bl_enable_response = bl_enable_response_signal;
    this->_data.br_enable_response = br_enable_response_signal;
    this->_data.fl_enable_response = fl_enable_response_signal;
    this->_data.fr_enable_response = fr_enable_response_signal;

    this->_data.ecu_status = ecu_status_signal;
    this->_data.bms_status = bms_status_signal;
    this->_data.pdm_status = pdm_status_signal;

    this->_data.dynamics_status = dynamics_status_signal;
    this->_data.front_status = front_status_signal;
    this->_data.telemetry_status = telemetry_status_signal;

    this->_data.bl_status = bl_status_signal;
    this->_data.br_status = br_status_signal;
    this->_data.fl_status = fl_status_signal;
    this->_data.fr_status = fr_status_signal;

    // this->_data.steering_angle = steering_angle_signal;
    this->steering_angle_signal = this->_data.steering_angle;



#endif
}

void DriveBus::playReadyToDriveSound()
{
    // check if we are changing into neutral

    uint8_t current = drive_state_signal;

    // are we channging into ON?
    if (current != DriveState::DS_ON)
    {
        return; // no need to play the sound
    }

    // we only play the sound if we are transitioning from neutral to on
    // have to check current and previous, just in case
    // this is called during an interrupt
    if (Resources::driveBusData().driveState == DriveState::DS_NEUTRAL ||
        Resources::prevDriveBusData().driveState == DriveState::DS_NEUTRAL)
    {
        // we must be transitioining neutral -> on
        // Serial.println("Playing ready to drive!");
        Resources::instance().soundDriver.playSong();
    }
}