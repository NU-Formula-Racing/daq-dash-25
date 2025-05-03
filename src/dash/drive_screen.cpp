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

// for states, after mid state, goes to last state
static const float motor_temp_last_state = 70;
static const float motor_temp_mid_state = 30;
static const float inverter_current_last_state = 100;
static const float inverter_current_mid_state = 50;
static const float min_voltage_last_state = 2.7;
static const float min_voltage_mid_state = 3.2;
static const float hv_battery_voltage_last_state = 3.4;
static const float hv_battery_voltage_mid_state = 3.2;
static const float lv_battery_voltage_last_state = 3.5;
static const float lv_battery_voltage_mid_state = 3.1;  // min 2.7
static const float max_cell_temp_last_state = 50;       // max 50 celsius
static const float max_cell_temp_mid_state = 45;
static const float min_cell_temp_last_state = 15;
static const float min_cell_temp_mid_state = 11;  // min 8 celsius
float prev_mileage = 0;

#define OUTLINE_COLOR KAWAII_BLACK

static uint16_t getDriveStateColor() {
    switch (Resources::driveBusData().driveState) {
        case 0:
            return INDIAN_RED;
        case 1:
            return KAWAII_YELLOW;
        case 2:
            return KAWAII_GREEN;
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
                              .width = 400,
                              .height = 250,
                              .fill = true,
                              .strokeThickness = 20,
                              .strokeColor = OUTLINE_COLOR,
                              .fillColor = color,
                              .cornerRadius = 15,
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
                       (TextDrawOptions){
                           .x = SCREEN_WIDTH / 2,
                           .y = SCREEN_HEIGHT / 2 + 60,
                           .size = 8,
                           .color = RA8875_BLACK,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawMileageCounter(Adafruit_RA8875 tft) {
    // dont need wheel speed start x y anymore i think
    uint16_t color = KAWAII_BLUE;

    Drawer::drawRect(tft, (RectDrawOptions){
                              .x = SCREEN_WIDTH / 2,
                              .y = SCREEN_HEIGHT * 8 / 9,
                              .width = 300,
                              .height = 80,
                              .fill = true,
                              .strokeThickness = 5,
                              .strokeColor = OUTLINE_COLOR,
                              .fillColor = color,
                              .cornerRadius = 5,
                              .hAlign = ALIGN_CENTER,
                              .vAlign = ALIGN_MIDDLE,
                          });

    // change sizes via if statement


    // need to change millis() with some kind of delta time, placed somewhere where it makes sense
    // maybe in Resources??
    float mileageNum = Resources::instance().logger.readMileCounter();

    Drawer::drawNum(tft, mileageNum,
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH / 2,
                        .y = SCREEN_HEIGHT * 8 / 9,
                        .size = 6,
                        .color = RA8875_BLACK,
                        .backgroundColor = color,
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE,
                    });
    std::string driveString = "mi";
    Drawer::drawString(tft, driveString,
                       (TextDrawOptions){
                           .x = SCREEN_WIDTH * 2 / 3 - 10,
                           .y = SCREEN_HEIGHT * 8 / 9,
                           .size = 3,
                           .color = RA8875_BLACK,
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
                        .color = RA8875_BLACK,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
}

static void drawCircleStatus(Adafruit_RA8875 tft, float startX, float startY, float value, float lowerBound, float upperBound) {
    // Determine the current state based on the thresholds.
    int currState = 0;
    if (value > upperBound) {
        currState = 2;
    } else if (value > lowerBound) {
        currState = 1;
    } else {
        currState = 0;
    }

    int16_t fillColor;
    switch (currState) {
        case 0:
            fillColor = KAWAII_GREEN;
            break;
        case 1:
            fillColor = KAWAII_YELLOW;
            break;
        case 2:
            fillColor = KAWAII_PINK;
            break;
        default:
            fillColor = KAWAII_GREEN;
            break;
    }

    // Set a default circle radius (adjust this value if desired).
    const int defaultRadius = 80;

    // Create the options struct to draw a filled circle.
    CircleDrawOptions circleOptions;
    circleOptions.centerX = static_cast<int>(startX);
    circleOptions.centerY = static_cast<int>(startY);
    circleOptions.radius = defaultRadius;
    circleOptions.fill = true;
    circleOptions.fillColor = fillColor;
    circleOptions.strokeThickness = 10;
    circleOptions.strokeColor = OUTLINE_COLOR;

    // Draw the status circle.
    Drawer::drawCircle(tft, circleOptions);

    // Prepare options to draw the numeric value inside the circle.
    NumberDrawOptions numOptions;
    numOptions.x = circleOptions.centerX;
    numOptions.y = circleOptions.centerY;
    numOptions.size = 6;
    numOptions.color = RA8875_BLACK;
    numOptions.backgroundColor = fillColor;
    numOptions.precision = 1;
    numOptions.hAlign = ALIGN_CENTER;
    numOptions.vAlign = ALIGN_MIDDLE;

    // Draw the numeric value using the Drawer class's drawNum method.
    Drawer::drawNum(tft, value, numOptions);
}

void DriveScreen::draw(Adafruit_RA8875 tft) {
    Serial.print("Drawing DriveScreen!");
    tft.fillScreen(BACKGROUND_GRAY);

    // draw a nice gold line down the middle
    RectDrawOptions options = {0};
    options.x = SCREEN_WIDTH / 2;
    options.y = SCREEN_HEIGHT / 2;
    options.width = 100;
    options.height = SCREEN_HEIGHT;
    options.fillColor = KAWAII_BLACK;
    options.fill = true;
    options.strokeThickness = 0;
    options.hAlign = ALIGN_CENTER;
    options.vAlign = ALIGN_MIDDLE;
    Drawer::drawRect(tft, options);

    // write circle labels
    int16_t color = RA8875_BLACK;
    Drawer::drawString(tft, "HV Battery", hv_bat_volt_startX * 0.15, hv_bat_volt_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "Voltage", hv_bat_volt_startX * 0.4, hv_bat_volt_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "LV Battery", lv_bat_volt_startX * 0.15, lv_bat_volt_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "Voltage", lv_bat_volt_startX * 0.4, lv_bat_volt_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "Max Cell", max_cell_temp_startX * 0.9, max_cell_temp_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "Temp", max_cell_temp_startX * 0.95, max_cell_temp_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "Min Cell", min_cell_temp_startX * 0.9, min_cell_temp_startY - SCREEN_WIDTH / 6 - 10, 3, RA8875_BLACK, color);
    Drawer::drawString(tft, "Temp", min_cell_temp_startX * 0.95, min_cell_temp_startY - SCREEN_WIDTH / 8 - 10, 3, RA8875_BLACK, color);
}

void DriveScreen::update(Adafruit_RA8875 tft, bool force) {
    if (abs(Resources::driveBusData().averageWheelSpeed() - prev_mileage) >= 0.1 || force)
        drawMileageCounter(tft);
        prev_mileage = Resources::driveBusData().averageWheelSpeed();
    if (abs(Resources::driveBusData().driveState - Resources::prevDriveBusData().driveState) >= 0.1 || force) {
        drawDriveState(tft);
        drawWheelSpeed(tft); // gotta redraw that
    }
    if (abs(Resources::driveBusData().averageWheelSpeed() - Resources::prevDriveBusData().averageWheelSpeed()) >= 0.1 || force)
        drawWheelSpeed(tft);
    // Update high-voltage battery status.
    if (abs(Resources::driveBusData().HVVoltage - Resources::prevDriveBusData().HVVoltage) >= 0.1 || force) {
        drawCircleStatus(tft,
                         hv_bat_volt_startX,
                         hv_bat_volt_startY,
                         Resources::driveBusData().HVVoltage,
                         hv_battery_voltage_mid_state,
                         hv_battery_voltage_last_state);
    }

    // Update low-voltage battery status.
    if (abs(Resources::driveBusData().LVVoltage - Resources::prevDriveBusData().LVVoltage) >= 0.1 || force) {
        drawCircleStatus(tft,
                         lv_bat_volt_startX,
                         lv_bat_volt_startY,
                         Resources::driveBusData().LVVoltage,
                         lv_battery_voltage_mid_state,
                         lv_battery_voltage_last_state);
    }

    // Update maximum cell temperature display.
    if (abs(Resources::driveBusData().maxCellTemp - Resources::prevDriveBusData().maxCellTemp) >= 0.1 || force) {
        drawCircleStatus(tft,
                         max_cell_temp_startX,
                         max_cell_temp_startY,
                         Resources::driveBusData().maxCellTemp,  // Change to maxCellTemp if defined separately.
                         max_cell_temp_mid_state,
                         max_cell_temp_last_state);
    }

    // Update minimum cell temperature display.
    if (abs(Resources::driveBusData().minCellTemp - Resources::prevDriveBusData().minCellTemp) >= 0.1 || force) {
        drawCircleStatus(tft,
                         min_cell_temp_startX,
                         min_cell_temp_startY,
                         Resources::driveBusData().minCellTemp,
                         min_cell_temp_mid_state,
                         min_cell_temp_last_state);
    }
}