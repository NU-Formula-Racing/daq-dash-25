#ifndef __DRIVE_SCREEN_H__
#define __DRIVE_SCREEN_H__

#include <Adafruit_RA8875.h>
#include "screen.h"
#include "resources.h"


class DriveScreen : public Screen {
    public:
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);
    void setupEncoder();
    
    private:
    RotaryEncoder rotaryEncoder;
};

#endif // __DRIVE_SCREEN_H__