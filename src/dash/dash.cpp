#include "dash/dash.h"

#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "dash/drive_bus_debug.h"
#include "dash/drive_screen.h"
#include "dash/error_screen.h"
#include "resources.h"

int bar_max_size = 480;

DashScreen defaultScreen = DashScreen::DS_DRIVE_DEBUG;

Dash::Dash() : _tft(RA8875_CS, RA8875_RESET), _currentScreen(defaultScreen) {
    _screens = {
        std::make_shared<DriveScreen>(),
        std::make_shared<ErrorScreen>(),
        std::make_shared<DriveBusDebugScreen>()};
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
        changeScreen(DashScreen::DS_ERROR);
    } else {
        changeScreen(defaultScreen);
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

    double dHours = (double)Resources::deltaTimeMs() / (1000.0 * 60.0 * 60.0);
    Resources::instance().milageCounter += (double)Resources::driveBusData().vehicleSpeedMPH() * dHours * 4;  // hack, cause I'm tired, but seems to be wrong by a factor of 4
}

void Dash::changeScreen(DashScreen screen) {
    // are we changing the screen
    if (_currentScreen == screen) return;  // no change

    _currentScreen = screen;
    _screens[_currentScreen]->draw(_tft);
    _screens[_currentScreen]->update(_tft, true);
}