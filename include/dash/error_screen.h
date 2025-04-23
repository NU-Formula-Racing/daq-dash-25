#ifndef __ERROR_SCREEN_H__
#define __ERROR_SCREEN_H__

#include "screen.h"

class ErrorScreen : public Screen {
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);
};


#endif // __ERROR_SCREEN_H__