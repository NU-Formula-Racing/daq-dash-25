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

    digitalWrite(IMD_INDICATOR, HIGH);
    digitalWrite(BMS_INDICATOR, HIGH);

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

    _lastTime = 0;

    _timer.AddTimer(1000,
                    [&]() {
                        _screens[_currentScreen]->periodicDraw(_tft);
                    });
}

void Dash::update() {
    _timer.Tick(millis());

    // check for errors
    if (Resources::driveBusData().faultPresent()) {
        // change the screen to an error
        // Serial.printf("Detected error!");
        // changeScreen(DashScreen::DS_ERROR);
    }

    // pull the pin for the imd
    // driven by mosfets, the logic is inverted
    bool imdFault = Resources::driveBusData().imdState == 0 || _imdErrored;
    digitalWrite(IMD_INDICATOR, imdFault ? LOW : HIGH);
    _imdErrored = imdFault;

    bool bmsFault = Resources::driveBusData().bmsFaults[BMS_FAULT_SUMMARY];
    digitalWrite(BMS_INDICATOR, bmsFault ? LOW : HIGH);

    // update the current screen
    // Serial.printf("Updating screen %d\n", (int)_currentScreen);
    _screens[_currentScreen]->update(_tft);
    // Serial.print("Finished!\n");

    long long now = millis();
    _deltaTime = now - _lastTime;
    float dSeconds = (float)_deltaTime / 1000;
    _lastTime = now;
    float rotDistanceInches = (Resources::driveBusData().averageWheelSpeed() * dSeconds) * WHEEL_DIAMETER * M_PI;
    Resources::instance().milageCounter += rotDistanceInches / (12 * 5280);
}

void Dash::changeScreen(DashScreen screen) {
    // are we changing the screen
    if (_currentScreen == screen) return;  // no change

    _currentScreen = screen;
    _screens[_currentScreen]->draw(_tft);
    _screens[_currentScreen]->update(_tft, true);
}