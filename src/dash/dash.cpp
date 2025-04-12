#include "dash/dash.h"

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
#define DEBUG false

int drive_state_startX = SCREEN_WIDTH / 4;
int drive_state_startY = SCREEN_HEIGHT / 3;
int error_state_startX = SCREEN_WIDTH * 0.4;
int error_state_startY = SCREEN_HEIGHT * 6 / 8;
int hv_bat_volt_startX = SCREEN_WIDTH / 8;
int hv_bat_volt_startY = SCREEN_HEIGHT / 4 + 30;
int lv_bat_volt_startX = SCREEN_WIDTH / 8;
int lv_bat_volt_startY = SCREEN_HEIGHT * 3 / 4 + 30;
int wheel_speed_startX = SCREEN_WIDTH / 2 + 40;
int wheel_speed_startY = SCREEN_HEIGHT * 0.34;
int max_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
int max_cell_temp_startY = SCREEN_HEIGHT / 4 + 30;
int min_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
int min_cell_temp_startY = SCREEN_HEIGHT * 3 / 4 + 30;
int handle_error_startX = 0;  // /4
int handle_error_startY = SCREEN_HEIGHT / 3;

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
int lv_battery_voltage_mid_state = 3.1;  // min 2.7
int max_cell_temp_last_state = 50;       // max 50 celsius
int max_cell_temp_mid_state = 45;
int min_cell_temp_last_state = 15;
int min_cell_temp_mid_state = 11;  // min 8 celsius

int bar_max_size = 480;

float motor_temp;
int drive_state = -1;
bool drive_state_drawn = false;

bool ifBMSfault = false;
bool ifIMDfault = false;
bool ifECUfault = false;
bool ifInverterfault = false;
bool ifErrorScreen = false;

std::string imd_err_str = "";
std::string bms_err_str = "";
std::string inverter_err_str = "";
std::string ecu_err_str = "";

void Dash::GetCAN() {

}

void Dash::Initialize() {
    digitalWrite(INDICATOR_LED, LOW);
}


void Dash::UpdateDisplay(Adafruit_RA8875 tft) {

}


void Dash::DrawIMDStatus(Adafruit_RA8875 tft, int startX, int startY, int imd_status, int squareSize) {
    std::string status;
    switch (imd_status) {
        case -10:
            status = "IMD:Short Circuit";
            ifIMDfault = true;
            imd_err_str = status;
            break;
        case -5:
            status = "IMD:Loading";
            ifIMDfault = true;
            imd_err_str = status;
            break;
        case -25:
            status = "IMD:Connection Fault";
            ifIMDfault = true;
            imd_err_str = status;
            break;  // added
        case -20:
            status = "IMD:Device Error";
            ifIMDfault = true;
            imd_err_str = status;
            break;
        default:
            ifIMDfault = false;
            return;
    }

    return;

    // HandleError(tft, status, startX, startY, IMD_FAULT);
}

void Dash::HandleBMSFaults(Adafruit_RA8875 tft, int startX, int startY) {
    if (this->bms_faults == 0) {  // order or no fault?
        ifBMSfault = false;
        return;
    }

    // ***!*************** not sure
    if (this->prev_bms_faults == this->bms_faults) {
        return;
    }

    // this->prev_bms_faults = this->bms_faults;

    // there is a fault
    std::cout << "DETECTED: BMS Faults: " << std::bitset<8>(bms_faults).to_string() << std::endl;
    ifBMSfault = true;
    std::string error_message = "BMS:";

    if (this->bms_faults & MASK(1)) {
        error_message += "UND_VOL,";  // under voltage
    }
    if (this->bms_faults & MASK(2)) {
        error_message += "OVR_VOL,";  // over voltage
    }
    if (this->bms_faults & MASK(3)) {
        error_message += "UND_TEMP,";  // under temperature
    }
    if (this->bms_faults & MASK(4)) {
        error_message += "OVR_TEMP,";  // over temperature
    }
    if (this->bms_faults & MASK(5)) {
        error_message += "OVR_CUR,";  // over current
    }
    if (this->bms_faults & MASK(6)) {
        error_message += "EXTN_KL,";  // external kill
    }
    if (this->bms_faults & MASK(7)) {
        error_message += "OPN_WIRE,";  // open wire
    }

    // remove the last comma
    error_message.pop_back();
    // HandleError(tft, error_message, startX, startY, BMS_FAULT);

    bms_err_str = error_message;
    return;
}


void Dash::HandleECUFaults(Adafruit_RA8875 tft, int startX, int startY) {
    if (this->ecu_faults) {  // order or no fault?
        ifECUfault = false;
        return;
    }

    if (this->prev_ecu_faults == this->ecu_faults) {
        return;
    }

    // this->prev_ecu_faults = this->ecu_faults;

    // there is a fault
    std::cout
        << "DETECTED: ECU Faults: ";  // << std::bitset<8>(bms_faults).to_string() << std::endl; //!?
    ifECUfault = true;
    std::string error_message = "ECU:";

    // error acronyms tbd
    if (static_cast<bool>(this->ecu_implausibility_present)) {
        this->ecu_faults = 1;
        error_message += "IMPLS_PRSNT,";
    }
    if (static_cast<bool>(this->ecu_implausibility_appss_disagreement_imp)) {
        this->ecu_faults = 2;
        error_message += "IMPLS_APPS_DIS_IMP";
    }
    if (static_cast<bool>(this->ecu_implausibility_bppc_imp)) {
        this->ecu_faults = 3;
        error_message += "IMPLS_BPPC_IMP,";
    }
    if (static_cast<bool>(this->ecu_implausibility_brake_invalid_imp)) {
        this->ecu_faults = 4;
        error_message += "IMPLS_BRK_INVLD_IMP,";
    }
    if (static_cast<bool>(this->ecu_implausibility_appss_invalid_imp)) {
        this->ecu_faults = 5;
        error_message += "IMPLS_APPS_INVLD_IMP,";
    }

    // remove the last comma
    error_message.pop_back();
    // HandleError(tft, error_message, startX, startY, ECU_FAULT);
    ecu_err_str = error_message;
    return;
}

void Dash::HandleInverterFaults(Adafruit_RA8875 tft, int startX, int startY) {
    if (this->inverter_faults == 0)  // made a separate variable to prevent possible direct editing of inverter_fault_status
                                     // might not actually be useful lol b/c now I don't think the inverter_fault is ever edited
    {                                // order or no fault?
        ifInverterfault = false;
        return;
    }

    if (this->prev_inverter_faults == this->inverter_faults) {
        return;
    }

    // this->prev_inverter_faults = this->inverter_faults;

    // there is a fault
    std::cout << "DETECTED: Inverter Faults: ";  //<< std::bitset<8>(bms_faults).to_string() << std::endl;
    ifInverterfault = true;
    std::string error_message = "Inverter:";

    // error acronyms tbd
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x01) {
        this->inverter_faults = inverter_fault_status;
        error_message += "OVR_VOL,";  // over voltage
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x02) {
        this->inverter_faults = inverter_fault_status;
        error_message += "UND_VOL,";  // under voltage
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x03) {
        this->inverter_faults = inverter_fault_status;
        error_message += "DRV_FLT,";  // drv fault
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x04) {
        this->inverter_faults = inverter_fault_status;
        error_message += "ABS_OVR_CUR,";  // ABS_OVER_CURRENT
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x05) {
        this->inverter_faults = inverter_fault_status;
        error_message += "OVR_TEMP_FET,";  // OVER_TEMP_FET
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x06) {
        this->inverter_faults = inverter_fault_status;
        error_message += "OVR_TEMP_MTR,";  // OVER_TEMP_MOTOR
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x07) {
        this->inverter_faults = inverter_fault_status;
        error_message += "GTE_DRV_OV,";  // GATE_DRIVER_OVER_VOLTAGE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x08) {
        this->inverter_faults = inverter_fault_status;
        error_message += "GTE_DRV_UV,";  // GATE_DRIVER_UNDER_VOLTAGE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x09) {
        this->inverter_faults = inverter_fault_status;
        error_message += "MCU_UV,";  // MCU_UNDER_VOLTAGE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0A) {
        this->inverter_faults = inverter_fault_status;
        error_message += "BTING_FRM_WCHDG_RST,";  // BOOTING_FROM_WATCHDOG_RESET
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0B) {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_SPI_FLT,";  // ENCODER_SPI_FAULT
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0C) {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_SICO_BLW_MAX_AMP,";  // ENCODER_SINCOS_BELOW_MIN_AMPLITUDE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0D) {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_SICO_ABV_MAX_AMP,";  // ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0E) {
        this->inverter_faults = inverter_fault_status;
        error_message += "FLASH_COR,";  // FLASH_CORRUPTION
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x0F) {
        this->inverter_faults = inverter_fault_status;
        error_message += "HI_OFFS_CUR_SENS1,";  // HIGH_OFFSET_CURRENT_SENSOR_1
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x10) {
        this->inverter_faults = inverter_fault_status;
        error_message += "HI_OFFS_CUR_SENS2,";  // HIGH_OFFSET_CURRENT_SENSOR_2
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x11) {
        this->inverter_faults = inverter_fault_status;
        error_message += "HI_OFFS_CUR_SENS3,";  // HIGH_OFFSET_CURRENT_SENSOR_3
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x12) {
        this->inverter_faults = inverter_fault_status;
        error_message += "UNBAL_CUR,";  // UNBALANCED_CURRENTS
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x13) {
        this->inverter_faults = inverter_fault_status;
        error_message += "BRK_FLT,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x14) {
        this->inverter_faults = inverter_fault_status;
        error_message += "RESLVR_LOT,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x15) {
        this->inverter_faults = inverter_fault_status;
        error_message += "RESLVR_DOS,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x16) {
        this->inverter_faults = inverter_fault_status;
        error_message += "RESLVR_LOS,";
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x17) {
        this->inverter_faults = inverter_fault_status;
        error_message += "FLSH_COR_APP_CFG,";  // FLASH_CORRUPTION_APP_CFG
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x18) {
        this->inverter_faults = inverter_fault_status;
        error_message += "FLSH_COR_APP_CFG,";  // FLASH_CORRUPTION_APP_CFG
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x19) {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_NO_MAG,";  // ENCODER_NO_MAGNET
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x1A) {
        this->inverter_faults = inverter_fault_status;
        error_message += "ENCDR_MAG_2_STRNG,";  // ENCODER_MAGNET_TOO_STRONG
    }
    if (static_cast<bool>(this->inverter_faults) && inverter_fault_status == 0x1B) {
        this->inverter_faults = inverter_fault_status;
        error_message += "PHS_FILTR_FLT,";  // PHASE_FILTER_FAULT
    }

    // remove the last comma
    error_message.pop_back();
    // HandleError(tft, error_message, startX, startY, INVERTER_FAULT);
    inverter_err_str = error_message;
    return;
}


void Dash::DrawState(Adafruit_RA8875 tft, int startX, int startY, int display_value, int squareSize, int midstate, int laststate) {
    int curr_state = 0;

    if (display_value > laststate) {
        curr_state = 2;
    } else if (display_value > midstate) {
        curr_state = 1;
    } else {
        curr_state = 0;
    }

    int16_t color;
    switch (curr_state) {
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

    tft.fillCircle(startX, startY, SCREEN_WIDTH / 10, color);
    // DrawString(tft, "IC", startX * 0.8, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    // drive_state = curr_accum_state;
    int precision = 1;  // Change this to 1, 2, 3... as needed change font size, startX, startY

    int multiplier = pow(10, precision);                         // e.g., 10 for 1 decimal, 100 for 2, etc.
    int float_as_int = (int)(display_value * multiplier + 0.5);  // rounding

    int digit_spacing = -8;
    int char_width = 48;

    startX -= char_width / 2;

    // Adjust for centering based on number of digits
    int temp = float_as_int;
    int digit_count = 0;
    while (temp > 0) {
        temp /= 10;
        digit_count++;
    }

    if (digit_count >= 5) {  // e.g. 999.99
        startX += char_width;
    } else if (digit_count >= 3) {  // e.g. 10.00
        startX += char_width / 2;
    }

    // Draw digits in reverse order (right to left)
    int digits_drawn = 0;
    while (float_as_int > 0 || digits_drawn <= precision) {
        if (digits_drawn == precision) {
            // Draw decimal point
            tft.drawChar(startX + 35, startY - 20, '.', RA8875_BLACK, color, 6);
            startX -= char_width / 2;  // Smaller space for '.'
        } else {
            int digit = float_as_int % 10;
            tft.drawChar(startX + 35, startY - 20, digit + '0', RA8875_BLACK, color, 6);
            startX -= char_width + digit_spacing;
            float_as_int /= 10;
        }
        digits_drawn++;
    }
}