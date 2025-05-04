#include "dash/error_screen.h"

#include <string>
#include <vector>

#include "dash/drawer.h"
#include "define.h"  // Must define SCREEN_WIDTH, SCREEN_HEIGHT, colors, etc.
#include "resources.h"

#define OUTLINE_COLOR KAWAII_BLACK
#define MAX_CHARS_PER_LINE 30

// --- IMD Error Lookup ---
static std::string getIMDErrorMessage() {
    if (Resources::driveBusData().imdState == 0) {
        return "IMD_ERR";
    }
    return "";
}

static std::string getBMSErrorMessage() {
    static const char* bmsFaultMessages[7] = {
        "UND_VOL",
        "OVR_VOL",
        "UND_TEMP",
        "OVR_TEMP",
        "OVR_CUR",
        "EXTN_KL",
        "OPN_WIRE"};

    std::string result = "BMS:";
    bool any = false;
    for (int i = 0; i < ECU_FAULT_COUNT; i++) {
        if (Resources::driveBusData().bmsFaults[i]) {
            any = true;
            result += bmsFaultMessages[i];
            result += ",";
        }
    }

    if (any) {  // Remove trailing comma
        result.pop_back();
    } else {
        return "";
    }

    return result;
}

static std::string getECUErrorMessage() {
    static const char* ecuFaultMessages[5] = {
        "IMPLS_PRSNT",
        "IMPLS_APPS_DIS_IMP",
        "IMPLS_BPPC_IMP",
        "IMPLS_BRK_INVLD_IMP",
        "IMPLS_APPS_INVLD_IMP"};

    std::string result = "ECU:";
    bool any = false;
    for (int i = 0; i < ECU_FAULT_COUNT; i++) {
        if (Resources::driveBusData().ecuFaults[i]) {
            any = true;
            result += ecuFaultMessages[i];
            result += ",";
        }
    }

    if (any) {
        result.pop_back();
    } else {
        return "";
    }

    return result;
}

static std::string getInverterErrorMessage() {
    struct InverterFault {
        uint8_t code;
        const char* msg;
    };

    static const InverterFault inverterFaultLUT[] = {
        {0x01, "OVR_VOL"},
        {0x02, "UND_VOL"},
        {0x03, "DRV_FLT"},
        {0x04, "ABS_OVR_CUR"},
        {0x05, "OVR_TEMP_FET"},
        {0x06, "OVR_TEMP_MTR"},
        {0x07, "GTE_DRV_OV"},
        {0x08, "GTE_DRV_UV"},
        {0x09, "MCU_UV"},
        {0x0A, "BTING_FRM_WCHDG_RST"},
        {0x0B, "ENCDR_SPI_FLT"},
        {0x0C, "ENCDR_SICO_BLW_MAX_AMP"},
        {0x0D, "ENCDR_SICO_ABV_MAX_AMP"},
        {0x0E, "FLASH_COR"},
        {0x0F, "HI_OFFS_CUR_SENS1"},
        {0x10, "HI_OFFS_CUR_SENS2"},
        {0x11, "HI_OFFS_CUR_SENS3"},
        {0x12, "UNBAL_CUR"},
        {0x13, "BRK_FLT"},
        {0x14, "RESLVR_LOT"},
        {0x15, "RESLVR_DOS"},
        {0x16, "RESLVR_LOS"},
        {0x17, "FLSH_COR_APP_CFG"},
        {0x18, "FLSH_COR_APP_CFG"},
        {0x19, "ENCDR_NO_MAG"},
        {0x1A, "ENCDR_MAG_2_STRNG"},
        {0x1B, "PHS_FILTR_FLT"}};
    const int lutSize = sizeof(inverterFaultLUT) / sizeof(inverterFaultLUT[0]);
    for (int i = 0; i < lutSize; i++) {
        if (inverterFaultLUT[i].code == Resources::driveBusData().inverterStatus) {
            return std::string(inverterFaultLUT[i].msg);
        }
    }

    return "";
}

static uint16_t getDriveStateColor() {
    switch (Resources::driveBusData().driveState) {
        case 0:
            return KAWAII_PINK;
        case 1:
            return KAWAII_YELLOW;
        case 2:
            return KAWAII_GREEN;
        default:
            return KAWAII_PINK;
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
                              .fillColor = color,
                              .hAlign = ALIGN_LEFT,
                              .vAlign = ALIGN_MIDDLE,
                          });

    // draw a little edge to the side
    Drawer::drawRect(tft, (RectDrawOptions){
                              .x = SCREEN_WIDTH - 220,
                              .y = SCREEN_HEIGHT / 2,
                              .width = 20,
                              .height = SCREEN_HEIGHT,
                              .fill = true,
                              .fillColor = OUTLINE_COLOR,
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
            driveString = "DRIVE";
            break;
        default:
            driveString = "ERROR";
            break;
    }

    Drawer::drawString(tft, driveString,
                       (TextDrawOptions){
                           .x = SCREEN_WIDTH - 100,
                           .y = SCREEN_HEIGHT / 2 + 60,
                           .size = 4,
                           .color = RA8875_BLACK,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawWheelSpeed(Adafruit_RA8875 tft) {
    Drawer::drawNum(tft, (Resources::driveBusData().averageWheelSpeed() / (WHEEL_DIAMETER * 12 * 5280 * 3)),  // times 3 cause 3:1 ratio as we are using motor rpm rn
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH - 100,
                        .y = SCREEN_HEIGHT / 2 - 40,
                        .size = 6,
                        .color = RA8875_BLACK,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
}

static bool errorsChanged() {
    return memcmp(Resources::driveBusData().bmsFaults, Resources::prevDriveBusData().bmsFaults, sizeof(bool) * BMS_FAULT_COUNT) == 1 ||
           memcmp(Resources::driveBusData().ecuFaults, Resources::prevDriveBusData().ecuFaults, sizeof(bool) * ECU_FAULT_COUNT) == 1;
}

// Helper function to wrap a text string based on a maximum number of characters.
static std::vector<std::string> wrapText(const std::string& text, size_t maxChars) {
    std::vector<std::string> lines;
    size_t start = 0;
    while (start < text.length()) {
        // Determine where to end this line.
        size_t end = start + maxChars;
        if (end >= text.length()) {
            lines.push_back(text.substr(start));
            break;
        }
        // Find the last space before end.
        size_t spacePos = text.rfind(' ', end);
        if (spacePos == std::string::npos || spacePos < start) {
            // No space found; force-break
            spacePos = end;
        }
        lines.push_back(text.substr(start, spacePos - start));
        // Skip space for next word.
        start = spacePos;
        while (start < text.length() && text[start] == ' ') {
            start++;
        }
    }
    return lines;
}

void ErrorScreen::draw(Adafruit_RA8875 tft) {
    // Clear the screen with a red background.
    tft.fillScreen(INDIAN_RED);

    TextDrawOptions headerOptions = {
        .x = (SCREEN_WIDTH - 220) / 2,
        .y = 20,
        .size = 5,
        .color = RA8875_WHITE,
        .backgroundColor = INDIAN_RED,
        .hAlign = ALIGN_CENTER,
        .vAlign = ALIGN_TOP};

    Drawer::drawString(tft, "ERROR SCREEN :(", headerOptions);
}

void ErrorScreen::update(Adafruit_RA8875 tft, bool force) {
    // Update mini drive state and wheel speed if changed (or if forced).
    if (Resources::driveBusData().driveState != Resources::prevDriveBusData().driveState || force) {
        // drawDriveState() is assumed to be defined elsewhere.
        drawDriveState(tft);
        // drawWheelSpeed() is assumed to be defined elsewhere.
        drawWheelSpeed(tft);
    }
    if (Resources::driveBusData().averageWheelSpeed() != Resources::prevDriveBusData().averageWheelSpeed() || force) {
        // drawWheelSpeed() is assumed to be defined elsewhere.
        drawWheelSpeed(tft);
    }

    // If errors have not changed, no need to update.
    if (errorsChanged() == false) return;

    std::vector<std::string> errorLines;
    // Here we use your LUT-based functions to get the error messages.
    std::string bmsLine = getBMSErrorMessage();
    std::string ecuLine = getECUErrorMessage();
    std::string imdLine = getIMDErrorMessage();
    std::string inverterLine = getInverterErrorMessage();

    if (!imdLine.empty()) errorLines.emplace_back(imdLine);
    if (!bmsLine.empty()) errorLines.emplace_back(bmsLine);
    if (!inverterLine.empty()) errorLines.emplace_back(inverterLine);
    if (!ecuLine.empty()) errorLines.emplace_back(ecuLine);

    // If no dynamic errors, show a "NO FAULTS!" message.
    if (errorLines.empty())
        errorLines.push_back("NO FAULTS!");

    // --- Wrap error messages if they are too long ---
    std::vector<std::string> wrappedLines;
    for (const std::string& line : errorLines) {
        std::vector<std::string> parts = wrapText(line, MAX_CHARS_PER_LINE);
        for (const std::string& part : parts) {
            wrappedLines.push_back(part);
        }
    }

    // --- Erase previous error messages area ---
    // Compute the total height needed for the wrapped lines.
    int lineHeight = 40;  // Adjust this as needed.
    int totalHeight = lineHeight * static_cast<int>(wrappedLines.size());
    int clearY = SCREEN_HEIGHT / 2 - totalHeight / 2;
    // Clear the area by filling it with the error background color.
    tft.fillRect(0, clearY, SCREEN_WIDTH - 240, 300, INDIAN_RED);

    // --- Draw each wrapped error message line ---
    for (size_t i = 0; i < wrappedLines.size(); i++) {
        TextDrawOptions options;
        options.x = 20;  // Left align; adjust if you want centering.
        options.y = clearY + static_cast<int>(i * lineHeight) + (lineHeight / 2);
        options.size = 3;
        options.color = RA8875_WHITE;
        options.backgroundColor = INDIAN_RED;
        options.hAlign = ALIGN_LEFT;
        options.vAlign = ALIGN_MIDDLE;
        Drawer::drawString(tft, wrappedLines[i], options);
    }
}