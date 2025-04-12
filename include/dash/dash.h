#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#include <map>
#include <memory>
#include <string>

#include "define.h"
#include "screen.h"

enum DashScreen {
    DS_DRIVE,
    DS_ERROR,
    DS_NUM_SCREENS,
};

class Dash {
   public:
    Dash();
    void initalize();
    void update(Adafruit_RA8875 tft);
    void changeScreen(DashScreen screen);

   private:
    Adafruit_RA8875 _tft;
    DashScreen _currentScreen;
    std::array<std::shared_ptr<Screen>, DS_NUM_SCREENS> _screens;
};
