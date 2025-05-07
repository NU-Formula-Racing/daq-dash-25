#include "throttle_can.h"

void ThrottleCAN::initialize(ThrottleLUT LUT) {
    driveBus.sendLUTCan(LUT);
}