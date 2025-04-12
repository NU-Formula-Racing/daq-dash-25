#include <CAN.h>

#include <map>
#include <string>

#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "define.h"
#include "virtualTimer.h"

#define DEFAULT_VALUE -100
#define DEFAULT_VALUE_UNSIGNED 255

class Dash
{
public:

    void GetCAN();
    void Initialize();
    void UpdateDisplay(Adafruit_RA8875 tft);

private:

    uint8_t prev_drive_state = DEFAULT_VALUE_UNSIGNED;
    float prev_wheel_speed = DEFAULT_VALUE;
    float prev_hv_bat_volt = DEFAULT_VALUE;
    float prev_lv_bat_volt = DEFAULT_VALUE;
    float prev_max_cell_temp = DEFAULT_VALUE;
    float prev_min_cell_temp = DEFAULT_VALUE;
    uint8_t bms_faults = 0;
    uint8_t prev_bms_faults = 0;
    int16_t backgroundColor = NORTHWESTERN_PURPLE;

    // new variables
    uint8_t ecu_faults = 0;
    uint8_t prev_ecu_faults = 0;
    uint8_t inverter_faults = 0;
    uint8_t prev_inverter_faults = 0;

    // prev variables for UpdateVariable
    std::string imd_prev_err_str = "";
};
