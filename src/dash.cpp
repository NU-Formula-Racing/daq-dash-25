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


int drive_state_startX = SCREEN_WIDTH / 2;
int drive_state_startY = SCREEN_HEIGHT / 2 - 160;
int motor_temp_startX = SCREEN_WIDTH / 8;
int motor_temp_startY = SCREEN_HEIGHT / 4;
int inverter_current_drawn_startX = SCREEN_WIDTH / 8;
int inverter_current_drawn_startY = SCREEN_HEIGHT * 3 / 4;
int min_volt_startX = SCREEN_WIDTH * 7 / 8;
int min_volt_startY = SCREEN_HEIGHT / 4;
int battery_volt_startX = SCREEN_WIDTH * 7 / 8;
int battery_volt_startY = SCREEN_HEIGHT * 3 / 4;
int wheel_speed_startX = SCREEN_WIDTH / 2 + 40;
int wheel_speed_startY = SCREEN_HEIGHT * 0.34;
int coolant_temp_startX = SCREEN_WIDTH / 4;
int max_cell_temp_startX = SCREEN_WIDTH / 4;
int inverter_temp_startX = SCREEN_WIDTH / 4;
int coolant_temp_startY = SCREEN_HEIGHT * 0.7;
int max_cell_temp_startY = SCREEN_HEIGHT * 0.8;
int inverter_temp_startY = SCREEN_HEIGHT * 0.9;

// for states, after mid state, goes to last state
int motor_temp_last_state = 70;
int motor_temp_mid_state = 30;
int inverter_current_last_state = 100;
int inverter_current_mid_state = 50;
int min_voltage_last_state = 2.7;
int min_voltage_mid_state = 3.2;
int battery_voltage_last_state = 3.4;
int battery_voltage_mid_state = 3.2;

int bar_max_size = 480;

float motor_temp;
int drive_state = -1;
bool drive_state_drawn = false;

bool ifBMSfault=false;
bool ifIMDfault=false;

void Dash::GetCAN() {
    g_can_bus.Tick();
}

void Dash::Initialize() {
    digitalWrite(INDICATOR_LED,LOW);
    g_can_bus.Initialize(ICAN::BaudRate::kBaud500K);
    g_can_bus.RegisterRXMessage(rx_wheel_speeds);
    g_can_bus.RegisterRXMessage(rx_drive_state);
    g_can_bus.RegisterRXMessage(rx_inverter_temp_status);
    // g_can_bus.RegisterRXMessage(rx_inverter_current_draw);
    g_can_bus.RegisterRXMessage(rx_bms_faults);
    g_can_bus.RegisterRXMessage(rx_bms_status);
    g_can_bus.RegisterRXMessage(rx_coolant_state);


    this->bars["coolant_temp"] = BarData("", 0, 100, SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT * 0.725, 15, SCREEN_WIDTH / 2 - 90);
    this->bars["inverter_temp"] = BarData("", 0, 100, SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT * 0.825, 15, SCREEN_WIDTH / 2 - 90);
    this->bars["motor_temp"] = BarData("", 0, 100, SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT * 0.925, 15, SCREEN_WIDTH / 2 - 90);
}

void Dash::DrawBackground(Adafruit_RA8875 tft, int16_t color) {
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

    // write text beneath the bars
    // iterate
    for (auto &bar : this->bars) {
        BarData &data = bar.second;
        DrawString(tft, data.displayName, data.x, data.y + 10, 4, RA8875_WHITE, RA8875_BLACK);
    }

    if (this->error != NO_ERROR) {
        return;
    }
}

float Dash::WheelSpeedAvg(float fl_wheel_speed, float fr_wheel_speed) {
    return (fl_wheel_speed + fr_wheel_speed) / 2;
}

void Dash::UpdateDisplay(Adafruit_RA8875 tft) {
#ifndef DEBUG
    float fl_wheel_speed = static_cast<float>(fl_wheel_speed_signal);
    float fr_wheel_speed = static_cast<float>(fr_wheel_speed_signal);
    uint8_t curr_drive_state = static_cast<uint8_t>(drive_state_signal);     
    int imd_status = static_cast<int>(imd_status_signal);
    float coolant_temp = static_cast<float>(coolant_temp_signal);            //
    int inverter_temp = static_cast<int>(inverter_temp_status_igbt_temp);    // unknown
    int motor_temp = static_cast<int>(inverter_temp_status_motor_temp);      // unknown
    float battery_voltage = static_cast<float>(bms_battery_voltage_signal);  //
    float min_voltage = static_cast<float>(bms_min_cell_voltage_signal);
    float max_cell_temp = static_cast<float>(bms_max_cell_temp_signal);  //
    float inverter_current_drawn = static_cast<uint32_t>(inverter_current_draw_ah_drawn);
#else
    // we should change the drive state for testing
    // cycle based on time
    float fl_wheel_speed = (millis() / 200) % 200;         //
    float fr_wheel_speed = (millis() / 200) % 200;         //
    int curr_drive_state = (millis() / 1000) % 3;          //
    int imd_status = millis() > 5000 ? -10 : 0;            //
    this->bms_faults = millis() > 10000 ? 0b11111111 : 0;  //

    float coolant_temp = (millis() / 100) % 100;  //
    int inverter_temp = (millis() / 20) % 100;
    int inverter_current_drawn = (millis() / 20) % 100;
    float motor_temp = (millis() / 10) % 100;
    float battery_voltage = (millis() / 100) % 100;  //
    float min_voltage = (millis() / 20) % 100;       //
    float max_cell_temp = (millis() / 10) % 100;     //

#endif
    float avg_wheel_speed = fl_wheel_speed + fr_wheel_speed / 2;

    if (this->prev_drive_state != curr_drive_state || FORCE_DRAW) {
        DrawDriveState(tft, drive_state_startX, drive_state_startY, curr_drive_state, 8, avg_wheel_speed, wheel_speed_startX, wheel_speed_startY);
        this->prev_drive_state = curr_drive_state;
    }
    // this->prev_wheel_speed = avg_wheel_speed;
    if (this->prev_motor_temp != motor_temp || FORCE_DRAW) {
        DrawMotorState(tft, motor_temp_startX, motor_temp_startY, motor_temp, 8);
        this->prev_motor_temp = motor_temp;
    }

    if (this->prev_inverter_current_drawn != inverter_current_drawn || FORCE_DRAW) {
        DrawInvCurState(tft, inverter_current_drawn_startX, inverter_current_drawn_startY, inverter_current_drawn, 8);
        this->prev_inverter_current_drawn = inverter_current_drawn;
    }
    if (this->prev_min_volt != min_voltage || FORCE_DRAW) {
        DrawMinVoltState(tft, min_volt_startX, min_volt_startY, min_voltage, 8);
        this->prev_min_volt = min_voltage;
    }
    if (this->prev_bat_volt != battery_voltage || FORCE_DRAW) {
        DrawBatteryVoltState(tft, battery_volt_startX, battery_volt_startY, battery_voltage, 8);
        this->prev_bat_volt = battery_voltage;
    }
    if (this->prev_coolant_temp != coolant_temp || FORCE_DRAW) {
        DrawCoolantTemp(tft, coolant_temp, coolant_temp_startX, coolant_temp_startY);
        this->prev_coolant_temp = coolant_temp;
    }
    if (this->prev_max_cell_temp != max_cell_temp || FORCE_DRAW) {
        DrawMaxCellTemp(tft, max_cell_temp, max_cell_temp_startX, max_cell_temp_startY);
        this->prev_max_cell_temp = max_cell_temp;
    }
    if (this->prev_inverter_temp != inverter_temp || FORCE_DRAW) {
        DrawInverterTemp(tft, inverter_temp, inverter_temp_startX, inverter_temp_startY);
        this->prev_inverter_temp = inverter_temp;
    }

    // draw IMD status
    if(ifBMSfault==false & ifIMDfault==false){
        digitalWrite(INDICATOR_LED, LOW);
    }
    DrawIMDStatus(tft, 8, 2, imd_status, 32);
    HandleBMSFaults(tft, 8, 2);

    // draw the test bar
    this->DrawBar(tft, "coolant_temp", coolant_temp, FERN_GREEN, this->backgroundColor);
    this->DrawBar(tft, "inverter_temp", inverter_temp, GOLD, this->backgroundColor);
    this->DrawBar(tft, "motor_temp", max_cell_temp, RA8875_BLUE, this->backgroundColor);

    // this->DrawBar(tft, "battery_voltage", battery_voltage, FERN_GREEN, this->backgroundColor);
    // this->DrawBar(tft, "min_voltage", min_voltage, GOLD, this->backgroundColor);
    // this->DrawBar(tft, "max_cell_temp", max_cell_temp, RA8875_BLUE, this->backgroundColor);

    timer_group.Tick(millis());
}

void Dash::DrawBar(Adafruit_RA8875 tft, std::string barName, float newValue, int16_t barColor, int16_t backgroundColor) {
    // get the bar if any
    if (this->bars.find(barName) == this->bars.end()) {
        return;
    }

    BarData &bar = this->bars[barName];

    // are the values the same?
    if (bar.value == newValue) {
        return;
    }

    // calculate the height of the bar
    int newWidth = CalcBarWidth(newValue, bar.min, bar.max, bar.maxWidth);
    int oldWidth = CalcBarWidth(bar.value, bar.min, bar.max, bar.maxWidth);

    if (newWidth == oldWidth) {
        return;
    }

    // draw the bar
    int diff = newWidth - oldWidth;
    bar.value = newValue;

    // std::cout << "Drawing bar " << barName << " with value " << newValue << " and height " << newHeight << std::endl;

    if (diff > 0) {
        // if the new height is greater than the old height, we need to fill in the difference
        // we will draw the bar to go upwards
        // the top left of the screen is 0,0
        // tft.fillRect(bar.x + newWidth, bar.y, diff, bar.height, barColor);
        tft.fillRect(bar.x + oldWidth, bar.y, diff, bar.height, barColor);
    } else {
        // if the new height is less than the old height, we need to clear the difference
        // we will draw the bar to go downwards
        // tft.fillRect(bar.x + oldWidth, bar.y, -diff, bar.height, NORTHWESTERN_PURPLE);
        tft.fillRect(bar.x + newWidth, bar.y, -diff, bar.height, backgroundColor);
    }

    // write the value at the bottom of the
}

// COME HERE NEXT TIME TO DRAW THE NUMBER IN THE MIDDLE. ALSO, CHANGE DRAWCHAR FOR OTHER DRAW STATE CIRCLES AND THE MIDDLE RECTANGLE BC FULL WORDS

void Dash::DrawCoolantTemp(Adafruit_RA8875 tft, float coolant_temp, int startX, int startY) {
    DrawString(tft, "CT", startX + 2, startY, 5, RA8875_WHITE, RUSSIAN_VIOLET);
}

void Dash::DrawMaxCellTemp(Adafruit_RA8875 tft, float max_cell_temp, int startX, int startY) {
    DrawString(tft, "MCT", startX + 2, startY, 5, RA8875_WHITE, RUSSIAN_VIOLET);
}

void Dash::DrawInverterTemp(Adafruit_RA8875 tft, int inverter_temp, int startX, int startY) {
    DrawString(tft, "IT", startX + 2, startY, 5, RA8875_WHITE, RUSSIAN_VIOLET);
}

// Draws drive state on screen based on CAN signal
void Dash::DrawDriveState(Adafruit_RA8875 tft, int startX, int startY, uint8_t curr_drive_state, int squareSize, float wheel_speed, int wheel_speed_startX, int wheel_speed_startY) {
    int16_t color = INDIAN_RED;
    switch (curr_drive_state) {
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

    tft.fillRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, color);

    switch (curr_drive_state) {
        case 0:
            DrawString(tft, "OFF", SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.58, 5, RA8875_WHITE, color);
            break;
        case 1:
            DrawString(tft, "NEUTRAL", SCREEN_WIDTH * 0.47, SCREEN_HEIGHT * 0.58, 5, RA8875_BLACK, color);
            break;
        case 2:
            DrawString(tft, "DRIVE", SCREEN_WIDTH * 0.45, SCREEN_HEIGHT * 0.58, 5, RA8875_WHITE, color);
            break;
        default:
            DrawString(tft, "ERROR", SCREEN_WIDTH * 0.45, SCREEN_HEIGHT * 0.58, 5, RA8875_WHITE, color);
            break;
    }

    drive_state = curr_drive_state;
    int rounded_wheel_speed = round(wheel_speed);

    int digit_spacing = 8;
    int char_width = 80;

    startX -= char_width / 2;

    // Making a naive assumption that 0 <= wheel speed < 100
    if (wheel_speed > 99) {
        startX += char_width;
    } else if (wheel_speed > 9) {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_wheel_speed > 0) {
        int digit = rounded_wheel_speed % 10;
        tft.drawChar(wheel_speed_startX, wheel_speed_startY, digit + '0', RA8875_BLACK, color, 13);
        wheel_speed_startX -= char_width + digit_spacing;
        rounded_wheel_speed /= 10;
    }
}

// Draws motor temp circle
void Dash::DrawMotorState(Adafruit_RA8875 tft, int startX, int startY, int motor_temp, int squareSize) {
    int16_t color;
    int curr_motor_state = 0;
    if (motor_temp > motor_temp_last_state) {
        curr_motor_state = 2;
    } else if (motor_temp > motor_temp_mid_state) {
        curr_motor_state = 1;
    } else {
        curr_motor_state = 0;
    }

    switch (curr_motor_state) {
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

    tft.fillCircle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 8, color);
    DrawString(tft, "MT", startX * 0.8, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    int rounded_motor_temp = round(motor_temp);
    int digit_spacing = -14;
    int char_width = 80;

    startX -= char_width / 2;

    // Making a naive assumption that 0 <= wheel speed < 100
    if (motor_temp > 99) {
        startX += char_width;
    } else if (motor_temp > 9) {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_motor_temp > 0) {
        int digit = rounded_motor_temp % 10;
        tft.drawChar(startX + 4, startY * 0.8, digit + '0', RA8875_BLACK, color, 11);
        startX -= char_width + digit_spacing;
        rounded_motor_temp /= 10;
    }
}

void Dash::DrawInvCurState(Adafruit_RA8875 tft, int startX, int startY, int inverter_current_drawn, int squareSize) {
    int curr_inverter_current_drawn_state = 0;

    if (inverter_current_drawn > inverter_current_last_state) {
        curr_inverter_current_drawn_state = 2;
    } else if (inverter_current_drawn > inverter_current_mid_state) {
        curr_inverter_current_drawn_state = 1;
    } else {
        curr_inverter_current_drawn_state = 0;
    }
    int16_t color;
    switch (curr_inverter_current_drawn_state) {
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
    DrawString(tft, "IC", startX * 0.8, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    // drive_state = curr_accum_state;
    int rounded_inverter_current_drawn = round(inverter_current_drawn);

    int digit_spacing = -14;
    int char_width = 80;

    startX -= char_width / 2;

    // Making a naive assumption that 0 <= wheel speed < 100
    if (inverter_current_drawn > 99) {
        startX += char_width;
    } else if (inverter_current_drawn > 9) {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_inverter_current_drawn > 0) {
        int digit = rounded_inverter_current_drawn % 10;
        tft.drawChar(startX + 4, startY * 0.8 + SCREEN_WIDTH / 16, digit + '0', RA8875_BLACK, color, 11);
        startX -= char_width + digit_spacing;
        rounded_inverter_current_drawn /= 10;
    }
}

// Draws min voltage circle
void Dash::DrawMinVoltState(Adafruit_RA8875 tft, int startX, int startY, int min_voltage, int squareSize) {
    int curr_minVolt_state = 0;

    if (min_voltage < min_voltage_last_state) {
        curr_minVolt_state = 2;
    } else if (min_voltage < min_voltage_mid_state) {
        curr_minVolt_state = 1;
    } else {
        curr_minVolt_state = 0;
    }

    int16_t color;
    switch (curr_minVolt_state) {
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

    tft.fillCircle(SCREEN_WIDTH * 7 / 8, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 8, color);
    DrawString(tft, "MV", startX - 20, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    int rounded_min_voltage = round(min_voltage);
    int digit_spacing = -14;
    int char_width = 80;
    startX -= char_width / 2;
    // Making a naive assumption that 0 <= wheel speed < 100
    if (min_voltage > 99) {
        startX += char_width;
    } else if (min_voltage > 9) {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_min_voltage > 0) {
        int digit = rounded_min_voltage % 10;
        tft.drawChar(startX + 4, startY * 0.8, digit + '0', RA8875_BLACK, color, 11);
        startX -= char_width + digit_spacing;
        rounded_min_voltage /= 10;
    }
}

// Draws battery voltage circle
void Dash::DrawBatteryVoltState(Adafruit_RA8875 tft, int startX, int startY, int battery_voltage, int squareSize) {
    int curr_batteryVolt_state = 0;

    if (battery_voltage > battery_voltage_last_state) {
        curr_batteryVolt_state = 2;
    } else if (battery_voltage > battery_voltage_mid_state) {
        curr_batteryVolt_state = 1;
    } else {
        curr_batteryVolt_state = 0;
    }

    int16_t color = INDIAN_RED;
    switch (curr_batteryVolt_state) {
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

    tft.fillCircle(SCREEN_WIDTH * 7 / 8, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH / 8, color);
    DrawString(tft, "BV", startX - 20, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
    // drive_state = curr_batteryVolt_state;
    int rounded_battery_voltage = round(battery_voltage);
    int digit_spacing = -14;
    int char_width = 80;
    startX -= char_width / 2;
    // Making a naive assumption that 0 <= wheel speed < 100
    if (battery_voltage > 99) {
        startX += char_width;
    } else if (battery_voltage > 9) {
        // Digits must be off center for double digit numbers
        startX += char_width / 2;
    }

    // Draw the digits
    while (rounded_battery_voltage > 0) {
        int digit = rounded_battery_voltage % 10;
        tft.drawChar(startX + 4, startY * 0.8 + SCREEN_WIDTH / 16, digit + '0', RA8875_BLACK, color, 11);
        startX -= char_width + digit_spacing;
        rounded_battery_voltage /= 10;
    }
}

void Dash::DrawIMDStatus(Adafruit_RA8875 tft, int startX, int startY, int imd_status, int squareSize) {
    std::string status;
    switch (imd_status) {
        case -10:
            status = "IMD:Short Circuit";
            ifIMDfault=true;
            break;
        case -5:
            status = "IMD:Loading";
            ifIMDfault=true;
            break;
        case -25:
            status = "IMD:Connection Fault";
            ifIMDfault=true;
            break; //added
        case -20:
            status = "IMD:Device Error";
            ifIMDfault=true;
            break;
        default:
            return;
            ifIMDfault=false;
    }

    HandleError(tft, status, startX, startY, IMD_FAULT);
}

void Dash::HandleBMSFaults(Adafruit_RA8875 tft, int startX, int startY) {
    if (this->bms_faults == 0) {
        ifBMSfault=false;
        return;
    }

    if (this->prev_bms_faults == this->bms_faults) {
        return;
    }

    this->prev_bms_faults = this->bms_faults;

    // there is a fault
    std::cout << "DETECTED: BMS Faults: " << std::bitset<8>(bms_faults).to_string() << std::endl;
    ifBMSfault=true;
    std::string error_message = "BMS:";

    if (this->bms_faults & MASK(1)) {
        error_message += "UV,";  // under voltage
    }
    if (this->bms_faults & MASK(2)) {
        error_message += "OV,";  // over voltage
    }
    if (this->bms_faults & MASK(3)) {
        error_message += "UT,";  // under temperature
    }
    if (this->bms_faults & MASK(4)) {
        error_message += "OT,";  // over temperature
    }
    if (this->bms_faults & MASK(5)) {
        error_message += "OC,";  // over current
    }
    if (this->bms_faults & MASK(6)) {
        error_message += "EK,";  // external kill
    }
    if (this->bms_faults & MASK(7)) {
        error_message += "OW,";  // open wire
    }

    // remove the last comma
    error_message.pop_back();
    HandleError(tft, error_message, startX, startY, BMS_FAULT);
}

void Dash::DrawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir) {
    for (int i = 0; i < message.length(); i++) {
        switch (dir) {
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

int Dash::CalcBarHeight(float value, float min, float max, int maxHeight) {
    int lerp = (value - min) / (max - min) * maxHeight;
    // clamp the value between 0 and maxHeight
    return lerp > maxHeight ? maxHeight : lerp < 0 ? 0
                                                   : lerp;
}

int Dash::CalcBarWidth(float value, float min, float max, int maxWidth) {
    int lerp = (value - min) / (max - min) * maxWidth;
    // clamp the value between 0 and maxHeight
    return lerp > maxWidth ? maxWidth : lerp < 0 ? 0
                                                 : lerp;
}

void Dash::RecordBMSFaults() {
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

void Dash::HandleError(Adafruit_RA8875 tft, std::string error_message, int startX, int startY, Error type) {
    tft.fillRect(SCREEN_WIDTH / 4, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6, RA8875_RED);
    DrawString(tft, error_message, SCREEN_WIDTH / 4, 0, 2, RA8875_BLACK, RA8875_RED);
    if(ifBMSfault==true || ifIMDfault==true){
        digitalWrite(INDICATOR_LED, HIGH);
    }
}