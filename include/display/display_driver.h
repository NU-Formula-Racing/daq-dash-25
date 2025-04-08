#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__

#include <SPI.h>
#include <stdint.h>

#include <array>

#include "display/primatives.h"
#include "display/texture.h"

struct DisplayDriverOptions {
    uint8_t pinCs;
    uint8_t pinRst;
    uint16_t width;
    uint16_t height;
    uint32_t clockSpeed;
    SPIClass spi;
};

class DisplayDriver {
   public:
    DisplayDriver(DisplayDriverOptions options) : _options(options), 
        _spiSettings(options.clockSpeed, MSBFIRST, SPI_MODE0),
        _spi(options.spi)
    {}

    // basic functions
    bool
    begin();
    void reset();
    void toggleDisplay(bool on);

    // draw functions
    void draw(const DisplayPrimative& primative);

   private:
    const DisplayDriverOptions _options;
    SPIClass _spi;
    SPISettings _spiSettings;

    void _halDrawLine(const DisplayPrimative& prim);
    void _halDrawSquare(const DisplayPrimative& prim);
    void _halDrawCircle(const DisplayPrimative& prim);
    void _halDrawTri(const DisplayPrimative& prim);
    void _halDrawEllipse(const DisplayPrimative& prim);
    void _halDrawRoundedSquare(const DisplayPrimative& prim);

    void _halWriteReg(uint8_t reg, uint8_t val);
    uint8_t _halReadReg(uint8_t reg);
};

#endif  // __DISPLAY_DRIVER_H__