#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__

#include "texture.h"
#include "primatives.h"

#include <stdint.h>

struct DisplayDriverOptions {
    uint8_t pinCs;
    uint8_t pinRst;
    uint16_t width;
    uint16_t height;
};

class DisplayDriver {
   public:
    DisplayDriver(DisplayDriverOptions options) : _options(options) {}

    // basic functions
    bool begin();
    void reset();
    void toggleDisplay(bool on);

    // draw functions
    void draw();

   private:
    const std::array<void(*)(const &DisplayPrimative), PRIM_COUNT> _halDrawFuncs = {
        _halDrawLine,
        _halDrawSquare,
        _halDrawTri,
        _halDrawEllipse,
        _halDrawRoundedSquare
    };
    
    void _halDrawLine(const &DisplayPrimative prim);
    void _halDrawSquare(const &DisplayPrimative prim);
    void _halDrawCircle(const &DisplayPrimative prim);
    void _halDrawTri(const &DisplayPrimative prim);
    void _halDrawEllipse(const &DisplayPrimative prim);
    void _halDrawRoundedSquare(const &DisplayPrimative prim);

    const DisplayDriverOptions _options;
};

#endif  // __DISPLAY_DRIVER_H__