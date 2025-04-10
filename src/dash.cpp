#include "dash.h"

#include <bitset>
#include <cmath>
#include <iostream>
#include <map>
#include <string>

#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "CAN.h"
#include "SPI.h"
#include "virtualTimer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define CENTER SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2
#define CENTER_OFFSET(x, y) SCREEN_WIDTH / 2 + x, SCREEN_HEIGHT / 2 + y
#define BAND_HEIGHT 30
#define BAR_HEIGHT SCREEN_HEIGHT - BAND_HEIGHT * 2
#define BAR_WIDTH 50
#define BAR_SPACING 15
#define MASK(x) (1 << x)

#define FORCE_DRAW false
// #define DEBUG false

int drive_state_startX = SCREEN_WIDTH / 4;
int drive_state_startY = SCREEN_HEIGHT / 3;
int hv_bat_volt_startX = SCREEN_WIDTH / 8;
int hv_bat_volt_startY = SCREEN_HEIGHT / 4;
int lv_bat_volt_startX = SCREEN_WIDTH / 8;
int lv_bat_volt_startY = SCREEN_HEIGHT * 3 / 4;
int wheel_speed_startX = SCREEN_WIDTH / 2 + 40;
int wheel_speed_startY = SCREEN_HEIGHT * 0.34;
int max_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
int max_cell_temp_startY = SCREEN_HEIGHT / 4;
int min_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
int min_cell_temp_startY = SCREEN_HEIGHT * 3 / 4;

// for states, after mid state, goes to last state
int motor_temp_last_state = 70;
int motor_temp_mid_state = 30;
int inverter_current_last_state = 100;
int inverter_current_mid_state = 50;
int min_voltage_last_state = 2.7;
int min_voltage_mid_state = 3.2;
int hv_battery_voltage_last_state = 3.4;
int hv_battery_voltage_mid_state = 3.2;
int lv_battery_voltage_last_state = 3.5;
int lv_battery_voltage_mid_state = 3.1; // min 2.7
int max_cell_temp_last_state = 2;
int max_cell_temp_mid_state = 1;
int min_cell_temp_last_state = 3;
int min_cell_temp_mid_state = 2;

int bar_max_size = 480;

float motor_temp;
int drive_state = -1;
bool drive_state_drawn = false;

bool ifBMSfault = false;
bool ifIMDfault = false;
bool ifECUfault = false;
bool ifInverterfault = false;
bool ifErrorScreen = false;

void Dash::GetCAN()
{
    g_can_bus.Tick();
}

void Dash::Initialize()
{
    digitalWrite(INDICATOR_LED, LOW);
    g_can_bus.Initialize(ICAN::BaudRate::kBaud500K);
    g_can_bus.RegisterRXMessage(rx_wheel_speeds);
    g_can_bus.RegisterRXMessage(rx_drive_state);
    // g_can_bus.RegisterRXMessage(rx_inverter_current_draw);
    g_can_bus.RegisterRXMessage(rx_bms_faults);
    g_can_bus.RegisterRXMessage(rx_bms_status);
    g_can_bus.RegisterRXMessage(rx_pdm_bat_volt);
    g_can_bus.RegisterRXMessage(rx_ecu_implausibility);

    // this->bars["coolant_temp"] = BarData("", 0, 100, SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT * 0.725, 15, SCREEN_WIDTH / 2 - 90);
    // this->bars["inverter_temp"] = BarData("", 0, 100, SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT * 0.825, 15, SCREEN_WIDTH / 2 - 90);
    // this->bars["motor_temp"] = BarData("", 0, 100, SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT * 0.925, 15, SCREEN_WIDTH / 2 - 90);
}

void Dash::DrawBackground(Adafruit_RA8875 tft, int16_t color)
{
    this->backgroundColor = color;
    // black out the screen
    tft.fillScreen(color);
    // draw outlines
    // MIDDLE DRIVE RECT
    tft.drawRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, RA8875_WHITE);
    // COOLANT, MAX CELL, INTERVERT TEMPS RECT
    tft.drawRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT * 2 / 3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, RA8875_WHITE);
    // MOTOR TEMP CIRC TOP LEFT
    tft.drawCircle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 8, RA8875_WHITE);
    // ACCUM TEMP CIRC BOTTOM LEFT
    tft.drawCircle(SCREEN_WIDTH / 8, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH / 8, RA8875_WHITE);
    // MIN VOLTAGE CIRC TOP RIGHT
    tft.drawCircle(SCREEN_WIDTH * 7 / 8, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 8, RA8875_WHITE);
    // BATTERY VOLTAGE CIRC BOTTOM RIGHT
    tft.drawCircle(SCREEN_WIDTH * 7 / 8, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH / 8, RA8875_WHITE);

    // write circle labels
    DrawString(tft, "HV Battery Voltage", hv_bat_volt_startX * 0.8, hv_bat_volt_startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    DrawString(tft, "LV Battery Voltage", lv_bat_volt_startX * 0.8, lv_bat_volt_startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    DrawString(tft, "Max Cell Temp", lv_bat_volt_startX * 0.8, lv_bat_volt_startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    DrawString(tft, "Min Cell Temp", lv_bat_volt_startX * 0.8, lv_bat_volt_startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    // write text beneath the bars
    // iterate
    for (auto &bar : this->bars)
    {
        BarData &data = bar.second;
        DrawString(tft, data.displayName, data.x, data.y + 10, 4, RA8875_WHITE, RA8875_BLACK);
    }

    if (this->error != NO_ERROR)
    {
        return;
    }
}

float Dash::WheelSpeedAvg(float fl_wheel_speed, float fr_wheel_speed)
{
    return (fl_wheel_speed + fr_wheel_speed) / 2;
}

void Dash::UpdateDisplay(Adafruit_RA8875 tft)
{
#ifndef DEBUG
    float fl_wheel_speed = static_cast<float>(fl_wheel_speed_signal);
    float fr_wheel_speed = static_cast<float>(fr_wheel_speed_signal);
    uint8_t curr_drive_state = static_cast<uint8_t>(drive_state_signal);
    int imd_status = static_cast<int>(imd_status_signal);
    float max_cell_temp = static_cast<float>(bms_max_cell_temp_signal);
    float min_cell_temp = static_cast<float>(bms_min_cell_temp_signal);
    float hv_bat_volt = static_cast<float>(bms_battery_voltage_signal);
    float lv_bat_volt = static_cast<float>(pdm_bat_volt);

#else
    // we should change the drive state for testing
    // cycle based on time
    float fl_wheel_speed = (millis() / 200) % 200;             //
    float fr_wheel_speed = (millis() / 200) % 200;             //
    int curr_drive_state = (millis() / 1000) % 3;              //
    int imd_status = millis() > 5000 ? -10 : 0;                //
    this->bms_faults = millis() > 10000 ? 0b11111111 : 0;      //
    this->inverter_faults = millis() > 10000 ? 0b11111111 : 0; //

    float coolant_temp = (millis() / 100) % 100; //
    int inverter_temp = (millis() / 20) % 100;
    int inverter_current_drawn = (millis() / 20) % 100;
    float motor_temp = (millis() / 10) % 100;
    float battery_voltage = (millis() / 100) % 100; //
    float min_voltage = (millis() / 20) % 100;      //
    float max_cell_temp = (millis() / 10) % 100;    //

#endif
    float avg_wheel_speed = fl_wheel_speed + fr_wheel_speed / 2;
    if (this->prev_drive_state != curr_drive_state || FORCE_DRAW)
    {
        DrawDriveState(tft, drive_state_startX, drive_state_startY, curr_drive_state, 8, avg_wheel_speed, wheel_speed_startX, wheel_speed_startY, ifErrorScreen);
        this->prev_drive_state = curr_drive_state;
    }
    // this->prev_wheel_speed = avg_wheel_speed;
    if (this->prev_hv_bat_volt != hv_bat_volt || FORCE_DRAW)
    {
        DrawState(tft, hv_bat_volt_startX, hv_bat_volt_startY, hv_bat_volt, 8, hv_battery_voltage_mid_state, hv_battery_voltage_last_state);
        this->prev_hv_bat_volt = hv_bat_volt;
    }
    if (this->prev_lv_bat_volt != lv_bat_volt || FORCE_DRAW)
    {
        DrawState(tft, lv_bat_volt_startX, lv_bat_volt_startY, lv_bat_volt, 8, lv_battery_voltage_mid_state, lv_battery_voltage_last_state);
        this->prev_lv_bat_volt = lv_bat_volt;
    }
    if (this->prev_max_cell_temp != max_cell_temp || FORCE_DRAW)
    {
        DrawState(tft, max_cell_temp, max_cell_temp_startX, max_cell_temp_startY, 8, max_cell_temp_mid_state, max_cell_temp_last_state);
        this->prev_max_cell_temp = max_cell_temp;
    }
    if (this->prev_min_cell_temp != min_cell_temp || FORCE_DRAW)
    {
        DrawState(tft, min_cell_temp, min_cell_temp_startX, min_cell_temp_startY, 8, max_cell_temp_mid_state, max_cell_temp_last_state);
        this->prev_max_cell_temp = max_cell_temp;
    }

    // draw IMD status
    if (ifBMSfault == false & ifIMDfault == false)
    {
        digitalWrite(INDICATOR_LED, LOW);
    }
    // if (ifBMSfault == false & ifIMDfault == false & ifECUfault == false & ifInverterfault == false)
    // {
    //     tft.fillScreen(this->backgroundColor);
    // }
    DrawIMDStatus(tft, 8, 2, imd_status, 32);
    HandleBMSFaults(tft, 8, 2);
    HandleInverterFaults(tft, 8, 3);
    HandleECUFaults(tft, 8, 2);

    timer_group.Tick(millis());
}

// COME HERE NEXT TIME TO DRAW THE NUMBER IN THE MIDDLE. ALSO, CHANGE DRAWCHAR FOR OTHER DRAW STATE CIRCLES AND THE MIDDLE RECTANGLE BC FULL WORDS

// Draws drive state on screen based on CAN signal
// Draws drive state on screen based on CAN signal
void Dash::DrawDriveState(Adafruit_RA8875 tft, int startX, int startY, uint8_t curr_drive_state, int squareSize, float wheel_speed, int wheel_speed_startX, int wheel_speed_startY, bool ifErrorScreen)
{
    // dont need wheel speed start x y anymore i think
    int16_t color = INDIAN_RED;
    int driveRectw = startX;
    int driveRecth = startY * 2;
    int digit_spacing = 8;
    int char_width = 80;
    int draw_digit_size = 13;
    if (ifErrorScreen == false)
    {
        int driveRectw = startX;
        int driveRecth = startY * 2;
        int digit_spacing = 8;
        int char_width = 80;
        int draw_digit_size = 13;
    }
    else
    {
        int driveRectw = startX / 2;
        int driveRecth = startY;
        int digit_spacing = 4;
        int char_width = 40;
        int draw_digit_size = 6;
    }
    switch (curr_drive_state)
    {
    case 0:
        color = INDIAN_RED;
        break;
    case 1:
        color = GOLD;
        break;
    case 2:
        color = FERN_GREEN;
        break;
    default:
        color = INDIAN_RED;
        break;
    }
    tft.fillRect(startX, startY, driveRectw, driveRecth, color);
    // change sizes via if statement
    switch (curr_drive_state)
    {
    case 0:
        DrawString(tft, "OFF", startX * 4 * 0.4, startY * 3 * 0.58, 5, RA8875_WHITE, color);
        // DrawString(tft, "OFF", SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.58, 5, RA8875_WHITE, color);
        break;
    case 1:
        DrawString(tft, "NEUTRAL", startX * 4 * 0.47, startY * 3 * 0.58, 5, RA8875_BLACK, color);
        // DrawString(tft, "NEUTRAL", SCREEN_WIDTH * 0.47, SCREEN_HEIGHT * 0.58, 5, RA8875_BLACK, color);
        break;
    case 2:
        DrawString(tft, "DRIVE", startX * 4 * 0.45, startY * 3 * 0.58, 5, RA8875_WHITE, color);
        break;
    default:
        DrawString(tft, "ERROR", startX * 4 * 0.45, startY * 3 * 0.58, 5, RA8875_WHITE, color);
        break;
    }

    drive_state = curr_drive_state;
    int rounded_wheel_speed = round(wheel_speed);

    // int digit_spacing = 8;
    // int char_width = 80;

    startX -= char_width / 2;

    // Making a naive assumption that 0 <= wheel speed < 100
    if (wheel_speed > 99)
    {
        startX += char_width;
    }
    else if (wheel_speed > 9)
    {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_wheel_speed > 0)
    {
        int digit = rounded_wheel_speed % 10;
        tft.drawChar(startX * 2 + 40, startY * 3 * 0.34, digit + '0', RA8875_BLACK, color, draw_digit_size);
        wheel_speed_startX -= char_width + digit_spacing;
        rounded_wheel_speed /= 10;
    }
}

void Dash::DrawIMDStatus(Adafruit_RA8875 tft, int startX, int startY, int imd_status, int squareSize)
{
    std::string status;
    switch (imd_status)
    {
    case -10:
        status = "IMD:Short Circuit";
        ifIMDfault = true;
        break;
    case -5:
        status = "IMD:Loading";
        ifIMDfault = true;
        break;
    case -25:
        status = "IMD:Connection Fault";
        ifIMDfault = true;
        break; // added
    case -20:
        status = "IMD:Device Error";
        ifIMDfault = true;
        break;
    default:
        return;
        ifIMDfault = false;
    }

    HandleError(tft, status, startX, startY, IMD_FAULT);
}

void Dash::HandleBMSFaults(Adafruit_RA8875 tft, int startX, int startY)
{
    if (this->bms_faults == 0)
    { // order or no fault?
        ifBMSfault = false;
        return;
    }

    if (this->prev_bms_faults == this->bms_faults)
    {
        return;
    }

    this->prev_bms_faults = this->bms_faults;

    // there is a fault
    std::cout << "DETECTED: BMS Faults: " << std::bitset<8>(bms_faults).to_string() << std::endl;
    ifBMSfault = true;
    std::string error_message = "BMS:";

    if (this->bms_faults & MASK(1))
    {
        error_message += "UND_VOL,"; // under voltage
    }
    if (this->bms_faults & MASK(2))
    {
        error_message += "OVR_VOL,"; // over voltage
    }
    if (this->bms_faults & MASK(3))
    {
        error_message += "UND_TEMP,"; // under temperature
    }
    if (this->bms_faults & MASK(4))
    {
        error_message += "OVR_TEMP,"; // over temperature
    }
    if (this->bms_faults & MASK(5))
    {
        error_message += "OVR_CUR,"; // over current
    }
    if (this->bms_faults & MASK(6))
    {
        error_message += "EXTN_KL,"; // external kill
    }
    if (this->bms_faults & MASK(7))
    {
        error_message += "OPN_WIRE,"; // open wire
    }

    // remove the last comma
    error_message.pop_back();
    HandleError(tft, error_message, startX, startY, BMS_FAULT);
}

void Dash::DrawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir)
{
    for (int i = 0; i < message.length(); i++)
    {
        switch (dir)
        {
        case LEFT_TO_RIGHT:
            tft.drawChar(startX + i * size * 6, startY, message[i], color, backgroundColor, size);
            break;
        case UP_TO_DOWN:
            tft.drawChar(startX, startY + i * size * 8, message[i], color, backgroundColor, size);
            break;
        default:
            break;
        }
    }
}

void Dash::HandleECUFaults(Adafruit_RA8875 tft, int startX, int startY)
{
    if (this->ecu_faults)
    { // order or no fault?
        ifECUfault = false;
        return;
    }

    if (this->prev_ecu_faults == this->ecu_faults)
    {
        return;
    }

    this->prev_ecu_faults = this->ecu_faults;

    // there is a fault
    std::cout << "DETECTED: ECU Faults: "; // << std::bitset<8>(bms_faults).to_string() << std::endl; //!?
    ifECUfault = true;
    std::string error_message = "ECU:";

    // error acronyms tbd
    if (static_cast<bool>(this->ecu_implausibility_present))
    {
        this->ecu_faults = 1;
        error_message += "IMPLS_PRSNT,";
    }
    if (static_cast<bool>(this->ecu_implausibility_appss_disagreement_imp))
    {
        this->ecu_faults = 2;
        error_message += "IMPLS_APPS_DIS_IMP";
    }
    if (static_cast<bool>(this->ecu_implausibility_bppc_imp))
    {
        this->ecu_faults = 3;
        error_message += "IMPLS_BPPC_IMP,";
    }
    if (static_cast<bool>(this->ecu_implausibility_brake_invalid_imp))
    {
        this->ecu_faults = 4;
        error_message += "IMPLS_BRK_INVLD_IMP,";
    }
    if (static_cast<bool>(this->ecu_implausibility_appss_invalid_imp))
    {
        this->ecu_faults = 5;
        error_message += "IMPLS_APPS_INVLD_IMP,";
    }

    // remove the last comma
    error_message.pop_back();
    HandleError(tft, error_message, startX, startY, ECU_FAULT);
}

void Dash::HandleInverterFaults(Adafruit_RA8875 tft, int startX, int startY)
{
    if (this->inverter_faults == 0) // made a separate variable to prevent possible direct editing of inverter_fault_status
                                    // might not actually be useful lol b/c now I don't think the inverter_fault is ever edited
    {                               // order or no fault?
        ifInverterfault = false;
        return;
    }

    if (this->prev_inverter_faults == this->inverter_faults)
    {
        return;
    }

    this->prev_inverter_faults = this->inverter_faults;

    // there is a fault
    std::cout << "DETECTED: Inverter Faults: "; //<< std::bitset<8>(bms_faults).to_string() << std::endl;
    ifInverterfault = true;
    std::string error_message = "Inverter:";

    // error acronyms tbd
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x01)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "OVR_VOL,"; // over voltage
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x02)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "UND_VOL,"; // under voltage
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x03)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "DRV_FLT,"; // drv fault
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x04)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "ABS_OVR_CUR,"; // ABS_OVER_CURRENT
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x05)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "OVR_TEMP_FET,"; // OVER_TEMP_FET
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x06)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "OVR_TEMP_MTR,"; // OVER_TEMP_MOTOR
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x07)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "GTE_DRV_OV,"; // GATE_DRIVER_OVER_VOLTAGE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x08)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "GTE_DRV_UV,"; // GATE_DRIVER_UNDER_VOLTAGE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x09)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "MCU_UV,"; // MCU_UNDER_VOLTAGE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0A)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "BTING_FRM_WCHDG_RST,"; // BOOTING_FROM_WATCHDOG_RESET
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0B)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_SPI_FLT,"; // ENCODER_SPI_FAULT
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0C)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_SICO_BLW_MAX_AMP,"; // ENCODER_SINCOS_BELOW_MIN_AMPLITUDE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0D)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_SICO_ABV_MAX_AMP,"; // ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0E)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "FLASH_COR,"; // FLASH_CORRUPTION
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0F)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "HI_OFFS_CUR_SENS1,"; // HIGH_OFFSET_CURRENT_SENSOR_1
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x10)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "HI_OFFS_CUR_SENS2,"; // HIGH_OFFSET_CURRENT_SENSOR_2
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x11)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "HI_OFFS_CUR_SENS3,"; // HIGH_OFFSET_CURRENT_SENSOR_3
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x12)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "UNBAL_CUR,"; // UNBALANCED_CURRENTS
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x13)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "BRK_FLT,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x14)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "RESLVR_LOT,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x15)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "RESLVR_DOS,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x16)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "RESLVR_LOS,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x17)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "FLSH_COR_APP_CFG,"; // FLASH_CORRUPTION_APP_CFG
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x18)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "FLSH_COR_APP_CFG,"; // FLASH_CORRUPTION_APP_CFG
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x19)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_NO_MAG,"; // ENCODER_NO_MAGNET
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x1A)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_MAG_2_STRNG,"; // ENCODER_MAGNET_TOO_STRONG
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x1B)
    {
        this->inverter_faults = inverter_fault_status;
        error_message += "PHS_FILTR_FLT,"; // PHASE_FILTER_FAULT
    }

    // remove the last comma
    error_message.pop_back();
    HandleError(tft, error_message, startX, startY, INVERTER_FAULT);
}

int Dash::CalcBarHeight(float value, float min, float max, int maxHeight)
{
    int lerp = (value - min) / (max - min) * maxHeight;
    // clamp the value between 0 and maxHeight
    return lerp > maxHeight ? maxHeight : lerp < 0 ? 0
                                                   : lerp;
}

int Dash::CalcBarWidth(float value, float min, float max, int maxWidth)
{
    int lerp = (value - min) / (max - min) * maxWidth;
    // clamp the value between 0 and maxHeight
    return lerp > maxWidth ? maxWidth : lerp < 0 ? 0
                                                 : lerp;
}

void Dash::RecordBMSFaults()
{
    uint8_t faults = 0;
    // bit mask for the faults
    faults |= static_cast<bool>(bms_fault_summary_signal) << 0;
    faults |= static_cast<bool>(bms_fault_under_voltage_signal) << 1;
    faults |= static_cast<bool>(bms_fault_over_voltage_signal) << 2;
    faults |= static_cast<bool>(bms_fault_under_temperature_signal) << 3;
    faults |= static_cast<bool>(bms_fault_over_temperature_signal) << 4;
    faults |= static_cast<bool>(bms_fault_over_current_signal) << 5;
    faults |= static_cast<bool>(bms_fault_external_kill_signal) << 6;
    faults |= static_cast<bool>(bms_fault_open_wire_signal) << 7;

    std::cout << "BMS Faults: " << std::bitset<8>(faults).to_string() << std::endl;
    bms_faults = faults;
}

void Dash::HandleError(Adafruit_RA8875 tft, std::string error_message, int startX, int startY, Error type)
{
    if (ifBMSfault == true || ifIMDfault == true)
    {
        digitalWrite(INDICATOR_LED, HIGH);
    }
    if (ifBMSfault == true || ifIMDfault == true || ifECUfault == true || ifInverterfault == true)
    {
        tft.fillScreen(RA8875_RED);
    }
    // make it so that the draw drive state and wheel state are small and off to the side and the error messages are big in the center
    switch (this->prev_drive_state)
    {
    case 0:
        DrawString(tft, "OFF", 20, 20, 5, RA8875_WHITE, RA8875_RED);
        break;
    case 1:
        DrawString(tft, "NEUTRAL", 20, 20, 5, RA8875_WHITE, RA8875_YELLOW);
        break;
    case 2:
        DrawString(tft, "DRIVE", 20, 20, 5, RA8875_WHITE, RA8875_GREEN);
        break;
    default:
        DrawString(tft, "ERROR", 20, 20, 5, RA8875_WHITE, RA8875_RED);
        break;
    }

    int rounded_wheel_speed = round(this->prev_wheel_speed);
    std::string speed_str = std::to_string(rounded_wheel_speed);
    int wheelx = 20;
    int wheely = 30;
    int char_space = 5;
    for (char ch : speed_str)
    {
        tft.drawChar(wheelx, wheely, ch, RA8875_WHITE, RA8875_RED, 1);
        wheelx += char_space;
    }

    DrawString(tft, error_message, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4, RA8875_BLACK, RA8875_RED);
}

void Dash::DrawState(Adafruit_RA8875 tft, int startX, int startY, int display_value, int squareSize, int midstate, int laststate)
{
    int curr_state = 0;

    if (display_value > laststate)
    {
        curr_state = 2;
    }
    else if (display_value > midstate)
    {
        curr_state = 1;
    }
    else
    {
        curr_state = 0;
    }
    int16_t color;
    switch (curr_state)
    {
    case 0:
        color = FERN_GREEN;
        break;
    case 1:
        color = GOLD;
        break;
    case 2:
        color = INDIAN_RED;
        break;
    }

    tft.fillCircle(SCREEN_WIDTH / 8, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH / 8, color);
    // DrawString(tft, "IC", startX * 0.8, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    // drive_state = curr_accum_state;
    int rounded_display_value = round(display_value);

    int digit_spacing = -14;
    int char_width = 80;

    startX -= char_width / 2;

    // Making a naive assumption that 0 <= wheel speed < 100
    if (display_value > 99)
    {
        startX += char_width;
    }
    else if (display_value > 9)
    {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_display_value > 0)
    {
        int digit = rounded_display_value % 10;
        tft.drawChar(startX + 4, startY * 0.8 + SCREEN_WIDTH / 16, digit + '0', RA8875_BLACK, color, 11);
        startX -= char_width + digit_spacing;
        rounded_display_value /= 10;
    }
}