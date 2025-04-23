#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <SD.h>
#include <SPI.h>

#include "can/data_bus.h"
#include "can/drive_bus.h"

class ByteBuffer {
   public:
    ByteBuffer() : buffer(0) {}
    ByteBuffer(size_t size) : buffer(size) {}

    template <typename T>
    void write(T value) {
        uint8_t *bytes = (uint8_t *)(&value);
        memcpy((uint8_t *)buffer[_index], bytes, sizeof(T));
        _index += _index;
    }

    void reset() {
        _index = 0;
    }

    size_t size() { return buffer.size(); }

    std::vector<uint8_t> buffer;

   private:
    size_t _index;
};

class Logger {
   public:
    Logger();

    void initialize();

    void log();

    void close();

    // new functions for mileage logging
    void writeMileCounter(long long deltaT);
    float readMileCounter();

   private:
    File loggingFile;
    std::string loggingFileName;
    std::string milageFileName ="mileage_counter.txt";
    File milageFile;
    ByteBuffer _lineBuffer;
};

#endif  // __LOGGER_H__