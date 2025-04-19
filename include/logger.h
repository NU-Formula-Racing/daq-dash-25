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
    void write_mile_counter(long long deltaT);
    float read_mile_counter();

   private:
    File _file;
    std::string logger_file_name = "log_" + std::to_string(day()) + "_" + std::to_string(month()) + "_" + std::to_string(year()) + ".csv";
    std::string milage_file_name ="mileage_counter.txt";
    File _file_mileage;
    ByteBuffer _lineBuffer;
};

#endif  // __LOGGER_H__