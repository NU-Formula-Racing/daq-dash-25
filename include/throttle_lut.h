#pragma once
#ifndef __THROTTLE_LUT__
#define __THROTTLE_LUT__

#include <map>
#include "teensy_can.h"
#include "can/drive_bus.h"

enum class InterpType : uint8_t {
    Linear = 0,
    SmoothStep = 1
};

struct ThrottleLut {
    bool files_present;
    uint8_t num_pairs;
    InterpType interp_type;
    uint8_t lut_id;
    std::vector<int16_t> x_vals;
    std::vector<float> y_vals;
};

class ThrottleCAN {
    public:
        ThrottleCAN(DriveBus& driveBus) : driveBus(driveBus) {}
        void initialize(ThrottleLut lut);
    private:
        DriveBus& driveBus;

};

#endif // __THROTTLE_LUT__