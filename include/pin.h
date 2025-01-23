#include <map>
#include <string>
#include "virtualTimer.h"
#include <CAN.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

#define rawRGB24toRGB565(r, g, b) uint16_t((r / 8) << 11) | ((g / 4) << 5) | (b / 8)
#define NORTHWESTERN_PURPLE rawRGB24toRGB565(78, 42, 132)
