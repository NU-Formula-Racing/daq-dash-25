#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <SD.h>
#include <SPI.h>

#include "can/data_bus.h"
#include "can/drive_bus.h"

class ByteBuffer {
   public:
    ByteBuffer() : buffer(0), _index(0) {}
    ByteBuffer(size_t size) : buffer(size), _size(size) {}

    template <typename T>
    void writeValue(T value) {
        // Serial.printf("Writing %d bytes!\n", sizeof(T));
        memcpy(&(buffer[_index]), (void *)(&value), sizeof(T));
        _index += sizeof(T);
    }

    template <typename T>
    void writeFromPtr(T *value) {
        // Serial.printf("Writing %d bytes!\n", sizeof(T));
        memcpy(&(buffer[_index]), (void *)(value), sizeof(T));
        _index += sizeof(T);
    }

    void reset() {
        _index = 0;
    }

    size_t size() { return _size; }

    std::vector<uint8_t> buffer;

   private:
    size_t _index;
    size_t _size;
};

enum LoggerStatus {
    UNABLE_TO_LOG,
    LOGGING
};

class Logger {
   public:
    Logger();

    void initialize();

    void log();

    LoggerStatus status() const;

    std::string logFileName() const;

    // new functions for mileage logging
    void writeMileCounter();
    float readMileCounter();

   private:
    File loggingFile;
    std::string loggingFileName;
    std::string milageFileName = "mileage_counter.txt";
    File milageFile;
    ByteBuffer _lineBuffer;
    LoggerStatus _status;
};

#endif  // __LOGGER_H__