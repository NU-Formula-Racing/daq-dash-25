#ifndef __DATA_BUS_H__
#define __DATA_BUS_H__

#include <Arduino.h>
#include <CAN.h>

#include <string>

#include "virtualTimer.h"

#define NUM_TEMP_CELLS 79
#define NUM_VOLT_CELLS 139

struct DataBusData {
    float cellTemperatures[NUM_TEMP_CELLS];
    float cellVoltages[NUM_VOLT_CELLS];
};

class DataBus {
   public:
    const DataBusData &getData() const;

    void initialize();
    void update();

   private:
    DataBusData _data;
    TeensyCAN<2> _dataBus;
    VirtualTimerGroup timer_group{};

    MakeSignedCANSignal(float, 0, 16, 0.04432234432, 0) gen_amps_signal;
    MakeSignedCANSignal(float, 16, 16, 0.04432234432, 0) fan_amps_signal;
    MakeSignedCANSignal(float, 32, 16, 0.04432234432, 0) pump_amps_signal;
    CANRXMessage<3> rx_pdm_current{_dataBus, 0x290, gen_amps_signal, fan_amps_signal, pump_amps_signal};
}

#endif  // __DATA_BUS_H__