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
static const int wheel_speed_startX = SCREEN_WIDTH / 2;
static const int wheel_speed_startY = SCREEN_HEIGHT / 2;
static const int max_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
static const int max_cell_temp_startY = SCREEN_HEIGHT / 4 + 30;
static const int min_cell_temp_startX = SCREEN_WIDTH * 7 / 8;
static const int min_cell_temp_startY = SCREEN_HEIGHT * 3 / 4 + 30;
static const int handle_error_startX = 0;  // /4
static const int handle_error_startY = SCREEN_HEIGHT / 3;

static uint16_t getDriveStateColor() {
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
    uint16_t color = getDriveStateColor();

    Drawer::drawRect(tft, (RectDrawOptions){
                              .x = SCREEN_WIDTH / 2,
                              .y = SCREEN_HEIGHT / 2,
                              .width = 350,
                              .height = 250,
                              .fill = true,
                              .strokeThickness = 10,
                              .strokeColor = RA8875_WHITE,
                              .fillColor = color,
                              .cornerRadius = 6,
                              .hAlign = ALIGN_CENTER,
                              .vAlign = ALIGN_MIDDLE,
                          });

    // change sizes via if statement
    std::string driveString = "";
    switch (Resources::driveBusData().driveState) {
        case 0:
            driveString = "OFF";
            break;
        case 1:
            driveString = "NEUTRAL";
            break;
        case 2:
            driveString = "ON";
            break;
        default:
            driveString = "ERROR";
            break;
    }

    Drawer::drawString(tft, driveString,
        (TextDrawOptions) {
            .x = SCREEN_WIDTH / 2,
            .y = SCREEN_HEIGHT / 2 + 60,
            .size = 8,
            .color = RA8875_WHITE,
            .backgroundColor = color,
            .hAlign = ALIGN_CENTER,
            .vAlign = ALIGN_MIDDLE,
        });
}

static void drawWheelSpeed(Adafruit_RA8875 tft) {
    Drawer::drawNum(tft, Resources::driveBusData().averageWheelSpeed(),
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH / 2,
                        .y = SCREEN_HEIGHT / 2 - 40,
                        .size = 10,
                        .color = RA8875_WHITE,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
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