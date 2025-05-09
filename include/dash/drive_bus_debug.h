#ifndef __DRIVE_BUS_DEBUG_SCREEN_H__
#define __DRIVE_BUS_DEBUG_SCREEN_H__

#include <Adafruit_RA8875.h>

#include "resources.h"
#include "screen.h"

class DriveBusDebugScreen : public Screen {
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);
};

#endif  // __DRIVE_BUS_DEBUG_SCREEN_H__