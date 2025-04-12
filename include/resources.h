#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "can/data_bus.h"
#include "can/drive_bus.h"
#include "logger.h"
#include "songs.h"
#include "sound.h"

class Resources {
   public:
    static Resources& instance() {
        static Resources r;
        return r;
    }

    static const DataBusData &dataBusData() {
        return instance().dataBus.getData();
    }

    static const DriveBusData &driveBusData() {
        return instance().driveBus.getData();
    }

    static const DriveBusData &prevDriveBusData() {
        return instance().driveBus.getPrevData();
    }

   private:
    Resources() : soundDriver(PIEZO_INPUT) {}

    void operator=(Resources const& other) = delete;

   public:
    DataBus dataBus;
    DriveBus driveBus;
    SoundDriver soundDriver;

    void update() {
        dataBus.update();
        driveBus.update();
    }
};

#endif  // __RESOURCES_H__