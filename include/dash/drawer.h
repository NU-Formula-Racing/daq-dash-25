#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <Adafruit_RA8875.h>

#include <string>

enum Direction {
    LEFT_TO_RIGHT,
    UP_TO_DOWN
};

class Drawer {
   public:
    static void drawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir = LEFT_TO_RIGHT);
    static void drawNum(Adafruit_RA8875 tft, float num, int startX, int startY, int size, int16_t color, int16_t backgroundColor);
};

#endif  // __DRAWER_H__