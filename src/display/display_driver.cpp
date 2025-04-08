#include "display/display_driver.h"

#include "display/primatives.h"
#include "display/texture.h"
#include "display/__dd_const.h"

bool DisplayDriver::begin() {
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

uint8_t _halReadReg(uint8_t reg) {

}