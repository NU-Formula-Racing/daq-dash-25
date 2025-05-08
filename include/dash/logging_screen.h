#ifndef __LOGGING_SCREEN_H__
#define __LOGGING_SCREEN_H__

#include "screen.h"

class LoggingScreen : public Screen {
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);
};


#endif // __LOGGING_SCREEN_H__