#include "logger.h"

#include <TimeLib.h>

#include <memory>
#include <string>

#include "dash/dash.h"
#include "define.h"
#include "resources.h"

const int chipSelect = BUILTIN_SDCARD;

time_t getTeensy3Time() {
    return Teensy3Clock.get();
}

Logger::Logger() {
    // calculate the size of the data
    size_t timeSize = sizeof(uint32_t);  // millis

    _lineBuffer = ByteBuffer(timeSize + sizeof(DataBusData) + sizeof(DriveBusData));
}

void Logger::initialize() {
    Resources::instance().driveBus.initialize();
    Resources::instance().dataBus.initialize();
    setSyncProvider(getTeensy3Time);

    Serial.println("Initializing Logger!");

    if (!SD.begin(chipSelect)) {
        Serial.println("SD initalization failed.");
        _status = LoggerStatus::UNABLE_TO_LOG;
        return;
    }

    Serial.println("SD card initialized.");
    _status = LoggerStatus::LOGGING;

    char fname[16];  // "log_" + 3-digits + ".bin" + '\0'
    uint16_t index = 0;

    do {
        snprintf(fname, sizeof(fname), "log_%03u.bin", index);
        ++index;
    } while (SD.exists(fname) && index < 1000);  // stop at 999 just in case

    loggingFileName = fname;
    Serial.print("Next log file: ");
    Serial.println(loggingFileName.c_str());

    _status = LoggerStatus::LOGGING;

    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);

    /// write the preamble
    this->loggingFile.write('N');
    this->loggingFile.write('F');
    this->loggingFile.write('R');
    this->loggingFile.write('2');
    this->loggingFile.write('5');
    this->loggingFile.write((uint8_t)0);
    this->loggingFile.write((uint8_t)0);
    this->loggingFile.write((uint8_t)1);
    this->loggingFile.write(_lineBuffer.size());
}

void Logger::log() {
    if (_status == LoggerStatus::UNABLE_TO_LOG) return;

    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);

    this->loggingFile.seek(this->loggingFile.size());

    DriveBusData driveData = Resources::instance().driveBus.getData();
    DataBusData dataData = Resources::instance().dataBus.getData();

    _lineBuffer.reset();

    // write the time
    _lineBuffer.writeValue(millis());
    _lineBuffer.writeFromPtr(&driveData);
    _lineBuffer.writeFromPtr(&dataData);

    this->loggingFile.write(_lineBuffer.buffer.data(), _lineBuffer.size());

    this->loggingFile.close();
}

LoggerStatus Logger::status() const {
    return _status;
}

std::string Logger::logFileName() const {
    return loggingFileName;
}

void Logger::writeMileCounter() {
    if (_status == LoggerStatus::UNABLE_TO_LOG) return;

    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE_BEGIN);
    String counter = "";
    counter.append(Resources::instance().milageCounter);
    this->milageFile.write(counter.c_str());
    this->milageFile.close();
    // reopen old logger file
    this->loggingFile = SD.open(loggingFileName.c_str(), FILE_WRITE);
}

// returns current mileage
float Logger::readMileCounter() {
    if (_status == LoggerStatus::UNABLE_TO_LOG) return 0;

    // open mileage file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_READ);
    float miles = 0;
    // if empty, read as 0?
    if (this->milageFile && this->milageFile.size() == 0) {
        return 0;
    } else {                                                           // else, read
        this->milageFile.seek(0);                                      // Go to the start of the file
        String numberString = this->milageFile.readStringUntil('\n');  // or '\r' or any delimiter
        miles = numberString.toFloat();                                // or .toInt() for integers
    }
    // close mileage file
    this->milageFile.close();
    // reopen old logger file
    this->milageFile = SD.open(milageFileName.c_str(), FILE_WRITE);
    // returns current mileage
    return miles;
}

// every two seconds, update mileage counter
