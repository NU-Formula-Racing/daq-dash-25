#include "dash/logging_screen.h"


#include "dash/drawer.h"
#include "define.h"
#include "resources.h"

void LoggingScreen::draw(Adafruit_RA8875 tft)
{
    // Clear the screen with a gray background
    tft.fillScreen(BACKGROUND_GRAY);
    setupEncoder();
   
}

static void drawSelectionStatus(Adafruit_RA8875 tft, float startX, float startY, bool isSelected, std::string label)
{
    int16_t fillColor;
    switch (isSelected)
    {
    case false: // case false
        fillColor = KAWAII_BLUE;
        break;
    case true:
        fillColor = KAWAII_GREEN;
        break;
    }

    RectDrawOptions options = {0};
    options.x = static_cast<int>(startX);
    options.y = static_cast<int>(startY);
    options.width = SCREEN_WIDTH;
    options.height = SCREEN_HEIGHT / 5;
    options.fillColor = fillColor;
    options.fill = true;
    options.strokeThickness = 2;
    options.strokeColor = NORTHWESTERN_PURPLE;
    options.hAlign = ALIGN_CENTER;
    options.vAlign = ALIGN_MIDDLE;
    Drawer::drawRect(tft, options);

    Drawer::drawString(tft, label,
                       (TextDrawOptions){
                           .x = static_cast<int>(startX),
                           .y = static_cast<int>(startY),
                           .size = 10,
                           .color = RA8875_WHITE,
                           .backgroundColor = fillColor,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

void LoggingScreen::update(Adafruit_RA8875 tft, bool force)
{
    for (size_t i = 0; i < labels.size(); i++)
    {
        drawSelectionStatus(tft, SCREEN_WIDTH / 2, SCREEN_HEIGHT * (0.2 + i * 0.2), (highlighted == i), labels[i]);
    }
}

void LoggingScreen::setupEncoder() {
    rotaryEncoder.registerL([this]() {
        highlighted = (highlighted + 1) % labels.size();
    });

    rotaryEncoder.registerR([this]() {
        highlighted = (highlighted - 1 + labels.size()) % labels.size();
    });

    rotaryEncoder.registerPush([this]() {
        // do something
    });
}