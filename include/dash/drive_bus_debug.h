#ifndef __DRIVE_BUS_DEBUG_H__
#define __DRIVE_BUS_DEBUG_H__

#include <Adafruit_RA8875.h>

#include <functional>
#include <string>
#include <vector>

#include "dash/drawer.h"
#include "define.h"
#include "screen.h"
#include "resources.h"

struct DebugField {
    const char* label;
    std::function<std::string()> current;
    std::function<std::string()> previous;
};

class DriveBusDebugScreen : public Screen {
   public:
    DriveBusDebugScreen() {}
    void draw(Adafruit_RA8875 tft);
    void update(Adafruit_RA8875 tft, bool force = false);

   private:
    static const std::vector<DebugField>& fields();

    // layout
    static constexpr int START_Y = 50;
    static constexpr int LINE_H = 25;
    static constexpr int COL_COUNT = 3;
    static constexpr int COL_WIDTH = SCREEN_WIDTH / COL_COUNT;
    static constexpr int LABEL_X_OFF = 10;
    static constexpr int VALUE_X_OFF = 150;

    static constexpr TextDrawOptions textOpts = {
        .size = 2,
        .color = RA8875_WHITE,
        .backgroundColor = RA8875_BLACK,
        .hAlign = ALIGN_LEFT,
        .vAlign = ALIGN_TOP};
};
#endif  // __DRIVE_BUS_DEBUG_H__
