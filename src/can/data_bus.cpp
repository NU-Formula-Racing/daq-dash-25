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

#else
    for (size_t i = 0; i < NUM_TEMP_CELLS; i++) {
        this->_data.cellTemperatures[i] = randomFloat(0, 100);
    }

    for (size_t i = 0; i < NUM_VOLT_CELLS; i++) {
        this->_data.cellVoltages[i] = randomFloat(2, 6);
    }

#endif
}