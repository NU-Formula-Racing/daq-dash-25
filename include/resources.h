#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "can/data_bus.h"
#include "can/drive_bus.h"
#include "sound/songs.h"
#include "sound/sound.h"

class Resources {
   public:
    static Resources& instance() {
        static Resources r;
        return r;
    }

    static const DriveBusData& driveData() {
        return Resources::instance().driveBus.getData();
    }

    static const DataBusData& dataData() {
        return Resources::instance().dataBus.getData();
    }

   private:
    Resources();
    void operator=(Resources const& other) = delete;

   public:
    DriveBus driveBus;
    DataBus dataBus;
};

#endif  // __RESOURCES_H__