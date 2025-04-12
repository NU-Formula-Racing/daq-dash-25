#include "dash/error_screen.h"

#include <string>
#include <vector>

#include "dash/drawer.h"
#include "define.h"  // Must define SCREEN_WIDTH, SCREEN_HEIGHT, colors, etc.
#include "resources.h"

#define OUTLINE_COLOR GOLD

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
                              .x = SCREEN_WIDTH - 200,
                              .y = SCREEN_HEIGHT / 2,
                              .width = 200,
                              .height = SCREEN_HEIGHT,
                              .fill = true,
                              .strokeThickness = 0,
                              .fillColor = color,
                              .hAlign = ALIGN_LEFT,
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
                           .x = SCREEN_WIDTH - 100,
                           .y = SCREEN_HEIGHT / 2 + 60,
                           .size = 6,
                           .color = RA8875_WHITE,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawWheelSpeed(Adafruit_RA8875 tft) {
    Drawer::drawNum(tft, Resources::driveBusData().averageWheelSpeed(),
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH  - 100,
                        .y = SCREEN_HEIGHT / 2 - 40,
                        .size = 4,
                        .color = RA8875_WHITE,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
}



void ErrorScreen::draw(Adafruit_RA8875 tft) {
    // Clear the screen with a red background.
    tft.fillScreen(INDIAN_RED);

    TextDrawOptions headerOptions = {
        .x = (SCREEN_WIDTH - 200) / 2, 
        .y = 20, 
        .size = 5, 
        .color = RA8875_WHITE, 
        .backgroundColor = INDIAN_RED,
        .hAlign = ALIGN_CENTER, 
        .vAlign = ALIGN_TOP
    };

    Drawer::drawString(tft, "ERROR SCREEN :(", headerOptions);

}

void ErrorScreen::update(Adafruit_RA8875 tft, bool force) {
    if (Resources::driveBusData().driveState != Resources::prevDriveBusData().driveState || force)
        drawDriveState(tft);
    if (Resources::driveBusData().averageWheelSpeed() != Resources::prevDriveBusData().averageWheelSpeed() || force)
        drawWheelSpeed(tft);

    // --- Collect and draw error messages ---
    std::vector<std::string> errorLines;
    if (errorLines.empty()) {
        errorLines.push_back("NO FAULTS");
    }

}
