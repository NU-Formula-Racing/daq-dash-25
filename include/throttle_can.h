#pragma once
#ifndef __THROTTLE_CAN__
#define __THROTTLE_CAN__

#include "can/drive_bus.h"

class ThrottleCAN {
  public:
      ThrottleCAN(DriveBus& driveBus) : driveBus(driveBus) {}
      void initialize(ThrottleLUT LUT);
  private:
      DriveBus& driveBus;

};
#endif