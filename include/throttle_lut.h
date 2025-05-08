#pragma once
#ifndef __THROTTLE_LUT__
#define __THROTTLE_LUT__

enum class FileStatus : uint8_t {
    FILE_PRESENT_AND_VALID = 0,
    FILE_NOT_PRESENT = 1,
    INVALID_KEY_PHRASE = 2,
    LOGGING_ERROR = 3,
    FILE_OPEN_ERROR = 4
};

enum class InterpType : uint8_t {
    LINEAR = 0,
    SMOOTH_STEP = 1
};

struct ThrottleLUT {
    FileStatus fileStatus;
    uint8_t numPairs;
    InterpType interpType;
    uint8_t LUTId;
    std::vector<int16_t> xVals;
    std::vector<float> yVals;
};

#endif // __THROTTLE_LUT__