#ifndef __PIN_H__
#define __PIN_H__

#include <Arduino.h>
#include <stdint.h>
//pins_arduino.h
// #define PIN_SPI_SS    (10)
// #define PIN_SPI_MOSI  (11)
// #define PIN_SPI_MISO  (12)
// #define PIN_SPI_SCK   (13)
// static const uint8_t SS = 10;
// static const uint8_t MOSI = 11;
// static const uint8_t MISO = 12;
// static const uint8_t SCK = 13;

//dash.h
#define RA8875_INT 21
#define RA8875_WAIT 20
#define RA8875_RESET 19
#define RA8875_CS PIN_SPI_SS
#define RA8875_LITE 18
#define PIEZO_INPUT A8

#define INTERNAL_LED 13
#define INDICATOR_LED 17 //is this right?


//other pins
// drive can tx   1
// drive can rx    0
#endif // __PIN_H__