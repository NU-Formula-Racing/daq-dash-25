#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "pin.h"

#define rawRGB24toRGB565(r, g, b) uint16_t((r / 8) << 11) | ((g / 4) << 5) | (b / 8)

#define NORTHWESTERN_PURPLE rawRGB24toRGB565(78, 42, 132)
#define PURPLE_130 rawRGB24toRGB565(38, 8, 65)
#define RUSSIAN_VIOLET rawRGB24toRGB565(49, 20, 82)
#define GOLD rawRGB24toRGB565(177, 149, 61)
#define ROSE rawRGB24toRGB565(129, 83, 85)
#define GREEN rawRGB24toRGB565(100, 120, 84)
#define FERN_GREEN rawRGB24toRGB565(70, 109, 38)
#define INDIAN_RED rawRGB24toRGB565(201, 42, 69)
#define OFF_BLACK rawRGB24toRGB565(24, 4, 41)
// #define KAWAII_PINK rawRGB24toRGB565(248, 152, 178)
#define BACKGROUND_GREEN rawRGB24toRGB565(159, 243, 123)
#define KAWAII_YELLOW rawRGB24toRGB565(245, 220, 81)
#define BACKGROUND_PURPLE rawRGB24toRGB565(167, 107, 235)
#define KAWAII_PINK rawRGB24toRGB565(237, 52, 160)
#define KAWAII_GREEN rawRGB24toRGB565(66, 199, 46)
// #define KAWAII_YELLOW rawRGB24toRGB565(245, 225, 5)
#define KAWAII_PURPLE rawRGB24toRGB565(143, 2, 236)
#define BACKGROUND_GRAY rawRGB24toRGB565(163, 162, 196)
#define KAWAII_BLUE rawRGB24toRGB565(71, 172, 203)
#define KAWAII_BLACK rawRGB24toRGB565(49, 48, 75)


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define MASK(x) (1 << x)

#define FORCE_DRAW false
#define DEBUG false

#define SP28_ANGLE 0
#define CLK_ROTARY 0
#define DT_ROTARY 0
#define SW_ROTARY 0

#define WHEEL_DIAMETER 16 // in inches

#define DRIVE_DEBUG


#endif  // __DEFINE_H__