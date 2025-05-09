#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include <virtualTimer.h>

#include <map>
#include <memory>
#include <string>

#include "define.h"
#include "screen.h"

enum DashScreen {
    DS_DRIVE,
    DS_ERROR,
    DS_DRIVE_DEBUG,
    DS_NUM_SCREENS,
};

class Dash {
   public:
    Dash();
    void initalize();
    void update();
    void changeScreen(DashScreen screen);

   private:
    Adafruit_RA8875 _tft;
    DashScreen _currentScreen;
    std::array<std::shared_ptr<Screen>, DS_NUM_SCREENS> _screens;
    bool _imdErrored = false;
    VirtualTimerGroup _timer;

    long long _deltaTime;
    long long _lastTime;
};
