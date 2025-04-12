#include "dash/drawer.h"
#include <sstream>

void Drawer::drawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir = LEFT_TO_RIGHT) {
    for (int i = 0; i < message.length(); i++) {
        switch (dir) {
            case LEFT_TO_RIGHT:
                tft.drawChar(startX + i * size * 6, startY, message[i], color, backgroundColor, size);
                break;
            case UP_TO_DOWN:
                tft.drawChar(startX, startY + i * size * 8, message[i], color, backgroundColor, size);
                break;
            default:
                break;
        }
    }
}

void Drawer::drawNum(Adafruit_RA8875 tft, float num, int startX, int startY, int size, int16_t color, int16_t backgroundColor) {
    std::stringstream ss;
    ss << num;   
    Drawer::drawString(tft, ss.str(), startX, startY, size, color, backgroundColor);
}