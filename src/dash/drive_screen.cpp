#include "dash/drive_screen.h"

#include "dash/drawer.h"
#include "define.h"
#include "resources.h"

#define OUTLINE_COLOR GOLD

static const int infoPaddingVertical = 50;  // from the top and bottom edges of the screen
static const int infoPaddingSides = 20;     // from the left and right edges of the screen
static const int infoWidth = 190;           // width of each info box
static const int infoHeight = 100;
static const int infoLabelSize = 3;
static const int infoValueSize = 3;

struct InfoBox {
    const char *label;
    std::function<float()> currentValue;
    std::function<float()> previousValue;
    float lowerBoundHard;  // lower threshold for the value, show in red if below this
    float lowerBoundSoft;  // lower threshold for the value, show in yellow if below this
    float upperBoundHard;  // upper threshold for the value, show in red if above this
    float upperBoundSoft;  // upper threshold for the value, show in yellow if above this
};

std::vector<InfoBox> leftBoxes = {
    {
        .label = "HV Volt",
        .currentValue = []() { return Resources::driveBusData().hvVoltage; },
        .previousValue = []() { return Resources::prevDriveBusData().hvVoltage; },
        .lowerBoundHard = 500,
        .lowerBoundSoft = 530,
        .upperBoundHard = 590,
        .upperBoundSoft = 580,
    },
    {
        .label = "BMS SOC",
        .currentValue = []() { return Resources::driveBusData().bmsSOC; },
        .previousValue = []() { return Resources::prevDriveBusData().bmsSOC; },
        .lowerBoundHard = 0.10,
        .lowerBoundSoft = 0.60,
        .upperBoundHard = 1.00,
        .upperBoundSoft = 0.95,
    },
    {
        .label = "LV Volt",
        .currentValue = []() { return Resources::driveBusData().lvVoltage; },
        .previousValue = []() { return Resources::prevDriveBusData().lvVoltage; },
        .lowerBoundHard = 20,
        .lowerBoundSoft = 21,
        .upperBoundHard = 24.5,
        .upperBoundSoft = 24,
    },
};

std::vector<InfoBox> rightBoxes = {
    {
        .label = "Max Cell T",
        .currentValue = []() { return Resources::driveBusData().maxCellTemp; },
        .previousValue = []() { return Resources::prevDriveBusData().maxCellTemp; },
        .lowerBoundHard = 15,
        .lowerBoundSoft = 16,
        .upperBoundHard = 100,
        .upperBoundSoft = 90,
    },
    {
        .label = "Min Cell V",
        .currentValue = []() { return Resources::driveBusData().minCellVoltage; },
        .previousValue = []() { return Resources::prevDriveBusData().minCellVoltage; },
        .lowerBoundHard = 2.5,
        .lowerBoundSoft = 2.7,
        .upperBoundHard = 4.2,
        .upperBoundSoft = 4.1,
    },
    {
        .label = "IGBT Temp",
        .currentValue = []() { return Resources::driveBusData().inverterIGBTTemp; },
        .previousValue = []() { return Resources::prevDriveBusData().inverterIGBTTemp; },
        .lowerBoundHard = 15,
        .lowerBoundSoft = 16,
        .upperBoundHard = 70,
        .upperBoundSoft = 65,
    }};

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
                              .width = 300,
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
                           .y = SCREEN_HEIGHT / 2 + 40,
                           .size = 7,
                           .color = GOTH_WHITE,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawMileageCounter(Adafruit_RA8875 tft) {
    // dont need wheel speed start x y anymore i think
    uint16_t color = NORTHWESTERN_PURPLE;

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
                              .width = 280,
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
                        .size = 8,
                        .color = GOTH_WHITE,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
}

static void drawLoggerStatus(Adafruit_RA8875 tft) {
    uint16_t color =
        (Resources::instance().logger.status() == LoggerStatus::LOGGING) ? NORTHWESTERN_PURPLE : GOTH_RED;

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

static uint16_t getInfoBoxColor(InfoBox &box) {
    float value = box.currentValue();
    if (value < box.lowerBoundHard || value > box.upperBoundHard) {
        return GOTH_RED;  // hard bounds
    } else if (value < box.lowerBoundSoft || value > box.upperBoundSoft) {
        return GOLD;  // soft bounds
    } else {
        return GOTH_GREEN;  // normal
    }
}

static void drawInfoBoxes(Adafruit_RA8875 tft, bool force) {
    int infoAvailableVerticalSpace = SCREEN_HEIGHT - infoPaddingVertical * 2;
    int infoFullVerticalSpace = leftBoxes.size() * infoHeight;
    int infoGap = (infoAvailableVerticalSpace - infoFullVerticalSpace) / (leftBoxes.size() - 1);

    // draw boxes for the labels and values
    for (size_t i = 0; i < leftBoxes.size(); i++) {
        InfoBox &box = leftBoxes[i];

        // check if there is enough of a change in the value to warrant a redraw
        if (!force && abs(box.currentValue() - box.previousValue()) < 0.01) {
            continue;  // no need to redraw
        }

        int y = infoPaddingVertical + i * (infoHeight + infoGap) + infoHeight / 2;

        Drawer::drawRect(tft, (RectDrawOptions){
                                  .x = infoPaddingSides + infoWidth / 2,
                                  .y = y,
                                  .width = infoWidth,
                                  .height = infoHeight,
                                  .fill = true,
                                  .strokeThickness = 10,
                                  .strokeColor = OUTLINE_COLOR,
                                  .fillColor = getInfoBoxColor(box),
                                  .cornerRadius = 15,
                                  .hAlign = ALIGN_CENTER,
                                  .vAlign = ALIGN_MIDDLE,
                              });

        Drawer::drawString(tft, box.label,
                           (TextDrawOptions){
                               .x = infoPaddingSides + infoWidth / 2,
                               .y = y - infoHeight / 4,
                               .size = infoLabelSize,
                               .color = GOTH_WHITE,
                               .backgroundColor = getInfoBoxColor(box),
                               .hAlign = ALIGN_CENTER,
                               .vAlign = ALIGN_MIDDLE,
                           });

        Drawer::drawNum(tft, box.currentValue(),
                        (NumberDrawOptions){
                            .x = infoPaddingSides + infoWidth / 2,
                            .y = y + infoHeight / 4,
                            .size = infoValueSize,
                            .color = GOTH_WHITE,
                            .backgroundColor = getInfoBoxColor(box),
                            .precision = 2,
                            .hAlign = ALIGN_CENTER,
                            .vAlign = ALIGN_MIDDLE});
    }

    // do the same for the right side
    infoAvailableVerticalSpace = SCREEN_HEIGHT - infoPaddingVertical * 2;
    infoFullVerticalSpace = rightBoxes.size() * infoHeight;
    infoGap = (infoAvailableVerticalSpace - infoFullVerticalSpace) / (rightBoxes.size() - 1);

    for (size_t i = 0; i < rightBoxes.size(); i++) {
        InfoBox &box = rightBoxes[i];

        // check if there is enough of a change in the value to warrant a redraw
        if (!force && abs(box.currentValue() - box.previousValue()) < 0.01) {
            continue;  // no need to redraw
        }

        int y = infoPaddingVertical + i * (infoHeight + infoGap) + infoHeight / 2;

        Drawer::drawRect(tft, (RectDrawOptions){
                                  .x = SCREEN_WIDTH - infoPaddingSides - infoWidth / 2,
                                  .y = y,
                                  .width = infoWidth,
                                  .height = infoHeight,
                                  .fill = true,
                                  .strokeThickness = 10,
                                  .strokeColor = OUTLINE_COLOR,
                                  .fillColor = getInfoBoxColor(box),
                                  .cornerRadius = 15,
                                  .hAlign = ALIGN_CENTER,
                                  .vAlign = ALIGN_MIDDLE,
                              });

        Drawer::drawString(tft, box.label,
                           (TextDrawOptions){
                               .x = SCREEN_WIDTH - infoPaddingSides - infoWidth / 2,
                               .y = y - infoHeight / 4,
                               .size = infoLabelSize,
                               .color = GOTH_WHITE,
                               .backgroundColor = getInfoBoxColor(box),
                               .hAlign = ALIGN_CENTER,
                               .vAlign = ALIGN_MIDDLE,
                           });

        Drawer::drawNum(tft, box.currentValue(),
                        (NumberDrawOptions){
                            .x = SCREEN_WIDTH - infoPaddingSides - infoWidth / 2,
                            .y = y + infoHeight / 4,
                            .size = infoValueSize,
                            .color = GOTH_WHITE,
                            .backgroundColor = getInfoBoxColor(box),
                            .precision = 2,
                            .hAlign = ALIGN_CENTER,
                            .vAlign = ALIGN_MIDDLE});
    }
}

void DriveScreen::draw(Adafruit_RA8875 tft) {
    Serial.print("Drawing DriveScreen!");
    tft.fillScreen(CARBON_FIBER_BLACK);

    // draw a continuously-sliding twill background with wrapping
    int checkerSize = 20;  // size of each square in px

    int rows = (SCREEN_HEIGHT + checkerSize - 1) / checkerSize;
    int cols = (SCREEN_WIDTH + checkerSize - 1) / checkerSize;

    // small helper for true modulo on negatives:
    auto wrapX = [&](int rawX) {
        int x = rawX % SCREEN_WIDTH;
        if (x < 0) x += SCREEN_WIDTH;
        return x;
    };

    for (int row = 0; row < rows; row++) {
        int y = row * checkerSize;
        // offset increases by half-square each row:
        int xOffset = row * (checkerSize / 2);

        // go a few extra cols to guarantee coverage when wrapping:
        for (int col = 0; col < cols + rows; col++) {
            int rawX = col * checkerSize - xOffset;
            int x = wrapX(rawX);

            // choose color by the sum of (row + col)
            bool even = ((row + col) % 2) == 0;
            uint16_t color = even ? CARBON_FIBER_BLACK : CARBON_FIBER_GREY;

            // if it fits entirely on the right side:
            if (x + checkerSize <= SCREEN_WIDTH) {
                Drawer::drawRect(tft, (RectDrawOptions){
                                          .x = x,
                                          .y = y,
                                          .width = checkerSize,
                                          .height = checkerSize,
                                          .fill = true,
                                          .strokeThickness = 0,
                                          .fillColor = color});
            } else {
                // split into two pieces:
                int width1 = SCREEN_WIDTH - x;      // part to the right edge
                int width2 = checkerSize - width1;  // remaining wrapping on left

                // right-edge piece
                Drawer::drawRect(tft, (RectDrawOptions){
                                          .x = x,
                                          .y = y,
                                          .width = width1,
                                          .height = checkerSize,
                                          .fill = true,
                                          .strokeThickness = 0,
                                          .fillColor = color});
                // left-edge wrapped piece
                Drawer::drawRect(tft, (RectDrawOptions){
                                          .x = 0,
                                          .y = y,
                                          .width = width2,
                                          .height = checkerSize,
                                          .fill = true,
                                          .strokeThickness = 0,
                                          .fillColor = color});
            }
        }
    }

    // draw a northwestern purple line going down the middle of the screen
    int lineThickness = 100;  // thickness of the purple line

    Drawer::drawLine(tft, (LineDrawOptions){
                              .x1 = SCREEN_WIDTH / 2,
                              .y1 = 0,
                              .x2 = SCREEN_WIDTH / 2,
                              .y2 = SCREEN_HEIGHT,
                              .thickness = lineThickness,
                              .color = NORTHWESTERN_PURPLE});

    // now draw thinner gold lines on either side of the purple line
    Drawer::drawLine(tft, (LineDrawOptions){
                              .x1 = SCREEN_WIDTH / 2 - lineThickness / 2 - 30,
                              .y1 = 0,
                              .x2 = SCREEN_WIDTH / 2 - lineThickness / 2 - 30,
                              .y2 = SCREEN_HEIGHT,
                              .thickness = 15,
                              .color = GOLD});

    Drawer::drawLine(tft, (LineDrawOptions){
                              .x1 = SCREEN_WIDTH / 2 + lineThickness / 2 + 30,
                              .y1 = 0,
                              .x2 = SCREEN_WIDTH / 2 + lineThickness / 2 + 30,
                              .y2 = SCREEN_HEIGHT,
                              .thickness = 15,
                              .color = GOLD});

    drawDriveState(tft);
    drawSpeed(tft);
    drawMileageCounter(tft);
    drawLoggerStatus(tft);

    drawInfoBoxes(tft, true);
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

    drawInfoBoxes(tft, force);
}

void DriveScreen::periodicDraw(Adafruit_RA8875 tft) {
    drawMileageCounter(tft);
}