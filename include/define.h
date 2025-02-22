#ifndef __DEFINE_H__
#define __DEFINE_H__

#define rawRGB24toRGB565(r, g, b) uint16_t((r / 8) << 11) | ((g / 4) << 5) | (b / 8)
#define NORTHWESTERN_PURPLE rawRGB24toRGB565(78, 42, 132)

#include "pin.h"


#endif // __DEFINE_H__