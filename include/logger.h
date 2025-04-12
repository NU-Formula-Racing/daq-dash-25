#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <SD.h>
#include <SPI.h>

#include "can/data_bus.h"
#include "can/drive_bus.h"

class Logger {
   public:
    void initialize();

    void log();

    void close();

   private:
    File file;
};

#endif  // __LOGGER_H__