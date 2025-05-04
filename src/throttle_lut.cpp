#include "throttle_lut.h"

void ThrottleCAN::initialize(ThrottleLut lut) {
    driveBus.sendLUTCan(lut.files_present, lut.num_pairs, static_cast<uint8_t>(lut.interp_type), lut.lut_id, lut.x_vals, lut.y_vals);
}