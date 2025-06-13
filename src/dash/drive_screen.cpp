#include "dash/drive_screen.h"

#include "dash/drawer.h"
#include "define.h"
#include "resources.h"

#define OUTLINE_COLOR GOLD

static const int infoPaddingVertical = 20; // from the top and bottom edges of the screen
static const int infoPaddingSides = 30; // from the left and right edges of the screen
static const int infoWidth = 160; // width of each info box
static const int infoHeight = 100;
static const int infoLabelSize = 3;
static const int infoValueSize = 3;


static std::vector<std::string> infoLabelsLeft = {
    "HV BAT V",
    "LV BAT V",
    "HV SOC"
};

static std::vector<std::string> infoLabelRight = {
    "MIN CELL T",
    "MAX CELL T",
    "INV T"
};

static uint16_t getDriveStateColor() {
    switch (Resources::driveBusData().driveState) {
        case 0:
            return GOTH_RED;
        case 1:
            return GOLD;
        case 2:
            return GOTH_GREEN;
        default:
            return GOTH_RED;
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
                              .height = 200,
                              .fill = true,
                              .strokeThickness = 10,
                              .strokeColor = OUTLINE_COLOR,
                              .fillColor = color,
                              .cornerRadius = 25,
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
            driveString = "DRIVE";
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
                           .color = GOTH_WHITE,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawMileageCounter(Adafruit_RA8875 tft) {
    // dont need wheel speed start x y anymore i think
    uint16_t color = GOTH_BLUE;

    Drawer::drawRect(tft, (RectDrawOptions){
                              .x = SCREEN_WIDTH / 2,
                              .y = SCREEN_HEIGHT * 7 / 8,
                              .width = 300,
                              .height = 80,
                              .fill = true,
                              .strokeThickness = 10,
                              .strokeColor = OUTLINE_COLOR,
                              .fillColor = color,
                              .cornerRadius = 15,
                              .hAlign = ALIGN_CENTER,
                              .vAlign = ALIGN_MIDDLE,
                          });

    Drawer::drawNum(tft, Resources::instance().milageCounter,
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH / 2,
                        .y = SCREEN_HEIGHT * 7 / 8,
                        .size = 6,
                        .color = GOTH_WHITE,
                        .backgroundColor = color,
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE,
                    });

    std::string driveString = "mi";
    Drawer::drawString(tft, driveString,
                       (TextDrawOptions){
                           .x = SCREEN_WIDTH * 2 / 3 - 10,
                           .y = SCREEN_HEIGHT * 7 / 8,
                           .size = 3,
                           .color = GOTH_WHITE,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawSpeed(Adafruit_RA8875 tft) {
    uint16_t color = getDriveStateColor();

    Drawer::drawRect(tft, (RectDrawOptions){
                              .x = SCREEN_WIDTH / 2,
                              .y = SCREEN_HEIGHT / 2 - 40,
                              .width = 330,
                              .height = 100,
                              .fill = true,
                              .strokeThickness = 0,
                              .strokeColor = OUTLINE_COLOR,
                              .fillColor = color,
                              .hAlign = ALIGN_CENTER,
                              .vAlign = ALIGN_MIDDLE,
                          });

    Drawer::drawNum(tft, Resources::driveBusData().vehicleSpeedMPH(),
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH / 2,
                        .y = SCREEN_HEIGHT / 2 - 40,
                        .size = 9,
                        .color = GOTH_WHITE,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
}

static void drawLoggerStatus(Adafruit_RA8875 tft) {
    uint16_t color =
        (Resources::instance().logger.status() == LoggerStatus::LOGGING) ? GOTH_BLUE : GOTH_RED;

    Drawer::drawRect(tft, (RectDrawOptions){
                              .x = SCREEN_WIDTH / 2,
                              .y = SCREEN_HEIGHT * 1 / 8,
                              .width = 300,
                              .height = 80,
                              .fill = true,
                              .strokeThickness = 10,
                              .strokeColor = OUTLINE_COLOR,
                              .fillColor = color,
                              .cornerRadius = 15,
                              .hAlign = ALIGN_CENTER,
                              .vAlign = ALIGN_MIDDLE,
                          });

    if (Resources::instance().logger.status() == LoggerStatus::LOGGING) {
        Drawer::drawString(tft, "LOGGING",
                           (TextDrawOptions){
                               .x = SCREEN_WIDTH / 2,
                               .y = SCREEN_HEIGHT * 1 / 8 - 15,
                               .size = 4,
                               .color = GOTH_WHITE,
                               .backgroundColor = color,
                               .hAlign = ALIGN_CENTER,
                               .vAlign = ALIGN_MIDDLE,
                           });

        Drawer::drawString(tft, Resources::instance().logger.logFileName(),
                           (TextDrawOptions){
                               .x = SCREEN_WIDTH / 2,
                               .y = SCREEN_HEIGHT * 1 / 8 + 15,
                               .size = 3,
                               .color = GOTH_WHITE,
                               .backgroundColor = color,
                               .hAlign = ALIGN_CENTER,
                               .vAlign = ALIGN_MIDDLE,
                           });
    } else {
        Drawer::drawString(tft, "NOT LOGGING",
                           (TextDrawOptions){
                               .x = SCREEN_WIDTH / 2,
                               .y = SCREEN_HEIGHT * 1 / 9 + 5,
                               .size = 4,
                               .color = GOTH_WHITE,
                               .backgroundColor = color,
                               .hAlign = ALIGN_CENTER,
                               .vAlign = ALIGN_MIDDLE,
                           });
    }
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
    options.fillColor = GOLD;
    options.fill = true;
    options.strokeThickness = 0;
    options.hAlign = ALIGN_CENTER;
    options.vAlign = ALIGN_MIDDLE;
    Drawer::drawRect(tft, options);

    // draw info labels on the left and right side
    // distribute them evenly vertically
    // left labels are aligned left horizontally
    // right labels are aligned right horizontally

    // total amount of space that we can distribute the labels in
    // draw the boxes for the labels + values

    int infoAvailableVerticalSpace = SCREEN_HEIGHT - infoPaddingVertical * 2;
    int infoFullVerticalSpace = infoLabelsLeft.size() * infoHeight + (infoLabelsLeft.size() - 1) * infoPaddingVertical;
    int infoGap = (infoAvailableVerticalSpace - infoFullVerticalSpace) / (infoLabelsLeft.size() + 1);

    // draw boxes for the labels and values
    for (size_t i = 0; i < infoLabelsLeft.size(); i++) {
        int y = infoPaddingVertical + i * (infoHeight + infoGap);
        Drawer::drawRect(tft, (RectDrawOptions){
                                  .x = infoPaddingSides + infoWidth / 2,
                                  .y = y + infoHeight / 2,
                                  .width = infoWidth,
                                  .height = infoHeight,
                                  .fill = true,
                                  .strokeThickness = 10,
                                  .strokeColor = OUTLINE_COLOR,
                                  .fillColor = GOTH_GREEN,
                                  .cornerRadius = 15,
                                  .hAlign = ALIGN_CENTER,
                                  .vAlign = ALIGN_MIDDLE,
                              });

        Drawer::drawString(tft, infoLabelsLeft[i],
                           (TextDrawOptions){
                               .x = infoPaddingSides,
                               .y = y + infoHeight / 2,
                               .size = infoLabelSize,
                               .color = GOTH_WHITE,
                               .backgroundColor = GOTH_GREEN,
                               .hAlign = ALIGN_LEFT,
                               .vAlign = ALIGN_MIDDLE,
                           });

        Drawer::drawNum(tft, Resources::driveBusData().hvVoltage,  // TODO: change to correct data
                        (NumberDrawOptions){
                            .x = infoPaddingSides,
                            .y = y - infoHeight / 2,
                            .size = infoValueSize,
                            .color = GOTH_WHITE,
                            .backgroundColor = GOTH_GREEN,
                            .precision = 2,
                            .hAlign = ALIGN_LEFT,
                            .vAlign = ALIGN_MIDDLE});
    }
}

void DriveScreen::update(Adafruit_RA8875 tft, bool force) {
    if (Resources::driveBusData().driveState != Resources::prevDriveBusData().driveState || force) {
        drawDriveState(tft);
        drawSpeed(tft);  // gotta redraw that
        // also just draw the logger stuff cause idk where to put it
        drawLoggerStatus(tft);
    }
    if (abs(Resources::driveBusData().averageWheelRPM() - Resources::prevDriveBusData().averageWheelRPM()) >= 0.1 || force) {
        drawSpeed(tft);
    }

    // // Update high-voltage battery status.
    // if (Resources::driveBusData().hvVoltage != Resources::prevDriveBusData().hvVoltage  || force) {
    //     drawCircleStatus(tft,
    //                      hv_bat_volt_startX,
    //                      hv_bat_volt_startY,
    //                      Resources::driveBusData().hvVoltage,
    //                      hv_battery_voltage_mid_state,
    //                      hv_battery_voltage_last_state);
    // }

    // // Update low-voltage battery status.
    // if (Resources::driveBusData().lvVoltage != Resources::prevDriveBusData().lvVoltage || force) {
    //     drawCircleStatus(tft,
    //                      lv_bat_volt_startX,
    //                      lv_bat_volt_startY,
    //                      Resources::driveBusData().lvVoltage,
    //                      lv_battery_voltage_mid_state,
    //                      lv_battery_voltage_last_state);
    // }

    // // Update maximum cell temperature display.
    // if (Resources::driveBusData().maxCellTemp != Resources::prevDriveBusData().maxCellTemp || force) {
    //     drawCircleStatus(tft,
    //                      max_cell_temp_startX,
    //                      max_cell_temp_startY,
    //                      Resources::driveBusData().maxCellTemp,  // Change to maxCellTemp if defined separately.
    //                      max_cell_temp_mid_state,
    //                      max_cell_temp_last_state);
    // }

    // // Update minimum cell temperature display.
    // if (Resources::driveBusData().minCellVoltage - Resources::prevDriveBusData().minCellVoltage || force) {
    //     drawCircleStatus(tft,
    //                      min_cell_temp_startX,
    //                      min_cell_temp_startY,
    //                      Resources::driveBusData().minCellVoltage,
    //                      min_cell_temp_mid_state,
    //                      min_cell_temp_last_state);
    // }
}

void DriveScreen::periodicDraw(Adafruit_RA8875 tft) {
    drawMileageCounter(tft);
}