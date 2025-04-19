#ifndef __DRIVE_SCREEN_H__
#define __DRIVE_SCREEN_H__

#include <Adafruit_RA8875.h>
#include "screen.h"

static const float wheel_diameter = 16; // inches ** might move later?
float cur_wheel_speed = Resources::driveBusData().averageWheelSpeed();
long long last_time = 0;
long long cur_time = 0;
long long deltaT = 0;

float mileageNum = (cur_wheel_speed * (wheel_diameter * M_PI) * millis()) / 63360; // mileage

class DriveScreen : public Screen {
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);
};

#endif // __DRIVE_SCREEN_H__