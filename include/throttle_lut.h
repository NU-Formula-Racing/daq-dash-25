#pragma once
#ifndef __THROTTLE_LUT__
#define __THROTTLE_LUT__

enum class InterpType : uint8_t {
    Linear = 0,
    SmoothStep = 1
};

struct ThrottleLUT {
    bool filesPresent;
    uint8_t numPairs;
    InterpType interpType;
    uint8_t LUTId;
    std::vector<int16_t> xVals;
    std::vector<float> yVals;
};

#endif // __THROTTLE_LUT__