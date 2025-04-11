#include <SD.h>
#include <SPI.h>

#include "can/drive_bus.h"
#include "can/data_bus.h"

class Logger {
    public:
    void initialize();

    void log();

    void close();

    private:
    DriveBus drive_bus;
    DataBus data_bus;
    File file;
};