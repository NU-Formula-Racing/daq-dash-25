#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "can/data_bus.h"
#include "can/drive_bus.h"
#include "logger.h"
#include "songs.h"
#include "sound.h"
#include "throttle_can.h"

class Resources {
   public:
    // Returns the singleton instance.
    static Resources &instance();

    // Static accessor for DataBusData.
    static const DataBusData &dataBusData();

    // Static accessor for DriveBusData.
    static const DriveBusData &driveBusData();

    // Static accessor for the previous DriveBusData.
    static const DriveBusData &prevDriveBusData();

    static float deltaTimeMs();

   private:
    // Private constructor.
    Resources();

    // Delete assignment operator.
    void operator=(Resources const &other) = delete;

    long long _lastTime;
    long long _deltaTime;

   public:
    DataBus dataBus;
    DriveBus driveBus;
    SoundDriver soundDriver;
    Logger logger;
    float milageCounter;
    ThrottleCAN throttleCAN{driveBus};

    // Update both DataBus and DriveBus.
    void update();
};

#endif  // __RESOURCES_H__
