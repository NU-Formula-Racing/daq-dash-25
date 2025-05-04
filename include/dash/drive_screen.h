#ifndef __DRIVE_SCREEN_H__
#define __DRIVE_SCREEN_H__

#include <Adafruit_RA8875.h>

#include "resources.h"
#include "screen.h"

class DriveScreen : public Screen {
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);
    void periodicDraw(Adafruit_RA8875 tft);
};

#endif  // __DRIVE_SCREEN_H__