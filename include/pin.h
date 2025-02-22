#ifndef __PIN_H__
#define __PIN_H__

#ifdef ARDUINO_TEENSY41
#include "pin_teensy41.h"
#endif

#ifdef ARDUINO_ARCH_ESP32
#include "pin_esp32.h"
#endif

#endif // __PIN_H__