#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "can/data_bus.h"
#include "can/drive_bus.h"
#include "logger.h"
#include "songs.h"
#include "sound.h"

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

   private:
    // Private constructor.
    Resources();

    // Delete assignment operator.
    void operator=(Resources const &other) = delete;

   public:
    DataBus dataBus;
    DriveBus driveBus;
    SoundDriver soundDriver;

    // Update both DataBus and DriveBus.
    void update();
};

#endif  // __RESOURCES_H__
