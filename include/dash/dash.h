#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#include <map>
#include <memory>
#include <string>

#include "define.h"
#include "screen.h"
#include "dash/rotary_encoder.h"
#ifndef DASH_H
#define DASH_H
enum DashScreen {
    DS_DRIVE,
    DS_ERROR,
    DS_LOGGING,
    DS_NUM_SCREENS,
};

class Dash {
   public:
    Dash();
    void initalize();
    void update();
    void changeScreen(DashScreen screen);
    void setupEncoder();

   private:
    Adafruit_RA8875 _tft;
    DashScreen _currentScreen;
    std::array<std::shared_ptr<Screen>, DS_NUM_SCREENS> _screens;
    bool _imdErrored = false;
    RotaryEncoder rotaryEncoder;

    long long _deltaTime;
    long long _lastTime;
};
#endif