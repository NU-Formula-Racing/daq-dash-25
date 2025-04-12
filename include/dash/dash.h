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

    enum Error
    {
        NO_ERROR,
        BMS_FAULT,
        IMD_FAULT,
        ECU_FAULT,
        INVERTER_FAULT
    };

    enum Direction
    {
        LEFT_TO_RIGHT,
        UP_TO_DOWN
    };

    char mode;
    uint16_t index;
    int error_banner;

    void GetCAN();
    void Initialize();
    void UpdateDisplay(Adafruit_RA8875 tft);

    void DrawBackground(Adafruit_RA8875 tft, int16_t color = RUSSIAN_VIOLET);
    void DrawBar(Adafruit_RA8875 tft, std::string barName, float newValue, int16_t barColor, int16_t backgroundColor);
    float WheelSpeedAvg(float fl_wheel_speed, float fr_wheel_speed);
    void DrawDriveState(Adafruit_RA8875 tft, int startX, int startY, uint8_t curr_drive_state, int squareSize, float wheel_speed, int wheel_speed_startX, int wheel_speed_startY, bool ifErrorScreen);
    void Dash::DrawState(Adafruit_RA8875 tft, int startX, int startY, int display_value, int squareSize, int laststate, int midstate);

    void DrawIMDStatus(Adafruit_RA8875 tft, int startX, int startY, int imd_status, int squareSize);
    void HandleError(Adafruit_RA8875 tft, int startX, int startY);
    void DrawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir = LEFT_TO_RIGHT);
    void HandleBMSFaults(Adafruit_RA8875 tft, int startX, int startY);
    void HandleECUFaults(Adafruit_RA8875 tft, int startX, int startY);
    void HandleInverterFaults(Adafruit_RA8875 tft, int startX, int startY);
    void DrawErrorState(Adafruit_RA8875 tft, int startX, int startY, uint8_t curr_drive_state, int squareSize, float wheel_speed, int wheel_speed_startX, int wheel_speed_startY, bool ifErrorScreen);

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
    // std::string bms_prev_err_str = "";
    // std::string inverter_prev_err_str = "";
    // std::string ecu_prev_err_str = "";

    int CalcBarHeight(float value, float min, float max, int maxHeight);
    int CalcBarWidth(float value, float min, float max, int maxWidth);
    void RecordBMSFaults();
};
