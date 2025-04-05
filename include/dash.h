#include <CAN.h>

#include <map>
#include <string>

#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "define.h"
#include "virtualTimer.h"

// VirtualTimerGroup read_timer;

#define DEFAULT_VALUE -100
#define DEFAULT_VALUE_UNSIGNED 255

class Dash {
   public:
    struct BarData {
        std::string displayName;
        float min;
        float max;
        float value;
        int x;
        int y;
        int height;
        int maxWidth;

        BarData() : displayName(""), min(0), max(0), value(0), x(0), y(0), height(20), maxWidth(100) {}

        BarData(std::string displayName, float min, float max, int startX, int startY, int height = 20, int maxWidth = 100)
            : displayName(displayName), min(min), max(max), value(min), x(startX), y(startY), height(height), maxWidth(maxWidth) {}
    };

    enum Error {
        NO_ERROR,
        BMS_FAULT,
        IMD_FAULT
    };

    enum Direction {
        LEFT_TO_RIGHT,
        UP_TO_DOWN
    };

    char mode;
    uint16_t index;
    int error_banner;

    void GetCAN();
    void Initialize();
    void UpdateDisplay(Adafruit_RA8875 tft);

    void DrawBackground(Adafruit_RA8875 tft, int16_t color = RUSSIAN_VIOLET);
    void DrawBar(Adafruit_RA8875 tft, std::string barName, float newValue, int16_t barColor, int16_t backgroundColor);
    float WheelSpeedAvg(float fl_wheel_speed, float fr_wheel_speed);
    void DrawDriveState(Adafruit_RA8875 tft, int startX, int startY, uint8_t curr_drive_state, int squareSize, float wheel_speed, int wheel_speed_startX, int wheel_speed_startY);
    void DrawMotorState(Adafruit_RA8875 tft, int startX, int startY, int motor_temp, int squareSize);
    void DrawInvCurState(Adafruit_RA8875 tft, int startX, int startY, int curr_accum_state, int squareSize);
    void DrawMinVoltState(Adafruit_RA8875 tft, int startX, int startY, int curr_minVolt_state, int squareSize);
    void DrawBatteryVoltState(Adafruit_RA8875 tft, int startX, int startY, int curr_batteryVolt_state, int squareSize);
    void DrawCoolantTemp(Adafruit_RA8875 tft, float coolant_temp, int startX, int startY);
    void DrawMaxCellTemp(Adafruit_RA8875 tft, float max_cell_temp, int startX, int startY);
    void DrawInverterTemp(Adafruit_RA8875 tft, int inverter_temp, int startX, int startY);
    void DrawIMDStatus(Adafruit_RA8875 tft, int startX, int startY, int imd_status, int squareSize);
    void HandleError(Adafruit_RA8875 tft, std::string error_message, int startX, int startY, Error type);
    void DrawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir = LEFT_TO_RIGHT);
    void HandleBMSFaults(Adafruit_RA8875 tft, int startX, int startY);

   private:
    TeensyCAN<3> g_can_bus{};
    VirtualTimerGroup timer_group{};

    // Coolant Signals
    CANSignal<float, 0, 16, CANTemplateConvertFloat(1), CANTemplateConvertFloat(-40), false> coolant_temp_signal;
    CANSignal<float, 16, 16, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> coolant_flow_signal;
    CANRXMessage<2> rx_coolant_state{g_can_bus, 0x420, coolant_temp_signal, coolant_flow_signal};

    // Wheel Speed Signals
    CANSignal<float, 16, 16, CANTemplateConvertFloat(0.01), CANTemplateConvertFloat(0), true> fl_wheel_speed_signal;
    CANSignal<float, 0, 16, CANTemplateConvertFloat(0.01), CANTemplateConvertFloat(0), true> fr_wheel_speed_signal;
    CANRXMessage<2> rx_wheel_speeds{g_can_bus, 0x411, fl_wheel_speed_signal, fr_wheel_speed_signal};

    // Inverter Temp Signals
    CANSignal<uint16_t, 0, 16, CANTemplateConvertFloat(0.1), CANTemplateConvertFloat(0.0), false> inverter_temp_status_igbt_temp{};
    CANSignal<uint16_t, 16, 16, CANTemplateConvertFloat(0.1), CANTemplateConvertFloat(0.0), false> inverter_temp_status_motor_temp{};
    CANRXMessage<2> rx_inverter_temp_status{g_can_bus, 0x282, inverter_temp_status_igbt_temp, inverter_temp_status_motor_temp};

    // Inverter Current Signals
    CANSignal<uint32_t, 0, 32, CANTemplateConvertFloat(0.0001), CANTemplateConvertFloat(0.0), false> inverter_current_draw_ah_drawn{};
    CANSignal<uint32_t, 32, 32, CANTemplateConvertFloat(0.0001), CANTemplateConvertFloat(0.0), false> inverter_current_draw_ah_charged{};
    CANTXMessage<2> rx_inverter_current_draw{g_can_bus, 0x283, 8, 100, inverter_current_draw_ah_drawn, inverter_current_draw_ah_charged};

    // ECU Signals
    CANSignal<uint8_t, 0, 8, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> drive_state_signal;
    CANRXMessage<1> rx_drive_state{g_can_bus, 0x206, drive_state_signal};

    // BMS Signals
    // Fault signals
    CANSignal<bool, 0, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_summary_signal;
    CANSignal<bool, 1, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_under_voltage_signal;
    CANSignal<bool, 2, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_over_voltage_signal;
    CANSignal<bool, 3, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_under_temperature_signal;
    CANSignal<bool, 4, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_over_temperature_signal;
    CANSignal<bool, 5, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_over_current_signal;
    CANSignal<bool, 6, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_external_kill_signal;
    CANSignal<bool, 7, 1, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_fault_open_wire_signal;
    CANRXMessage<8> rx_bms_faults{
        g_can_bus, 0x151, [this]() {
            RecordBMSFaults();
        },
        bms_fault_summary_signal, bms_fault_under_voltage_signal, bms_fault_over_voltage_signal, bms_fault_under_temperature_signal, bms_fault_over_temperature_signal, bms_fault_over_current_signal, bms_fault_external_kill_signal, bms_fault_open_wire_signal};

    // BMS Status Signals
    CANSignal<uint8_t, 0, 8, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> bms_state_signal;
    CANSignal<uint8_t, 8, 8, CANTemplateConvertFloat(1), CANTemplateConvertFloat(0), false> imd_status_signal;
    CANSignal<float, 16, 8, CANTemplateConvertFloat(1), CANTemplateConvertFloat(-40), false> bms_max_cell_temp_signal;
    CANSignal<float, 24, 8, CANTemplateConvertFloat(1), CANTemplateConvertFloat(-40), false> bms_min_cell_temp_signal;
    CANSignal<float, 32, 8, CANTemplateConvertFloat(0.012), CANTemplateConvertFloat(2), false> bms_max_cell_voltage_signal;
    CANSignal<float, 40, 8, CANTemplateConvertFloat(0.012), CANTemplateConvertFloat(2), false> bms_min_cell_voltage_signal;
    CANSignal<float, 48, 8, CANTemplateConvertFloat(0.5), CANTemplateConvertFloat(0), false> bms_soc_signal;
    CANRXMessage<7> rx_bms_status{
        g_can_bus, 0x152, bms_state_signal, imd_status_signal, bms_max_cell_temp_signal, bms_min_cell_temp_signal, bms_max_cell_voltage_signal, bms_min_cell_voltage_signal, bms_soc_signal};

    // BMS SOE Signals
    CANSignal<float, 0, 12, CANTemplateConvertFloat(0.1), CANTemplateConvertFloat(0), false> bms_max_discharge_signal;
    CANSignal<float, 12, 12, CANTemplateConvertFloat(0.1), CANTemplateConvertFloat(0), false> bms_max_regen_current_signal;
    CANSignal<float, 24, 16, CANTemplateConvertFloat(0.01), CANTemplateConvertFloat(0), false> bms_battery_voltage_signal;
    CANSignal<float, 40, 8, CANTemplateConvertFloat(1), CANTemplateConvertFloat(-40), false> bms_battery_temperature_signal;
    CANSignal<float, 48, 16, CANTemplateConvertFloat(0.01), CANTemplateConvertFloat(0), false> bms_battery_current_signal;
    CANRXMessage<5> rx_bms_soe{
        g_can_bus, 0x150, bms_max_discharge_signal, bms_max_regen_current_signal, bms_battery_voltage_signal, bms_battery_temperature_signal, bms_battery_current_signal};

    // PDM Bat Volt Signals
    CANSignal<float, 0, 16, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> pdm_bat_volt;
    CANSignal<bool, 16, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0.0), false> pdm_bat_volt_warning;
    CANRXMessage<2> can_bus_pdm_bat_volt{
        g_can_bus, 0x291, pdm_bat_volt, pdm_bat_volt_warning};

    // ECU Implausibility
    CANSignal<bool, 0, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_present;
    CANSignal<bool, 8, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_appss_disagreement_imp;
    CANSignal<bool, 16, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_bppc_imp;
    CANSignal<bool, 24, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_brake_invalid_imp;
    CANSignal<bool, 32, 8, CANTemplateConvertFloat(1.0), CANTemplateConvertFloat(0), false> ecu_implausibility_appss_invalid_imp;

    CANRXMessage<5> can_bus_ecu_implausibility{
        g_can_bus, 0x204, ecu_implausibility_present, ecu_implausibility_appss_disagreement_imp, 
        ecu_implausibility_bppc_imp, ecu_implausibility_brake_invalid_imp, ecu_implausibility_appss_invalid_imp};

    uint8_t prev_drive_state = DEFAULT_VALUE_UNSIGNED;
    float prev_wheel_speed = DEFAULT_VALUE;
    float prev_motor_temp = DEFAULT_VALUE;
    float prev_inverter_current_drawn = DEFAULT_VALUE;
    float prev_min_volt = DEFAULT_VALUE;
    float prev_bat_volt = DEFAULT_VALUE;
    float prev_fr_wheel_speed = DEFAULT_VALUE;
    float prev_coolant_temp = DEFAULT_VALUE;
    float prev_max_cell_temp = DEFAULT_VALUE;
    float prev_inverter_temp = DEFAULT_VALUE;
    Error error = NO_ERROR;
    uint8_t bms_faults = 0;
    uint8_t prev_bms_faults = 0;
    int16_t backgroundColor = NORTHWESTERN_PURPLE;
    std::map<std::string, BarData> bars;

    int CalcBarHeight(float value, float min, float max, int maxHeight);
    int CalcBarWidth(float value, float min, float max, int maxWidth);
    void RecordBMSFaults();
};