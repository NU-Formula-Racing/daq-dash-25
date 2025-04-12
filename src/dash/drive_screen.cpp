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

static int16_t getDriveStateColor() {
    switch (Resources::driveBusData().driveState) {
        case 0:
            return INDIAN_RED;
        case 1:
            return GOLD;
        case 2:
            return FERN_GREEN;
        default:
            return INDIAN_RED;
    }
}

// Draws drive state on screen based on CAN signal
static void drawDriveState(Adafruit_RA8875 tft) {
    // dont need wheel speed start x y anymore i think
    int16_t color = getDriveStateColor();
    int startX = drive_state_startX;
    int startY = drive_state_startY;
    int driveRectw = startX * 2;
    int driveRecth = startY;

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

static void drawWheelSpeed(Adafruit_RA8875 tft) {
    // Drawer::drawNum(tft, Resources::driveBusData().averageWheelSpeed(),
    //         (NumberDrawOptions) {
    //             .x = wheel_speed_startX,
    //             .y = wheel_speed_startY,
    //             .size = 20,
    //             .color = RA8875_WHITE,
    //             .backgroundColor = getDriveStateColor(),
    //             .alignment = CENTER
    //         }
    // );
}

void DriveScreen::draw(Adafruit_RA8875 tft) {
    Serial.print("Drawing DriveScreen!");
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

void DriveScreen::update(Adafruit_RA8875 tft, bool force) {
    if (Resources::driveBusData().driveState != Resources::prevDriveBusData().driveState || force)
        drawDriveState(tft);
    if (Resources::driveBusData().averageWheelSpeed() != Resources::prevDriveBusData().averageWheelSpeed() || force)
        drawWheelSpeed(tft);
}