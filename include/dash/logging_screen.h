#ifndef __LOGGING_SCREEN_H__
#define __LOGGING_SCREEN_H__

#include <Adafruit_RA8875.h>
#include "screen.h"
#include "resources.h"


class LoggingScreen : public Screen {
    public:
     void draw(Adafruit_RA8875 tft);
     void update(Adafruit_RA8875 tft, bool force = false);

    private:
    // defined these variables here for now; not sure if it's the best
     // place to do so, will change in the future if not
     RotaryEncoder rotaryEncoder;  // Declare the RotaryEncoder object
     std::vector <std::string> labels = {"Acceleration", "Skid Pad", "Endurance", "General Testing"};
     int highlighted = 0;
     bool accelSelected;
     bool skidSelected;
     bool endureSelected;
     bool genSelected;

     // *** highlighted will need to be modified via code from rotary_encoder.h
     // might need to re-define in the future?
};

#endif // LOGGING_SCREEN