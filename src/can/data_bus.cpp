#include "can/data_bus.h"

#include "define.h"

// Helper: Generate a random float between min and max.
static float randomFloat(float min, float max) {
    // Generate a value between 0 and 9999, then scale
    long r = random(0, 10000);
    return min + (max - min) * (r / 10000.0);
}

const DataBusData& DataBus::getData() const {
    return this->_data;
}

void DataBus::initialize() {
    _dataBus.Initialize(ICAN::BaudRate::kBaud500K);

    _dataBus.RegisterRXMessage(rx_bms_voltages_0);
    _dataBus.RegisterRXMessage(rx_bms_voltages_1);
    _dataBus.RegisterRXMessage(rx_bms_voltages_2);
    _dataBus.RegisterRXMessage(rx_bms_voltages_3);
    _dataBus.RegisterRXMessage(rx_bms_voltages_4);
    _dataBus.RegisterRXMessage(rx_bms_voltages_5);
    _dataBus.RegisterRXMessage(rx_bms_voltages_6);
    _dataBus.RegisterRXMessage(rx_bms_voltages_7);
    _dataBus.RegisterRXMessage(rx_bms_voltages_8);
    _dataBus.RegisterRXMessage(rx_bms_voltages_9);
    _dataBus.RegisterRXMessage(rx_bms_voltages_10);
    _dataBus.RegisterRXMessage(rx_bms_voltages_11);
    _dataBus.RegisterRXMessage(rx_bms_voltages_12);
    _dataBus.RegisterRXMessage(rx_bms_voltages_13);
    _dataBus.RegisterRXMessage(rx_bms_voltages_14);
    _dataBus.RegisterRXMessage(rx_bms_voltages_15);
    _dataBus.RegisterRXMessage(rx_bms_voltages_16);
    _dataBus.RegisterRXMessage(rx_bms_voltages_17);
    _dataBus.RegisterRXMessage(rx_bms_voltages_18);
    _dataBus.RegisterRXMessage(rx_bms_voltages_19);

    _dataBus.RegisterRXMessage(rx_bms_temps_0);
    _dataBus.RegisterRXMessage(rx_bms_temps_1);
    _dataBus.RegisterRXMessage(rx_bms_temps_2);
    _dataBus.RegisterRXMessage(rx_bms_temps_3);
    _dataBus.RegisterRXMessage(rx_bms_temps_4);
    _dataBus.RegisterRXMessage(rx_bms_temps_5);
    _dataBus.RegisterRXMessage(rx_bms_temps_6);
    _dataBus.RegisterRXMessage(rx_bms_temps_7);
    _dataBus.RegisterRXMessage(rx_bms_temps_8);
    _dataBus.RegisterRXMessage(rx_bms_temps_9);
}

void DataBus::update() {
#ifndef DATA_DEBUG
    this->_dataBus.Tick();
    this->_data.cellVoltages[0] = cell_v_0;
    this->_data.cellVoltages[1] = cell_v_1;
    this->_data.cellVoltages[2] = cell_v_2;
    this->_data.cellVoltages[3] = cell_v_3;
    this->_data.cellVoltages[4] = cell_v_4;
    this->_data.cellVoltages[5] = cell_v_5;
    this->_data.cellVoltages[6] = cell_v_6;
    this->_data.cellVoltages[7] = cell_v_7;
    this->_data.cellVoltages[8] = cell_v_8;
    this->_data.cellVoltages[9] = cell_v_9;
    this->_data.cellVoltages[10] = cell_v_10;
    this->_data.cellVoltages[11] = cell_v_11;
    this->_data.cellVoltages[12] = cell_v_12;
    this->_data.cellVoltages[13] = cell_v_13;
    this->_data.cellVoltages[14] = cell_v_14;
    this->_data.cellVoltages[15] = cell_v_15;
    this->_data.cellVoltages[16] = cell_v_16;
    this->_data.cellVoltages[17] = cell_v_17;
    this->_data.cellVoltages[18] = cell_v_18;
    this->_data.cellVoltages[19] = cell_v_19;
    this->_data.cellVoltages[20] = cell_v_20;
    this->_data.cellVoltages[21] = cell_v_21;
    this->_data.cellVoltages[22] = cell_v_22;
    this->_data.cellVoltages[23] = cell_v_23;
    this->_data.cellVoltages[24] = cell_v_24;
    this->_data.cellVoltages[25] = cell_v_25;
    this->_data.cellVoltages[26] = cell_v_26;
    this->_data.cellVoltages[27] = cell_v_27;
    this->_data.cellVoltages[28] = cell_v_28;
    this->_data.cellVoltages[29] = cell_v_29;
    this->_data.cellVoltages[30] = cell_v_30;
    this->_data.cellVoltages[31] = cell_v_31;
    this->_data.cellVoltages[32] = cell_v_32;
    this->_data.cellVoltages[33] = cell_v_33;
    this->_data.cellVoltages[34] = cell_v_34;
    this->_data.cellVoltages[35] = cell_v_35;
    this->_data.cellVoltages[36] = cell_v_36;
    this->_data.cellVoltages[37] = cell_v_37;
    this->_data.cellVoltages[38] = cell_v_38;
    this->_data.cellVoltages[39] = cell_v_39;
    this->_data.cellVoltages[40] = cell_v_40;
    this->_data.cellVoltages[41] = cell_v_41;
    this->_data.cellVoltages[42] = cell_v_42;
    this->_data.cellVoltages[43] = cell_v_43;
    this->_data.cellVoltages[44] = cell_v_44;
    this->_data.cellVoltages[45] = cell_v_45;
    this->_data.cellVoltages[46] = cell_v_46;
    this->_data.cellVoltages[47] = cell_v_47;
    this->_data.cellVoltages[48] = cell_v_48;
    this->_data.cellVoltages[49] = cell_v_49;
    this->_data.cellVoltages[50] = cell_v_50;
    this->_data.cellVoltages[51] = cell_v_51;
    this->_data.cellVoltages[52] = cell_v_52;
    this->_data.cellVoltages[53] = cell_v_53;
    this->_data.cellVoltages[54] = cell_v_54;
    this->_data.cellVoltages[55] = cell_v_55;
    this->_data.cellVoltages[56] = cell_v_56;
    this->_data.cellVoltages[57] = cell_v_57;
    this->_data.cellVoltages[58] = cell_v_58;
    this->_data.cellVoltages[59] = cell_v_59;
    this->_data.cellVoltages[60] = cell_v_60;
    this->_data.cellVoltages[61] = cell_v_61;
    this->_data.cellVoltages[62] = cell_v_62;
    this->_data.cellVoltages[63] = cell_v_63;
    this->_data.cellVoltages[64] = cell_v_64;
    this->_data.cellVoltages[65] = cell_v_65;
    this->_data.cellVoltages[66] = cell_v_66;
    this->_data.cellVoltages[67] = cell_v_67;
    this->_data.cellVoltages[68] = cell_v_68;
    this->_data.cellVoltages[69] = cell_v_69;
    this->_data.cellVoltages[70] = cell_v_70;
    this->_data.cellVoltages[71] = cell_v_71;
    this->_data.cellVoltages[72] = cell_v_72;
    this->_data.cellVoltages[73] = cell_v_73;
    this->_data.cellVoltages[74] = cell_v_74;
    this->_data.cellVoltages[75] = cell_v_75;
    this->_data.cellVoltages[76] = cell_v_76;
    this->_data.cellVoltages[77] = cell_v_77;
    this->_data.cellVoltages[78] = cell_v_78;
    this->_data.cellVoltages[79] = cell_v_79;
    this->_data.cellVoltages[80] = cell_v_80;
    this->_data.cellVoltages[81] = cell_v_81;
    this->_data.cellVoltages[82] = cell_v_82;
    this->_data.cellVoltages[83] = cell_v_83;
    this->_data.cellVoltages[84] = cell_v_84;
    this->_data.cellVoltages[85] = cell_v_85;
    this->_data.cellVoltages[86] = cell_v_86;
    this->_data.cellVoltages[87] = cell_v_87;
    this->_data.cellVoltages[88] = cell_v_88;
    this->_data.cellVoltages[89] = cell_v_89;
    this->_data.cellVoltages[90] = cell_v_90;
    this->_data.cellVoltages[91] = cell_v_91;
    this->_data.cellVoltages[92] = cell_v_92;
    this->_data.cellVoltages[93] = cell_v_93;
    this->_data.cellVoltages[94] = cell_v_94;
    this->_data.cellVoltages[95] = cell_v_95;
    this->_data.cellVoltages[96] = cell_v_96;
    this->_data.cellVoltages[97] = cell_v_97;
    this->_data.cellVoltages[98] = cell_v_98;
    this->_data.cellVoltages[99] = cell_v_99;
    this->_data.cellVoltages[100] = cell_v_100;
    this->_data.cellVoltages[101] = cell_v_101;
    this->_data.cellVoltages[102] = cell_v_102;
    this->_data.cellVoltages[103] = cell_v_103;
    this->_data.cellVoltages[104] = cell_v_104;
    this->_data.cellVoltages[105] = cell_v_105;
    this->_data.cellVoltages[106] = cell_v_106;
    this->_data.cellVoltages[107] = cell_v_107;
    this->_data.cellVoltages[108] = cell_v_108;
    this->_data.cellVoltages[109] = cell_v_109;
    this->_data.cellVoltages[110] = cell_v_110;
    this->_data.cellVoltages[111] = cell_v_111;
    this->_data.cellVoltages[112] = cell_v_112;
    this->_data.cellVoltages[113] = cell_v_113;
    this->_data.cellVoltages[114] = cell_v_114;
    this->_data.cellVoltages[115] = cell_v_115;
    this->_data.cellVoltages[116] = cell_v_116;
    this->_data.cellVoltages[117] = cell_v_117;
    this->_data.cellVoltages[118] = cell_v_118;
    this->_data.cellVoltages[119] = cell_v_119;
    this->_data.cellVoltages[120] = cell_v_120;
    this->_data.cellVoltages[121] = cell_v_121;
    this->_data.cellVoltages[122] = cell_v_122;
    this->_data.cellVoltages[123] = cell_v_123;
    this->_data.cellVoltages[124] = cell_v_124;
    this->_data.cellVoltages[125] = cell_v_125;
    this->_data.cellVoltages[126] = cell_v_126;
    this->_data.cellVoltages[127] = cell_v_127;
    this->_data.cellVoltages[128] = cell_v_128;
    this->_data.cellVoltages[129] = cell_v_129;
    this->_data.cellVoltages[130] = cell_v_130;
    this->_data.cellVoltages[131] = cell_v_131;
    this->_data.cellVoltages[132] = cell_v_132;
    this->_data.cellVoltages[133] = cell_v_133;
    this->_data.cellVoltages[134] = cell_v_134;
    this->_data.cellVoltages[135] = cell_v_135;
    this->_data.cellVoltages[136] = cell_v_136;
    this->_data.cellVoltages[137] = cell_v_137;
    this->_data.cellVoltages[138] = cell_v_138;
    this->_data.cellVoltages[139] = cell_v_139;

    this->_data.cellTemperatures[0] = cell_t_0;
    this->_data.cellTemperatures[1] = cell_t_1;
    this->_data.cellTemperatures[2] = cell_t_2;
    this->_data.cellTemperatures[3] = cell_t_3;
    this->_data.cellTemperatures[4] = cell_t_4;
    this->_data.cellTemperatures[5] = cell_t_5;
    this->_data.cellTemperatures[6] = cell_t_6;
    this->_data.cellTemperatures[7] = cell_t_7;
    this->_data.cellTemperatures[8] = cell_t_8;
    this->_data.cellTemperatures[9] = cell_t_9;
    this->_data.cellTemperatures[10] = cell_t_10;
    this->_data.cellTemperatures[11] = cell_t_11;
    this->_data.cellTemperatures[12] = cell_t_12;
    this->_data.cellTemperatures[13] = cell_t_13;
    this->_data.cellTemperatures[14] = cell_t_14;
    this->_data.cellTemperatures[15] = cell_t_15;
    this->_data.cellTemperatures[16] = cell_t_16;
    this->_data.cellTemperatures[17] = cell_t_17;
    this->_data.cellTemperatures[18] = cell_t_18;
    this->_data.cellTemperatures[19] = cell_t_19;
    this->_data.cellTemperatures[20] = cell_t_20;
    this->_data.cellTemperatures[21] = cell_t_21;
    this->_data.cellTemperatures[22] = cell_t_22;
    this->_data.cellTemperatures[23] = cell_t_23;
    this->_data.cellTemperatures[24] = cell_t_24;
    this->_data.cellTemperatures[25] = cell_t_25;
    this->_data.cellTemperatures[26] = cell_t_26;
    this->_data.cellTemperatures[27] = cell_t_27;
    this->_data.cellTemperatures[28] = cell_t_28;
    this->_data.cellTemperatures[29] = cell_t_29;
    this->_data.cellTemperatures[30] = cell_t_30;
    this->_data.cellTemperatures[31] = cell_t_31;
    this->_data.cellTemperatures[32] = cell_t_32;
    this->_data.cellTemperatures[33] = cell_t_33;
    this->_data.cellTemperatures[34] = cell_t_34;
    this->_data.cellTemperatures[35] = cell_t_35;
    this->_data.cellTemperatures[36] = cell_t_36;
    this->_data.cellTemperatures[37] = cell_t_37;
    this->_data.cellTemperatures[38] = cell_t_38;
    this->_data.cellTemperatures[39] = cell_t_39;
    this->_data.cellTemperatures[40] = cell_t_40;
    this->_data.cellTemperatures[41] = cell_t_41;
    this->_data.cellTemperatures[42] = cell_t_42;
    this->_data.cellTemperatures[43] = cell_t_43;
    this->_data.cellTemperatures[44] = cell_t_44;
    this->_data.cellTemperatures[45] = cell_t_45;
    this->_data.cellTemperatures[46] = cell_t_46;
    this->_data.cellTemperatures[47] = cell_t_47;
    this->_data.cellTemperatures[48] = cell_t_48;
    this->_data.cellTemperatures[49] = cell_t_49;
    this->_data.cellTemperatures[50] = cell_t_50;
    this->_data.cellTemperatures[51] = cell_t_51;
    this->_data.cellTemperatures[52] = cell_t_52;
    this->_data.cellTemperatures[53] = cell_t_53;
    this->_data.cellTemperatures[54] = cell_t_54;
    this->_data.cellTemperatures[55] = cell_t_55;
    this->_data.cellTemperatures[56] = cell_t_56;
    this->_data.cellTemperatures[57] = cell_t_57;
    this->_data.cellTemperatures[58] = cell_t_58;
    this->_data.cellTemperatures[59] = cell_t_59;
    this->_data.cellTemperatures[60] = cell_t_60;
    this->_data.cellTemperatures[61] = cell_t_61;
    this->_data.cellTemperatures[62] = cell_t_62;
    this->_data.cellTemperatures[63] = cell_t_63;
    this->_data.cellTemperatures[64] = cell_t_64;
    this->_data.cellTemperatures[65] = cell_t_65;
    this->_data.cellTemperatures[66] = cell_t_66;
    this->_data.cellTemperatures[67] = cell_t_67;
    this->_data.cellTemperatures[68] = cell_t_68;
    this->_data.cellTemperatures[69] = cell_t_69;
    this->_data.cellTemperatures[70] = cell_t_70;
    this->_data.cellTemperatures[71] = cell_t_71;
    this->_data.cellTemperatures[72] = cell_t_72;
    this->_data.cellTemperatures[73] = cell_t_73;
    this->_data.cellTemperatures[74] = cell_t_74;
    this->_data.cellTemperatures[75] = cell_t_75;
    this->_data.cellTemperatures[76] = cell_t_76;
    this->_data.cellTemperatures[77] = cell_t_77;
    this->_data.cellTemperatures[78] = cell_t_78;
    this->_data.cellTemperatures[79] = cell_t_79;

#else
    for (size_t i = 0; i < NUM_TEMP_CELLS; i++) {
        this->_data.cellTemperatures[i] = randomFloat(0, 100);
    }

    for (size_t i = 0; i < NUM_VOLT_CELLS; i++) {
        this->_data.cellVoltages[i] = randomFloat(2, 6);
    }

#endif
}