#include "display/display_driver.h"

#include "display/__dd_const.h"
#include "display/primatives.h"
#include "display/texture.h"

bool DisplayDriver::begin() {
    pinMode(_options.pinCs, OUTPUT);
    digitalWrite(_options.pinCs, HIGH);

    pinMode(_options.pinRst, OUTPUT);
    digitalWrite(_options.pinRst, LOW);
    delay(100);
    digitalWrite(_options.pinRst, HIGH);
    delay(100);

    _spi.beginTransaction(_spiSettings);
    uint8_t val = _halReadReg(0x0);
    // we expect the value to be a specific value
    if (val != 0x75) {
        return false;
    }
    _spi.endTransaction();

    // now initialize the screen by writing some initial settings
    /* Timing values */
    uint8_t pixclk;
    uint8_t hsync_start;
    uint8_t hsync_pw;
    uint8_t hsync_finetune;
    uint8_t hsync_nondisp;
    uint8_t vsync_pw;
    uint16_t vsync_nondisp;
    uint16_t vsync_start;

    /* Set the correct values for the display being used */
    if (_size == DD_480x80) {
        pixclk = DD_PCSR_PDATL | DD_PCSR_4CLK;
        hsync_nondisp = 10;
        hsync_start = 8;
        hsync_pw = 48;
        hsync_finetune = 0;
        vsync_nondisp = 3;
        vsync_start = 8;
        vsync_pw = 10;
        _voffset = 192;  // This uses the bottom 80 pixels of a 272 pixel controller
    } else if (_size == DD_480x128 || _size == DD_480x272) {
        pixclk = DD_PCSR_PDATL | DD_PCSR_4CLK;
        hsync_nondisp = 10;
        hsync_start = 8;
        hsync_pw = 48;
        hsync_finetune = 0;
        vsync_nondisp = 3;
        vsync_start = 8;
        vsync_pw = 10;
        _voffset = 0;
    } else  // (_size == DD_800x480)
    {
        pixclk = DD_PCSR_PDATL | DD_PCSR_2CLK;
        hsync_nondisp = 26;
        hsync_start = 32;
        hsync_pw = 96;
        hsync_finetune = 0;
        vsync_nondisp = 32;
        vsync_start = 23;
        vsync_pw = 2;
        _voffset = 0;
    }

    _halWriteReg(DD_PCSR, pixclk);
    delay(1);

    /* Horizontal settings registers */
    _halWriteReg(DD_HDWR, (_options.width / 8) - 1);  // H width: (HDWR + 1) * 8 = 480
    _halWriteReg(DD_HNDFTR, DD_HNDFTR_DE_HIGH + hsync_finetune);
    _halWriteReg(DD_HNDR, (hsync_nondisp - hsync_finetune - 2) /
                              8);                // H non-display: HNDR * 8 + HNDFTR + 2 = 10
    _halWriteReg(DD_HSTR, hsync_start / 8 - 1);  // Hsync start: (HSTR + 1)*8
    _halWriteReg(DD_HPWR,
                 DD_HPWR_LOW +
                     (hsync_pw / 8 - 1));  // HSync pulse width = (HPWR+1) * 8

    /* Vertical settings registers */
    _halWriteReg(DD_VDHR0, (uint16_t)(_options.height - 1 + _voffset) & 0xFF);
    _halWriteReg(DD_VDHR1, (uint16_t)(_options.height - 1 + _voffset) >> 8);
    _halWriteReg(DD_VNDR0, vsync_nondisp - 1);  // V non-display period = VNDR + 1
    _halWriteReg(DD_VNDR1, vsync_nondisp >> 8);
    _halWriteReg(DD_VSTR0, vsync_start - 1);  // Vsync start position = VSTR + 1
    _halWriteReg(DD_VSTR1, vsync_start >> 8);
    _halWriteReg(DD_VPWR,
                 DD_VPWR_LOW + vsync_pw - 1);  // Vsync pulse width = VPWR + 1

    /* Set active window X */
    _halWriteReg(DD_HSAW0, 0);  // horizontal start point
    _halWriteReg(DD_HSAW1, 0);
    _halWriteReg(DD_HEAW0, (uint16_t)(_options.width - 1) & 0xFF);  // horizontal end point
    _halWriteReg(DD_HEAW1, (uint16_t)(_options.width - 1) >> 8);

    /* Set active window Y */
    _halWriteReg(DD_VSAW0, 0 + _voffset);  // vertical start point
    _halWriteReg(DD_VSAW1, 0 + _voffset);
    _halWriteReg(DD_VEAW0,
                 (uint16_t)(_options.height - 1 + _voffset) & 0xFF);  // vertical end point
    _halWriteReg(DD_VEAW1, (uint16_t)(_options.height - 1 + _voffset) >> 8);

    /* ToDo: Setup touch panel? */

    /* Clear the entire window */
    _halWriteReg(DD_MCLR, DD_MCLR_START | DD_MCLR_FULL);
    delay(500);
}

void DisplayDriver::reset() {
}

void DisplayDriver::toggleDisplay(bool on) {
}

void DisplayDriver::draw(const DisplayPrimative &primative) {
}

void DisplayDriver::_halDrawLine(const DisplayPrimative &prim) {
}

void DisplayDriver::_halDrawSquare(const DisplayPrimative &prim) {
}

void DisplayDriver::_halDrawCircle(const DisplayPrimative &prim) {
}

void DisplayDriver::_halDrawTri(const DisplayPrimative &prim) {
}

void DisplayDriver::_halDrawEllipse(const DisplayPrimative &prim) {
}

void DisplayDriver::_halDrawRoundedSquare(const DisplayPrimative &prim) {
}

void DisplayDriver::_halWriteReg(uint8_t reg, uint8_t val) {
    digitalWrite(_options.pinCs, LOW);

    _spi.beginTransaction(_spiSettings);

    // write the register
    _spi.transfer(DD_CMDWRITE);
    _spi.transfer(reg);

    // imbetween
    digitalWrite(_options.pinCs, HIGH);
    digitalWrite(_options.pinCs, LOW);

    // write the value
    _spi.transfer(DD_CMDWRITE);
    _spi.transfer(val);

    _spi.endTransaction();

    digitalWrite(_options.pinCs, HIGH);
}

uint8_t DisplayDriver::_halReadReg(uint8_t reg) {
    digitalWrite(_options.pinCs, LOW);

    _spi.beginTransaction(_spiSettings);

    // write the register
    _spi.transfer(DD_CMDWRITE);
    _spi.transfer(reg);

    // imbetween
    digitalWrite(_options.pinCs, HIGH);
    digitalWrite(_options.pinCs, LOW);

    // write the value
    _spi.transfer(DD_DATAREAD);
    uint8_t val = _spi.transfer(0x0);

    _spi.endTransaction();

    digitalWrite(_options.pinCs, HIGH);

    return val;
}