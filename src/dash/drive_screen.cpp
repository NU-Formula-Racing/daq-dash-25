#include "dash/drive_screen.h"

#include "dash/drawer.h"
#include "define.h"
#include "resources.h"

static const int drive_state_startX = SCREEN_WIDTH / 4;
static const int drive_state_startY = SCREEN_HEIGHT / 3;
static const int error_state_startX = SCREEN_WIDTH * 0.4;
static const int error_state_startY = SCREEN_HEIGHT * 6 / 8;
static const int hv_bat_volt_startX = SCREEN_WIDTH / 8;
static const int hv_bat_volt_startY = SCREEN_HEIGHT / 4 + 30;
static const int lv_bat_volt_startX = SCREEN_WIDTH / 8;
static const int lv_bat_volt_startY = SCREEN_HEIGHT * 3 / 4 + 30;
static const int wheel_speed_startX = SCREEN_WIDTH / 2 + 40;
static const int wheel_speed_startY = SCREEN_HEIGHT * 0.34;
static const int max_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
static const int max_cell_temp_startY = SCREEN_HEIGHT / 4 + 30;
static const int min_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
static const int min_cell_temp_startY = SCREEN_HEIGHT * 3 / 4 + 30;
static const int handle_error_startX = 0;  // /4
static const int handle_error_startY = SCREEN_HEIGHT / 3;

// Draws drive state on screen based on CAN signal
// Draws drive state on screen based on CAN signal
static void drawDriveState(Adafruit_RA8875 tft) {
    // dont need wheel speed start x y anymore i think
    int16_t color = INDIAN_RED;
    int startX = drive_state_startX;
    int startY = drive_state_startY;
    int driveRectw = startX * 2;
    int driveRecth = startY;

    switch (Resources::driveBusData().driveState) {
        case 0:
            color = INDIAN_RED;
            break;
        case 1:
            color = GOLD;
            break;
        case 2:
            color = FERN_GREEN;
            break;
        default:
            color = INDIAN_RED;
            break;
    }
    tft.fillRect(startX, startY, driveRectw, driveRecth, color);

    // change sizes via if statement
    switch (Resources::driveBusData().driveState) {
        case 0:
            Drawer::drawString(tft, "OFF", startX * 4 * 0.45, startY * 3 * 0.58, 5, RA8875_WHITE, color);
            break;
        case 1:
            Drawer::drawString(tft, "NEUTRAL", startX * 4 * 0.38, startY * 3 * 0.58, 5, RA8875_BLACK, color);
            break;
        case 2:
            Drawer::drawString(tft, "DRIVE", startX * 4 * 0.4, startY * 3 * 0.58, 5, RA8875_WHITE, color);
            break;
        default:
            Drawer::drawString(tft, "ERROR", startX * 4 * 0.45, startY * 3 * 0.58, 5, RA8875_WHITE, color);
            break;
    }


}

void DriveScreen::draw(Adafruit_RA8875 tft) {
    tft.fillScreen(NORTHWESTERN_PURPLE);

    // write circle labels
    int16_t color = RA8875_WHITE;
    Drawer::drawString(tft, "HV Battery", hv_bat_volt_startX * 0.15, hv_bat_volt_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "Voltage", hv_bat_volt_startX * 0.4, hv_bat_volt_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "LV Battery", lv_bat_volt_startX * 0.15, lv_bat_volt_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "Voltage", lv_bat_volt_startX * 0.4, lv_bat_volt_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "Max Cell", max_cell_temp_startX * 0.9, max_cell_temp_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "Temp", max_cell_temp_startX * 0.95, max_cell_temp_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "Min Cell", min_cell_temp_startX * 0.9, min_cell_temp_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_WHITE, color);
    Drawer::drawString(tft, "Temp", min_cell_temp_startX * 0.95, min_cell_temp_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_WHITE, color);
}

void DriveScreen::update(Adafruit_RA8875 tft) {
    // if (this->prev_drive_state != curr_drive_state || FORCE_DRAW) {
    //     drawDriveState(tft, drive_state_startX, drive_state_startY, curr_drive_state, 8, avg_wheel_speed, wheel_speed_startX, wheel_speed_startY, ifErrorScreen);
    //     this->prev_drive_state = curr_drive_state;
    // }
    // // this->prev_wheel_speed = avg_wheel_speed;
    // if (this->prev_hv_bat_volt != hv_bat_volt || FORCE_DRAW) {
    //     DrawState(tft, hv_bat_volt_startX, hv_bat_volt_startY, hv_bat_volt, 8, hv_battery_voltage_mid_state, hv_battery_voltage_last_state);
    //     this->prev_hv_bat_volt = hv_bat_volt;
    // }
    // if (this->prev_lv_bat_volt != lv_bat_volt || FORCE_DRAW) {
    //     DrawState(tft, lv_bat_volt_startX, lv_bat_volt_startY, lv_bat_volt, 8, lv_battery_voltage_mid_state, lv_battery_voltage_last_state);
    //     this->prev_lv_bat_volt = lv_bat_volt;
    // }
    // if (this->prev_max_cell_temp != max_cell_temp || FORCE_DRAW) {
    //     DrawState(tft, max_cell_temp_startX, max_cell_temp_startY, max_cell_temp, 8, max_cell_temp_mid_state, max_cell_temp_last_state);
    //     this->prev_max_cell_temp = max_cell_temp;
    // }
    // if (this->prev_min_cell_temp != min_cell_temp || FORCE_DRAW) {
    //     DrawState(tft, min_cell_temp_startX, min_cell_temp_startY, min_cell_temp, 8, min_cell_temp_mid_state, min_cell_temp_last_state);
    //     this->prev_min_cell_temp = min_cell_temp;
    // }
}