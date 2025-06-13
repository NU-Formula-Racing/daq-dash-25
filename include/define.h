#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "pin.h"

#define toRGB(r, g, b) uint16_t((r / 8) << 11) | ((g / 4) << 5) | (b / 8)

#define NORTHWESTERN_PURPLE toRGB(78, 42, 132)
#define PURPLE_130 toRGB(38, 8, 65)
#define RUSSIAN_VIOLET toRGB(49, 20, 82)
#define GOLD toRGB(207, 161, 62)
#define ROSE toRGB(129, 83, 85)
#define GREEN toRGB(100, 120, 84)
#define FERN_GREEN toRGB(70, 109, 38)
#define INDIAN_RED toRGB(201, 42, 69)
#define OFF_BLACK toRGB(24, 4, 41)

// #define KAWAII_PINK toRGB(248, 152, 178)
#define BACKGROUND_GREEN toRGB(159, 243, 123)
#define KAWAII_YELLOW toRGB(245, 220, 81)
#define BACKGROUND_PURPLE toRGB(167, 107, 235)
#define KAWAII_PINK toRGB(237, 52, 160)
#define KAWAII_GREEN toRGB(66, 199, 46)

// #define KAWAII_YELLOW toRGB(245, 225, 5)
#define KAWAII_PURPLE toRGB(143, 2, 236)
#define BACKGROUND_GRAY toRGB(25, 10, 27)
#define KAWAII_BLUE toRGB(71, 172, 203)
#define KAWAII_BLACK toRGB(8, 2, 5)


#define GOTH_RED toRGB(140, 6, 51)
#define GOTH_GREEN toRGB(6, 128, 29)
#define GOTH_BLUE toRGB(46, 35, 145)
#define GOTH_WHITE toRGB(237, 235, 211)
#define CARBON_FIBER_BLACK toRGB(20, 20, 20)
#define CARBON_FIBER_GREY toRGB(50, 50, 50)


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

// #define DRIVE_DEBUG
// #define DATA_DEBUG

#endif  // __DEFINE_H__