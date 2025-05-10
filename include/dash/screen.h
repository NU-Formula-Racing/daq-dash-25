#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Adafruit_RA8875.h>

class Screen {
   public:
    virtual void draw(Adafruit_RA8875 tft) {}
    virtual void update(Adafruit_RA8875 tft, bool force = false) {}
    virtual void periodicDraw(Adafruit_RA8875) {};
};

#endif  // __SCREEN_H__