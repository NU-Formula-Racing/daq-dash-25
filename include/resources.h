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

   private:
    Resources();
    void operator=(Resources const& other) = delete;

   public:
    DataBus dataBus;
    DriveBus driveBus;
    SoundDriver soundDriver;
};

#endif  // __RESOURCES_H__