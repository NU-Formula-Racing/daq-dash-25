#pragma once
#ifndef __THROTTLE_LUT__
#define __THROTTLE_LUT__

#include <map>
#include "can/drive_bus.h"

enum class InterpType_t : uint8_t {
    Linear = 0,
    SmoothStep = 1
};

struct throttle_lut_t {
    bool file_present;
    uint8_t num_pairs;
    InterpType_t interp_type;
    uint8_t lut_id;
    std::map<int16_t, float> lut;
};

#endif // __THROTTLE_LUT__