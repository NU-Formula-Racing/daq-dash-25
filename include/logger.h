#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <SD.h>
#include <SPI.h>
#include <map>

#include "can/data_bus.h"
#include "can/drive_bus.h"
#include "throttle_lut.h"

class ByteBuffer
{
public:
    ByteBuffer() : buffer(0) {}
    ByteBuffer(size_t size) : buffer(size), _size(size) {}

    template <typename T>
    void write(T value)
    {
        // Serial.printf("Writing %d bytes!\n", sizeof(T));
        memcpy(&(buffer[_index]), (void *)(&value), sizeof(T));
        _index += _index;
    }

    void reset()
    {
        _index = 0;
    }

    size_t size() { return _size; }

    std::vector<uint8_t> buffer;

private:
    size_t _index;
    size_t _size;
};

class Logger
{
public:
    Logger();

    void initialize();

    void log();

    // new functions for mileage logging
    void writeMileCounter();
    float readMileCounter();

    // functions for lut
    ThrottleLut readThrottleLUT();

private:
    File loggingFile;
    std::string loggingFileName;
    std::string milageFileName = "mileage_counter.txt";
    File milageFile;
    std::string lutFileName = "throttle_lut.txt";
    File lutFile;
    ByteBuffer _lineBuffer;
    bool _loggerGood = false;
};

#endif // __LOGGER_H__