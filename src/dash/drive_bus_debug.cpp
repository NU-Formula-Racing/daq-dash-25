// dash/drive_bus_debug.cpp

#include "dash/drive_bus_debug.h"

#include <cstdio>
#include <string>
#include <vector>

#include "resources.h"

// dash/drive_bus_debug.cpp

#include <cstdio>
#include <string>
#include <vector>

#include "dash/drive_bus_debug.h"
#include "resources.h"
// dash/drive_bus_debug.cpp

#include <cstdio>
#include <string>
#include <vector>

#include "dash/drive_bus_debug.h"
#include "resources.h"

using std::string;
using std::vector;

// ——— helpers ———

static string fmt(const char* fmt, float v) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), fmt, v);
    return string(buf);
}
static string hex8(unsigned v) {
    char buf[8];
    std::snprintf(buf, sizeof(buf), "0x%02X", v);
    return string(buf);
}
static string hex16(unsigned v) {
    char buf[8];
    std::snprintf(buf, sizeof(buf), "0x%04X", v);
    return string(buf);
}

// ——— the table of all fields ———

static const vector<DebugField> debugFields = {
    // speeds
    {"FL_Spd",
     []() { return fmt("%.2f", Resources::driveBusData().wheelSpeeds[0]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelSpeeds[0]); }},
    {"FR_Spd",
     []() { return fmt("%.2f", Resources::driveBusData().wheelSpeeds[1]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelSpeeds[1]); }},
    {"BL_Spd",
     []() { return fmt("%.2f", Resources::driveBusData().wheelSpeeds[2]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelSpeeds[2]); }},
    {"BR_Spd",
     []() { return fmt("%.2f", Resources::driveBusData().wheelSpeeds[3]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelSpeeds[3]); }},

    // displacement
    {"FL_Disp",
     []() { return fmt("%.2f", Resources::driveBusData().wheelDisplacement[0]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelDisplacement[0]); }},
    {"FR_Disp",
     []() { return fmt("%.2f", Resources::driveBusData().wheelDisplacement[1]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelDisplacement[1]); }},
    {"BL_Disp",
     []() { return fmt("%.2f", Resources::driveBusData().wheelDisplacement[2]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelDisplacement[2]); }},
    {"BR_Disp",
     []() { return fmt("%.2f", Resources::driveBusData().wheelDisplacement[3]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().wheelDisplacement[3]); }},

    // strain
    {"FL_Strain",
     []() { return fmt("%.2f", Resources::driveBusData().prStrain[0]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().prStrain[0]); }},
    {"FR_Strain",
     []() { return fmt("%.2f", Resources::driveBusData().prStrain[1]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().prStrain[1]); }},
    {"BL_Strain",
     []() { return fmt("%.2f", Resources::driveBusData().prStrain[2]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().prStrain[2]); }},
    {"BR_Strain",
     []() { return fmt("%.2f", Resources::driveBusData().prStrain[3]); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().prStrain[3]); }},

    // averages & speeds
    {"Avg_RPM",
     []() { return fmt("%.2f", Resources::driveBusData().averageWheelRPM()); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().averageWheelRPM()); }},
    {"Veh_MPH",
     []() { return fmt("%.2f", Resources::driveBusData().vehicleSpeedMPH()); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().vehicleSpeedMPH()); }},

    // states
    {"Drv_St",
     []() { return std::to_string(Resources::driveBusData().driveState); },
     []() { return std::to_string(Resources::prevDriveBusData().driveState); }},
    {"BMS_St",
     []() { return std::to_string(Resources::driveBusData().bmsState); },
     []() { return std::to_string(Resources::prevDriveBusData().bmsState); }},
    {"IMD_St",
     []() { return std::to_string(Resources::driveBusData().imdState); },
     []() { return std::to_string(Resources::prevDriveBusData().imdState); }},

    // SOC
    {"BMS_SOC",
     []() { return fmt("%.1f", Resources::driveBusData().bmsSOC); },
     []() { return fmt("%.1f", Resources::prevDriveBusData().bmsSOC); }},

    // LV warning
    {"LV_Warn",
     []() { return Resources::driveBusData().lvVoltageWarning ? "WARN" : "OK"; },
     []() { return Resources::prevDriveBusData().lvVoltageWarning ? "WARN" : "OK"; }},

    // right-side floats
    {"HV_Volt",
     []() { return fmt("%.2f", Resources::driveBusData().hvVoltage); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().hvVoltage); }},
    {"LV_Volt",
     []() { return fmt("%.2f", Resources::driveBusData().lvVoltage); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().lvVoltage); }},
    {"Max_DisChg",
     []() { return fmt("%.2f", Resources::driveBusData().maxDischargeCurrent); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().maxDischargeCurrent); }},
    {"Max_Regen",
     []() { return fmt("%.2f", Resources::driveBusData().maxRegenCurrent); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().maxRegenCurrent); }},
    {"Batt_Temp",
     []() { return fmt("%.1f", Resources::driveBusData().batteryTemp); },
     []() { return fmt("%.1f", Resources::prevDriveBusData().batteryTemp); }},
    {"Max_Cell_T",
     []() { return fmt("%.1f", Resources::driveBusData().maxCellTemp); },
     []() { return fmt("%.1f", Resources::prevDriveBusData().maxCellTemp); }},
    {"Min_Cell_T",
     []() { return fmt("%.1f", Resources::driveBusData().minCellTemp); },
     []() { return fmt("%.1f", Resources::prevDriveBusData().minCellTemp); }},
    {"Max_Cell_V",
     []() { return fmt("%.2f", Resources::driveBusData().maxCellVoltage); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().maxCellVoltage); }},
    {"Min_Cell_V",
     []() { return fmt("%.2f", Resources::driveBusData().minCellVoltage); },
     []() { return fmt("%.2f", Resources::prevDriveBusData().minCellVoltage); }},

    // inverter & raw faults
    {"Inv_St",
     []() { return hex8(Resources::driveBusData().inverterStatus); },
     []() { return hex8(Resources::prevDriveBusData().inverterStatus); }},
    {"BMS_FltsRaw",
     []() { return hex16(Resources::driveBusData().bmsFaultsRaw); },
     []() { return hex16(Resources::prevDriveBusData().bmsFaultsRaw); }},

    // motor stats
    {"Motor_RPM",
     []() { return std::to_string(Resources::driveBusData().motorRPM); },
     []() { return std::to_string(Resources::prevDriveBusData().motorRPM); }},
    {"Motor_Curr",
     []() { return std::to_string(Resources::driveBusData().motorCurrent); },
     []() { return std::to_string(Resources::prevDriveBusData().motorCurrent); }},
    {"Motor_DC_V",
     []() { return std::to_string(Resources::driveBusData().motorDCVoltage); },
     []() { return std::to_string(Resources::prevDriveBusData().motorDCVoltage); }},
    {"Motor_DC_C",
     []() { return std::to_string(Resources::driveBusData().motorDCCurrent); },
     []() { return std::to_string(Resources::prevDriveBusData().motorDCCurrent); }},

    // ECU faults bitmask
    {"ECU_Flts",
     []() {
         uint16_t bits = 0;
         for (int i = 0; i < ECU_FAULT_COUNT; i++)
             if (Resources::driveBusData().ecuFaults[i]) bits |= (1 << i);
         return hex16(bits);
     },
     []() {
         uint16_t bits = 0;
         for (int i = 0; i < ECU_FAULT_COUNT; i++)
             if (Resources::prevDriveBusData().ecuFaults[i]) bits |= (1 << i);
         return hex16(bits);
     }}};

const std::vector<DebugField>& DriveBusDebugScreen::fields() {
    return debugFields;
}

// ——— draw & update ———

void DriveBusDebugScreen::draw(Adafruit_RA8875 tft) {
    tft.fillScreen(RA8875_BLACK);
    Drawer::drawString(tft, "DRIVE BUS DEBUG", {.x = SCREEN_WIDTH / 2, .y = 5, .size = 4, .color = RA8875_WHITE, .backgroundColor = RA8875_BLACK, .hAlign = ALIGN_CENTER, .vAlign = ALIGN_TOP});

    const auto& flds = fields();
    int perCol = (flds.size() + COL_COUNT - 1) / COL_COUNT;

    for (size_t i = 0; i < flds.size(); ++i) {
        int col = i / perCol;
        int row = i % perCol;
        int x = col * COL_WIDTH + LABEL_X_OFF;
        int y = START_Y + row * LINE_H;

        // draw label
        Drawer::drawString(tft, flds[i].label, {.x = x, .y = y, .size = textOpts.size, .color = textOpts.color, .backgroundColor = textOpts.backgroundColor, .hAlign = textOpts.hAlign, .vAlign = textOpts.vAlign});

        // draw initial value
        Drawer::drawString(tft, flds[i].current(), {.x = x + VALUE_X_OFF, .y = y, .size = textOpts.size, .color = textOpts.color, .backgroundColor = textOpts.backgroundColor, .hAlign = textOpts.hAlign, .vAlign = textOpts.vAlign});
    }
}

void DriveBusDebugScreen::update(Adafruit_RA8875 tft, bool force) {
    const auto& flds = fields();
    int perCol = (flds.size() + COL_COUNT - 1) / COL_COUNT;

    for (size_t i = 0; i < flds.size(); ++i) {
        int col = i / perCol;
        int row = i % perCol;
        int x = col * COL_WIDTH + LABEL_X_OFF + VALUE_X_OFF;
        int y = START_Y + row * LINE_H;

        auto cur = flds[i].current();
        auto prv = flds[i].previous();
        if (force || cur != prv) {
            Drawer::drawString(tft, cur, {.x = x, .y = y, .size = textOpts.size, .color = textOpts.color, .backgroundColor = textOpts.backgroundColor, .hAlign = textOpts.hAlign, .vAlign = textOpts.vAlign});
        }
    }
}
