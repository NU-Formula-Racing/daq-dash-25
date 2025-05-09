#ifndef __DRIVE_BUS_DEBUG_H__
#define __DRIVE_BUS_DEBUG_H__

#include "dash/drawer.h"
#include "dash/drive_bus_debug.h"
#include "define.h"
#include "resources.h"

// layout constants
static constexpr int LEFT_X = 10;
static constexpr int RIGHT_X = SCREEN_WIDTH / 2 + 10;
static constexpr int START_Y = 30;
static constexpr int LINE_H = 35;
static constexpr int TEXT_OFFSET = 250;

static TextDrawOptions textOpts = {
    .size = 4,
    .color = RA8875_WHITE,
    .backgroundColor = RA8875_BLACK,
    .hAlign = ALIGN_LEFT,
    .vAlign = ALIGN_TOP};

static NumberDrawOptions numOpts = {
    .size = 3,
    .color = RA8875_WHITE,
    .backgroundColor = RA8875_BLACK,
    .hAlign = ALIGN_LEFT,
    .vAlign = ALIGN_TOP};

void DriveBusDebugScreen::draw(Adafruit_RA8875 tft) {
    // 1) clear screen
    tft.fillScreen(RA8875_BLACK);

    // 2) header
    Drawer::drawString(tft, "DRIVE BUS DEBUG", {.x = SCREEN_WIDTH / 2, .y = 5, .size = 4, .color = RA8875_CYAN, .backgroundColor = RA8875_BLACK, .hAlign = ALIGN_CENTER, .vAlign = ALIGN_TOP});

    // 3) labels
    int line = 0;
    // left column
    const char* leftLabels[] = {
        "FL_Speed", "FR_Speed", "BL_Speed", "BR_Speed",
        "Avg_RPM", "Veh_MPH", "Drive_St", "BMS_St",
        "IMD_St", "BMS_SOC"};
    for (auto lbl : leftLabels) {
        Drawer::drawString(tft, lbl, {.x = LEFT_X, .y = START_Y + line * LINE_H, .size = textOpts.size, .color = textOpts.color, .backgroundColor = textOpts.backgroundColor, .hAlign = textOpts.hAlign, .vAlign = textOpts.vAlign});
        line++;
    }

    // right column
    line = 0;
    const char* rightLabels[] = {
        "HV_Volt", "LV_Volt", "Batt_Temp", "Max_Cell_T",
        "Min_Cell_T", "Max_Cell_V", "Min_Cell_V",
        "INV_Status", "BMS_Faults", "ECU_Faults"};
    for (auto lbl : rightLabels) {
        Drawer::drawString(tft, lbl, {.x = RIGHT_X, .y = START_Y + line * LINE_H, .size = textOpts.size, .color = textOpts.color, .backgroundColor = textOpts.backgroundColor, .hAlign = textOpts.hAlign, .vAlign = textOpts.vAlign});
        line++;
    }

    // 4) first-pass draw of all values
    update(tft, /*force=*/true);
}

void DriveBusDebugScreen::update(Adafruit_RA8875 tft, bool force) {
    const auto& d = Resources::driveBusData();
    const auto& pd = Resources::prevDriveBusData();

    auto drawIf = [&](float newVal, float oldVal, int x, int y) {
        if (force || newVal != oldVal) {
            Drawer::drawNum(tft, newVal, {.x = x, .y = y, .size = numOpts.size, .color = numOpts.color, .backgroundColor = numOpts.backgroundColor, .hAlign = numOpts.hAlign, .vAlign = numOpts.vAlign});
        }
    };
    auto drawIfU8 = [&](uint8_t newVal, uint8_t oldVal, int x, int y) {
        if (force || newVal != oldVal) {
            Drawer::drawNum(tft, newVal, {.x = x, .y = y, .size = numOpts.size, .color = numOpts.color, .backgroundColor = numOpts.backgroundColor, .hAlign = numOpts.hAlign, .vAlign = numOpts.vAlign});
        }
    };
    auto drawIfHex = [&](uint16_t newVal, uint16_t oldVal, int x, int y) {
        if (force || newVal != oldVal) {
            char buf[8];
            sprintf(buf, "0x%04X", newVal);
            Drawer::drawString(tft, buf, {.x = x, .y = y, .size = textOpts.size, .color = textOpts.color, .backgroundColor = textOpts.backgroundColor, .hAlign = textOpts.hAlign, .vAlign = textOpts.vAlign});
        }
    };

    // left col positions
    int ly = START_Y;
    drawIf(d.wheelSpeeds[0], pd.wheelSpeeds[0], LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIf(d.wheelSpeeds[1], pd.wheelSpeeds[1], LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIf(d.wheelSpeeds[2], pd.wheelSpeeds[2], LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIf(d.wheelSpeeds[3], pd.wheelSpeeds[3], LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;

    drawIf(d.averageWheelRPM(), pd.averageWheelRPM(), LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIf(d.vehicleSpeedMPH(), pd.vehicleSpeedMPH(), LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;

    drawIfU8(d.driveState, pd.driveState, LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIfU8(d.bmsState, pd.bmsState, LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIfU8(d.imdState, pd.imdState, LEFT_X + TEXT_OFFSET, ly);
    ly += LINE_H;
    drawIfU8(d.bmsSOC, pd.bmsSOC, LEFT_X + TEXT_OFFSET, ly); /* ly+=LINE_H; */

    // right col positions
    int ry = START_Y;
    drawIf(d.hvVoltage, pd.hvVoltage, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;
    drawIf(d.lvVoltage, pd.lvVoltage, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;
    drawIf(d.batteryTemp, pd.batteryTemp, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;
    drawIf(d.maxCellTemp, pd.maxCellTemp, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;
    drawIf(d.minCellTemp, pd.minCellTemp, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;
    drawIf(d.maxCellVoltage, pd.maxCellVoltage, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;
    drawIf(d.minCellVoltage, pd.minCellVoltage, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;

    drawIfU8(d.inverterStatus, pd.inverterStatus, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;

    drawIfHex(d.bmsFaultsRaw, pd.bmsFaultsRaw, RIGHT_X + TEXT_OFFSET, ry);
    ry += LINE_H;

    // simplest way to pack ECU faults as bits in hex too:
    uint16_t ecuBits = 0;
    for (int i = 0; i < ECU_FAULT_COUNT; i++)
        if (d.ecuFaults[i]) ecuBits |= (1 << i);
    uint16_t pEcuBits = 0;
    for (int i = 0; i < ECU_FAULT_COUNT; i++)
        if (pd.ecuFaults[i]) pEcuBits |= (1 << i);
    drawIfHex(ecuBits, pEcuBits, RIGHT_X + TEXT_OFFSET, ry);
}

#endif  // __DRIVE_BUS_DEBUG_H__