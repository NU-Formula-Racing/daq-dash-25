#ifndef __DATA_BUS_H__
#define __DATA_BUS_H__

#include <Arduino.h>
#include <CAN.h>

#include <string>

#include "virtualTimer.h"

#define NUM_TEMP_CELLS 80
#define NUM_VOLT_CELLS 140


//try
struct DataBusData {
    float cellTemperatures[NUM_TEMP_CELLS];
    float cellVoltages[NUM_VOLT_CELLS];
};

class DataBus {
   public:
    const DataBusData &getData() const;

    void initialize();
    void update();

   private:
    DataBusData _data;
    TeensyCAN<2> _dataBus;
    VirtualTimerGroup timer_group{};

    // MakeSignedCANSignal(float, 0, 16, 0.04432234432, 0) gen_amps_signal;
    // MakeSignedCANSignal(float, 16, 16, 0.04432234432, 0) fan_amps_signal;
    // MakeSignedCANSignal(float, 32, 16, 0.04432234432, 0) pump_amps_signal;
    // CANRXMessage<3> rx_pdm_current{_dataBus, 0x290, gen_amps_signal, fan_amps_signal, pump_amps_signal};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_0;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_1;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_2;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_3;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_4;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_5;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_6;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_0;
    CANRXMessage<8> rx_bms_voltages_0{_dataBus, 0x153, cell_v_0, cell_v_1, cell_v_2, cell_v_3, cell_v_4, cell_v_5, cell_v_6, cell_ocv_offset_0};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_7;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_8;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_9;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_10;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_11;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_12;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_13;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_1;
    CANRXMessage<8> rx_bms_voltages_1{_dataBus, 0x154, cell_v_7, cell_v_8, cell_v_9, cell_v_10, cell_v_11, cell_v_12, cell_v_13, cell_ocv_offset_1};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_14;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_15;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_16;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_17;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_18;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_19;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_20;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_2;
    CANRXMessage<8> rx_bms_voltages_2{_dataBus, 0x155, cell_v_14, cell_v_15, cell_v_16, cell_v_17, cell_v_18, cell_v_19, cell_v_20, cell_ocv_offset_2};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_21;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_22;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_23;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_24;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_25;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_26;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_27;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_3;
    CANRXMessage<8> rx_bms_voltages_3{_dataBus, 0x156, cell_v_21, cell_v_22, cell_v_23, cell_v_24, cell_v_25, cell_v_26, cell_v_27, cell_ocv_offset_3};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_28;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_29;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_30;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_31;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_32;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_33;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_34;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_4;
    CANRXMessage<8> rx_bms_voltages_4{_dataBus, 0x157, cell_v_28, cell_v_29, cell_v_30, cell_v_31, cell_v_32, cell_v_33, cell_v_34, cell_ocv_offset_4};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_35;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_36;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_37;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_38;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_39;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_40;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_41;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_5;
    CANRXMessage<8> rx_bms_voltages_5{_dataBus, 0x158, cell_v_35, cell_v_36, cell_v_37, cell_v_38, cell_v_39, cell_v_40, cell_v_41, cell_ocv_offset_5};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_42;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_43;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_44;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_45;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_46;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_47;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_48;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_6;
    CANRXMessage<8> rx_bms_voltages_6{_dataBus, 0x159, cell_v_42, cell_v_43, cell_v_44, cell_v_45, cell_v_46, cell_v_47, cell_v_48, cell_ocv_offset_6};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_49;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_50;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_51;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_52;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_53;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_54;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_55;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_7;
    CANRXMessage<8> rx_bms_voltages_7{_dataBus, 0x15A, cell_v_49, cell_v_50, cell_v_51, cell_v_52, cell_v_53, cell_v_54, cell_v_55, cell_ocv_offset_7};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_56;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_57;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_58;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_59;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_60;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_61;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_62;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_8;
    CANRXMessage<8> rx_bms_voltages_8{_dataBus, 0x15B, cell_v_56, cell_v_57, cell_v_58, cell_v_59, cell_v_60, cell_v_61, cell_v_62, cell_ocv_offset_8};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_63;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_64;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_65;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_66;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_67;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_68;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_69;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_9;
    CANRXMessage<8> rx_bms_voltages_9{_dataBus, 0x15C, cell_v_63, cell_v_64, cell_v_65, cell_v_66, cell_v_67, cell_v_68, cell_v_69, cell_ocv_offset_9};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_70;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_71;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_72;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_73;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_74;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_75;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_76;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_10;
    CANRXMessage<8> rx_bms_voltages_10{_dataBus, 0x15D, cell_v_70, cell_v_71, cell_v_72, cell_v_73, cell_v_74, cell_v_75, cell_v_76, cell_ocv_offset_10};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_77;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_78;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_79;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_80;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_81;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_82;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_83;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_11;
    CANRXMessage<8> rx_bms_voltages_11{_dataBus, 0x15E, cell_v_77, cell_v_78, cell_v_79, cell_v_80, cell_v_81, cell_v_82, cell_v_83, cell_ocv_offset_11};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_84;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_85;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_86;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_87;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_88;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_89;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_90;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_12;
    CANRXMessage<8> rx_bms_voltages_12{_dataBus, 0x15F, cell_v_84, cell_v_85, cell_v_86, cell_v_87, cell_v_88, cell_v_89, cell_v_90, cell_ocv_offset_12};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_91;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_92;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_93;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_94;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_95;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_96;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_97;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_13;
    CANRXMessage<8> rx_bms_voltages_13{_dataBus, 0x160, cell_v_91, cell_v_92, cell_v_93, cell_v_94, cell_v_95, cell_v_96, cell_v_97, cell_ocv_offset_13};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_98;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_99;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_100;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_101;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_102;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_103;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_104;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_14;
    CANRXMessage<8> rx_bms_voltages_14{_dataBus, 0x161, cell_v_98, cell_v_99, cell_v_100, cell_v_101, cell_v_102, cell_v_103, cell_v_104, cell_ocv_offset_14};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_105;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_106;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_107;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_108;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_109;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_110;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_111;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_15;
    CANRXMessage<8> rx_bms_voltages_15{_dataBus, 0x162, cell_v_105, cell_v_106, cell_v_107, cell_v_108, cell_v_109, cell_v_110, cell_v_111, cell_ocv_offset_15};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_112;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_113;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_114;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_115;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_116;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_117;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_118;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_16;
    CANRXMessage<8> rx_bms_voltages_16{_dataBus, 0x163, cell_v_112, cell_v_113, cell_v_114, cell_v_115, cell_v_116, cell_v_117, cell_v_118, cell_ocv_offset_16};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_119;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_120;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_121;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_122;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_123;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_124;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_125;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_17;
    CANRXMessage<8> rx_bms_voltages_17{_dataBus, 0x164, cell_v_119, cell_v_120, cell_v_121, cell_v_122, cell_v_123, cell_v_124, cell_v_125, cell_ocv_offset_17};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_126;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_127;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_128;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_129;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_130;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_131;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_132;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_18;
    CANRXMessage<8> rx_bms_voltages_18{_dataBus, 0x165, cell_v_126, cell_v_127, cell_v_128, cell_v_129, cell_v_130, cell_v_131, cell_v_132, cell_ocv_offset_18};

    MakeSignedCANSignal(float, 0, 8, 0.012, 2) cell_v_133;
    MakeSignedCANSignal(float, 8, 8, 0.012, 2) cell_v_134;
    MakeSignedCANSignal(float, 16, 8, 0.012, 2) cell_v_135;
    MakeSignedCANSignal(float, 24, 8, 0.012, 2) cell_v_136;
    MakeSignedCANSignal(float, 32, 8, 0.012, 2) cell_v_137;
    MakeSignedCANSignal(float, 40, 8, 0.012, 2) cell_v_138;
    MakeSignedCANSignal(float, 48, 8, 0.012, 2) cell_v_139;
    MakeSignedCANSignal(float, 56, 8, 0.004, 0) cell_ocv_offset_19;
    CANRXMessage<8> rx_bms_voltages_19{_dataBus, 0x166, cell_v_133, cell_v_134, cell_v_135, cell_v_136, cell_v_137, cell_v_138, cell_v_139, cell_ocv_offset_19};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_0;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_1;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_2;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_3;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_4;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_5;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_6;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_7;
    CANRXMessage<8> rx_bms_temps_0{_dataBus, 0x167, cell_t_0, cell_t_1, cell_t_2, cell_t_3, cell_t_4, cell_t_5, cell_t_6, cell_t_7};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_8;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_9;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_10;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_11;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_12;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_13;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_14;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_15;
    CANRXMessage<8> rx_bms_temps_1{_dataBus, 0x168, cell_t_8, cell_t_9, cell_t_10, cell_t_11, cell_t_12, cell_t_13, cell_t_14, cell_t_15};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_16;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_17;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_18;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_19;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_20;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_21;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_22;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_23;
    CANRXMessage<8> rx_bms_temps_2{_dataBus, 0x169, cell_t_16, cell_t_17, cell_t_18, cell_t_19, cell_t_20, cell_t_21, cell_t_22, cell_t_23};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_24;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_25;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_26;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_27;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_28;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_29;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_30;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_31;
    CANRXMessage<8> rx_bms_temps_3{_dataBus, 0x16A, cell_t_24, cell_t_25, cell_t_26, cell_t_27, cell_t_28, cell_t_29, cell_t_30, cell_t_31};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_32;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_33;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_34;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_35;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_36;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_37;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_38;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_39;
    CANRXMessage<8> rx_bms_temps_4{_dataBus, 0x16B, cell_t_32, cell_t_33, cell_t_34, cell_t_35, cell_t_36, cell_t_37, cell_t_38, cell_t_39};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_40;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_41;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_42;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_43;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_44;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_45;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_46;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_47;
    CANRXMessage<8> rx_bms_temps_5{_dataBus, 0x16C, cell_t_40, cell_t_41, cell_t_42, cell_t_43, cell_t_44, cell_t_45, cell_t_46, cell_t_47};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_48;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_49;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_50;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_51;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_52;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_53;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_54;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_55;
    CANRXMessage<8> rx_bms_temps_6{_dataBus, 0x16D, cell_t_48, cell_t_49, cell_t_50, cell_t_51, cell_t_52, cell_t_53, cell_t_54, cell_t_55};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_56;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_57;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_58;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_59;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_60;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_61;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_62;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_63;
    CANRXMessage<8> rx_bms_temps_7{_dataBus, 0x16E, cell_t_56, cell_t_57, cell_t_58, cell_t_59, cell_t_60, cell_t_61, cell_t_62, cell_t_63};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_64;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_65;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_66;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_67;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_68;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_69;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_70;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_71;
    CANRXMessage<8> rx_bms_temps_8{_dataBus, 0x16F, cell_t_64, cell_t_65, cell_t_66, cell_t_67, cell_t_68, cell_t_69, cell_t_70, cell_t_71};

    MakeSignedCANSignal(float, 0, 8, 1, -40) cell_t_72;
    MakeSignedCANSignal(float, 8, 8, 1, -40) cell_t_73;
    MakeSignedCANSignal(float, 16, 8, 1, -40) cell_t_74;
    MakeSignedCANSignal(float, 24, 8, 1, -40) cell_t_75;
    MakeSignedCANSignal(float, 32, 8, 1, -40) cell_t_76;
    MakeSignedCANSignal(float, 40, 8, 1, -40) cell_t_77;
    MakeSignedCANSignal(float, 48, 8, 1, -40) cell_t_78;
    MakeSignedCANSignal(float, 56, 8, 1, -40) cell_t_79;
    CANRXMessage<8> rx_bms_temps_9{_dataBus, 0x170, cell_t_72, cell_t_73, cell_t_74, cell_t_75, cell_t_76, cell_t_77, cell_t_78, cell_t_79};

};

#endif  // __DATA_BUS_H__