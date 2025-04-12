#include "dash/error_screen.h"
#include "dash/drawer.h"



void ErrorScreen::draw(Adafruit_RA8875 tft) {
    // // dont need wheel speed start x y anymore i think
    // int16_t color = INDIAN_RED;
    // int driveRectw = startX / 2;
    // int driveRecth = startY / 4;
    // int digit_spacing = 4;
    // int char_width = 40;
    // int draw_digit_size = 6;

    // //  int16_t color = INDIAN_RED;
    // //  int driveRectw = startX;
    // //  int driveRecth= startY/2;
    // //  int digit_spacing = 4;
    // //  int char_width = 40;
    // //  int draw_digit_size = 6;
    // //  startX= startX * 4 * 0.4;
    // //  startY= startY * 5;
    // switch (curr_drive_state) {
    //     case 0:
    //         color = INDIAN_RED;
    //         break;
    //     case 1:
    //         color = GOLD;
    //         break;
    //     case 2:
    //         color = FERN_GREEN;
    //         break;
    //     default:
    //         color = INDIAN_RED;
    //         break;
    // }
    // tft.fillRect(startX, startY + 30, driveRectw, driveRecth, color);
    // // change sizes via if statement
    // switch (curr_drive_state) {
    //     case 0:
    //         Drawer::drawString(tft, "OFF", startX * 1.15, startY * 1.25, 3, RA8875_WHITE, color);
    //         // Drawer::drawString(tft, "OFF", SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.58, 5, RA8875_WHITE, color);
    //         break;
    //     case 1:
    //         Drawer::drawString(tft, "NEUTRAL", startX * 1.05, startY * 1.25, 3, RA8875_BLACK, color);
    //         // Drawer::drawString(tft, "NEUTRAL", SCREEN_WIDTH * 0.47, SCREEN_HEIGHT * 0.58, 5, RA8875_BLACK, color);
    //         break;
    //     case 2:
    //         Drawer::drawString(tft, "DRIVE", startX * 1.12, startY * 1.25, 3, RA8875_WHITE, color);
    //         break;
    //     default:
    //         Drawer::drawString(tft, "ERROR", startX * 1.25, startY * 1.25, 3, RA8875_WHITE, color);
    //         break;
    // }

    // drive_state = curr_drive_state;
    // int rounded_wheel_speed = round(wheel_speed);

    // // int digit_spacing = 8;
    // // int char_width = 80;

    // startX -= char_width / 2;

    // // Making a naive assumption that 0 <= wheel speed < 100
    // if (wheel_speed > 99) {
    //     startX += char_width;
    // } else if (wheel_speed > 9) {
    //     // Digits must be off center for double digit numbers
    //     startX += char_width / 2;
    // }

    // // Draw the digits
    // while (rounded_wheel_speed > 0) {
    //     int digit = rounded_wheel_speed % 10;
    //     tft.drawChar(startX + 50, startY + 30, digit + '0', RA8875_BLACK, color, draw_digit_size);
    //     wheel_speed_startX -= char_width + digit_spacing;
    //     rounded_wheel_speed /= 10;
    // }
}

void ErrorScreen::update(Adafruit_RA8875 tft) {
}