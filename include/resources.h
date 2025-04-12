#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "can/data_bus.h"
#include "can/drive_bus.h"
#include "logger.h"

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
};

#endif  // __RESOURCES_H__