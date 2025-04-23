#include "dash/dash.h"

#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "dash/drive_screen.h"
#include "dash/error_screen.h"
#include "resources.h"


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

Dash::Dash() : _tft(RA8875_CS, RA8875_RESET), _currentScreen(DashScreen::DS_DRIVE) {
    _screens = {
        std::make_shared<DriveScreen>(),
        std::make_shared<ErrorScreen>()};
}

void Dash::initalize() {
    pinMode(IMD_INDICATOR, OUTPUT);
    pinMode(BMS_INDICATOR, OUTPUT);

    digitalWrite(IMD_INDICATOR, LOW);
    digitalWrite(BMS_INDICATOR, LOW);

    int numAttempts = 0;
    while (!_tft.begin(RA8875_800x480)) {
        numAttempts++;
        Serial.printf("Attempt %d to initialize RA8875 failed\n", numAttempts);
        delay(500);
    }
    Serial.println("Found RA8875");

    _tft.displayOn(true);
    _tft.GPIOX(true);                               // Enable TFT - display enable tied to GPIOX
    _tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);  // PWM output for backlight
    _tft.PWM1out(255);

    _screens[_currentScreen]->draw(_tft);
    _screens[_currentScreen]->update(_tft, true);
}

void Dash::update() {
    // check for errors
    if (Resources::driveBusData().faultPresent()) {
        // change the screen to an error
        Serial.printf("Detected error!");
        changeScreen(DashScreen::DS_ERROR);
    }

    // pull the pin for the imd
    bool imdFault = Resources::driveBusData().imdState == 0 || _imdErrored;
    digitalWrite(IMD_INDICATOR, imdFault ? HIGH : LOW);
    _imdErrored = imdFault;

    bool bmsFault = Resources::driveBusData().bmsFaults[BMS_FAULT_SUMMARY];
    digitalWrite(BMS_INDICATOR, bmsFault ? HIGH : LOW);
    
    // update the current screen
    // Serial.printf("Updating screen %d\n", (int)_currentScreen);
    _screens[_currentScreen]->update(_tft);
    // Serial.print("Finished!\n");

    // // updates time
    // cur_time = millis();
    // deltaT = (cur_time - last_time) / 1000;
    // last_time = cur_time;
}

void Dash::changeScreen(DashScreen screen) {
    // are we changing the screen
    if (_currentScreen == screen) return;  // no change

    _currentScreen = screen;
    _screens[_currentScreen]->draw(_tft);
    _screens[_currentScreen]->update(_tft, true);
}

// void Dash::DrawState(Adafruit_RA8875 tft, int startX, int startY, int display_value, int squareSize, int midstate, int laststate) {
//     int curr_state = 0;

//     if (display_value > laststate) {
//         curr_state = 2;
//     } else if (display_value > midstate) {
//         curr_state = 1;
//     } else {
//         curr_state = 0;
//     }

//     int16_t color;
//     switch (curr_state) {
//         case 0:
//             color = FERN_GREEN;
//             break;
//         case 1:
//             color = GOLD;
//             break;
//         case 2:
//             color = INDIAN_RED;
//             break;
//     }

//     tft.fillCircle(startX, startY, SCREEN_WIDTH / 10, color);
//     // DrawString(tft, "IC", startX * 0.8, startY - SCREEN_WIDTH / 9, 5, RA8875_BLACK, color);
//     // drive_state = curr_accum_state;
//     int precision = 1;  // Change this to 1, 2, 3... as needed change font size, startX, startY

//     int multiplier = pow(10, precision);                         // e.g., 10 for 1 decimal, 100 for 2, etc.
//     int float_as_int = (int)(display_value * multiplier + 0.5);  // rounding

//     int digit_spacing = -8;
//     int char_width = 48;

//     startX -= char_width / 2;

//     // Adjust for centering based on number of digits
//     int temp = float_as_int;
//     int digit_count = 0;
//     while (temp > 0) {
//         temp /= 10;
//         digit_count++;
//     }

//     if (digit_count >= 5) {  // e.g. 999.99
//         startX += char_width;
//     } else if (digit_count >= 3) {  // e.g. 10.00
//         startX += char_width / 2;
//     }

//     // Draw digits in reverse order (right to left)
//     int digits_drawn = 0;
//     while (float_as_int > 0 || digits_drawn <= precision) {
//         if (digits_drawn == precision) {
//             // Draw decimal point
//             tft.drawChar(startX + 35, startY - 20, '.', RA8875_BLACK, color, 6);
//             startX -= char_width / 2;  // Smaller space for '.'
//         } else {
//             int digit = float_as_int % 10;
//             tft.drawChar(startX + 35, startY - 20, digit + '0', RA8875_BLACK, color, 6);
//             startX -= char_width + digit_spacing;
//             float_as_int /= 10;
//         }
//         digits_drawn++;
//     }
// }