#pragma once
#ifndef __THROTTLE_LUT__
#define __THROTTLE_LUT__

#include <map>
#include "can/drive_bus.h"
#include "teensy_can.h"
#include "can/drive_bus.h"

enum class InterpType : uint8_t {
    Linear = 0,
    SmoothStep = 1
};

struct ThrottleLut {
    bool file_present;
    uint8_t num_pairs;
    InterpType interp_type;
    uint8_t lut_id;
    std::map<int16_t, float> lut;
};

class ThrottleCAN {
    public:
        ThrottleCAN(DriveBus &driveBus) : driveBus(driveBus) {}
    private:
        DriveBus &driveBus;

};

#endif // __THROTTLE_LUT__