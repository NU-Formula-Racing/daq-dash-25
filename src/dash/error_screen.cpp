#include "dash/error_screen.h"

#include <string>
#include <vector>

#include "dash/drawer.h"
#include "define.h"  // Must define SCREEN_WIDTH, SCREEN_HEIGHT, colors, etc.
#include "resources.h"

#define OUTLINE_COLOR GOLD

// --- IMD Error Lookup ---
static std::string getIMDErrorMessage() {
    struct {
        int code;
        const char* msg;
    } imdLUT[] = {
        {-10, "IMD:Short Circuit"},
        {-5, "IMD:Loading"},
        {-25, "IMD:Connection Fault"},
        {-20, "IMD:Device Error"}};
    const int count = sizeof(imdLUT) / sizeof(imdLUT[0]);
    // for (int i = 0; i < count; i++) {
    //     if (imd_status == imdLUT[i].code) {
    //         return std::string(imdLUT[i].msg);
    //     }
    // }
    return "";
}

// --- BMS Error Lookup ---
// We assume bits 1 through 7 of the fault mask represent:
// Bit1: UND_VOL, Bit2: OVR_VOL, Bit3: UND_TEMP, Bit4: OVR_TEMP,
// Bit5: OVR_CUR, Bit6: EXTN_KL, Bit7: OPN_WIRE
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
    }
    return result;
}

// --- ECU Error Lookup ---
// We assume ECU faults are stored in a bit mask (bits 0 - 4)
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
    }
    return result;
}

// --- Inverter Error Lookup ---
// A LUT mapping an inverter fault code (from 0x01 up to 0x1B) to a message.
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
        if (inverterFaultLUT[i].code == code) {
            return std::string(inverterFaultLUT[i].msg);
        }
    }
    return "";
}

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
                           .size = 4,
                           .color = RA8875_WHITE,
                           .backgroundColor = color,
                           .hAlign = ALIGN_CENTER,
                           .vAlign = ALIGN_MIDDLE,
                       });
}

static void drawWheelSpeed(Adafruit_RA8875 tft) {
    Drawer::drawNum(tft, Resources::driveBusData().averageWheelSpeed(),
                    (NumberDrawOptions){
                        .x = SCREEN_WIDTH - 100,
                        .y = SCREEN_HEIGHT / 2 - 40,
                        .size = 6,
                        .color = RA8875_WHITE,
                        .backgroundColor = getDriveStateColor(),
                        .hAlign = ALIGN_CENTER,
                        .vAlign = ALIGN_MIDDLE});
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
    if (Resources::driveBusData().driveState != Resources::prevDriveBusData().driveState || force)
        drawDriveState(tft);
    if (Resources::driveBusData().averageWheelSpeed() != Resources::prevDriveBusData().averageWheelSpeed() || force)
        drawWheelSpeed(tft);

    // --- Collect and draw error messages ---
    std::array<std::string, 3> errorLines;

    // get the error messages and put them line by line
    

    if (errorLines.empty()) {
        errorLines[0] = "NO FAULTS!";
    }
}
