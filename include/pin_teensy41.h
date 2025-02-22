#include <stdint.h>

//pins_arduino.h
#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)
static const uint8_t SS = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK = 13;

//dash.h
#define RA8875_INT 3
#define RA8875_WAIT 2
#define RA8875_RESET 4
#define RA8875_CS 5

//other pins
// drive can tx   1
//drive can rx    0