#include "dash/drawer.h"
#include <sstream>
#include <iomanip>

//-------------------------------------------------
// Legacy drawString implementation.
//-------------------------------------------------
void Drawer::drawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir) {
    for (int i = 0; i < message.length(); i++) {
        switch (dir) {
            case LEFT_TO_RIGHT:
                tft.drawChar(startX + i * size * 6, startY, message[i], color, backgroundColor, size);
                break;
            case UP_TO_DOWN:
                tft.drawChar(startX, startY + i * size * 8, message[i], color, backgroundColor, size);
                break;
            default:
                break;
        }
    }
}

//-------------------------------------------------
// Legacy drawNum implementation (with fixed precision).
//-------------------------------------------------
void Drawer::drawNum(Adafruit_RA8875 tft, float num, int startX, int startY, int size, int16_t color, int16_t backgroundColor) {
    int precision = 1;  // default precision

    int multiplier = pow(10, precision);
    int floatAsInt = static_cast<int>(num * multiplier + 0.5);

    int digitSpacing = -8;
    int charWidth = 48;

    startX -= charWidth / 2;  // Adjust starting position for centering

    // Count number of digits.
    int temp = floatAsInt;
    int digitCount = 0;
    while (temp > 0) {
        temp /= 10;
        digitCount++;
    }
    if (floatAsInt == 0) {
        digitCount = 1;
    }

    if (digitCount >= 5) {
        startX += charWidth;
    } else if (digitCount >= 3) {
        startX += charWidth / 2;
    }

    int digitsDrawn = 0;
    while (digitsDrawn < digitCount) {
        if (digitsDrawn == precision) {
            tft.drawChar(startX, startY, '.', color, backgroundColor, 6);
            startX -= charWidth / 2;
        } else {
            int digit = floatAsInt % 10;
            tft.drawChar(startX, startY, digit + '0', color, backgroundColor, 6);
            startX -= charWidth + digitSpacing;
            floatAsInt /= 10;
        }
        digitsDrawn++;
    }
}

//-------------------------------------------------
// New drawString implementation using TextDrawOptions.
//-------------------------------------------------
void Drawer::drawString(Adafruit_RA8875 tft, std::string message, const TextDrawOptions &options) {
    int x = options.x;
    int y = options.y;

    // Estimate dimensions. For left-to-right text:
    //   - Each character is ~ (size * 6) pixels wide.
    //   - Height is ~ (size * 8) pixels.
    int textWidth = message.length() * options.size * 6;
    int textHeight = options.size * 8;

    if (options.direction == LEFT_TO_RIGHT) {
        // Adjust x according to horizontal alignment.
        if (options.hAlign == ALIGN_CENTER) {
            x -= textWidth / 2;
        } else if (options.hAlign == ALIGN_RIGHT) {
            x -= textWidth;
        }
        // Adjust y according to vertical alignment.
        if (options.vAlign == ALIGN_MIDDLE) {
            y -= textHeight / 2;
        } else if (options.vAlign == ALIGN_BOTTOM) {
            y -= textHeight;
        }
        
        // Draw each character.
        for (int i = 0; i < message.length(); i++) {
            tft.drawChar(x + i * options.size * 6, y, message[i], options.color, options.backgroundColor, options.size);
        }
    } 
    else if (options.direction == UP_TO_DOWN) {
        // For vertical text, width is size * 6 and height is length * size * 8.
        int textWidthVertical = options.size * 6;
        int textHeightVertical = message.length() * options.size * 8;
        // Adjust x horizontally.
        if (options.hAlign == ALIGN_CENTER) {
            x -= textWidthVertical / 2;
        } else if (options.hAlign == ALIGN_RIGHT) {
            x -= textWidthVertical;
        }
        // Adjust y vertically.
        if (options.vAlign == ALIGN_MIDDLE) {
            y -= textHeightVertical / 2;
        } else if (options.vAlign == ALIGN_BOTTOM) {
            y -= textHeightVertical;
        }
        
        // Draw each character vertically.
        for (int i = 0; i < message.length(); i++) {
            tft.drawChar(x, y + i * options.size * 8, message[i], options.color, options.backgroundColor, options.size);
        }
    }
}
//-------------------------------------------------
// New drawNum implementation using NumberDrawOptions.
//-------------------------------------------------
void Drawer::drawNum(Adafruit_RA8875 tft, float num, const NumberDrawOptions &options) {
    // Create a buffer for the formatted number.
    char buf[20];
    dtostrf(num, 0, options.precision, buf);
    int textLength = strlen(buf);
    
    // Calculate approximate dimensions.
    int textWidth = textLength * options.size * 6;  // Approximate width per character.
    int textHeight = options.size * 8;                // Approximate height.
    
    // Adjust the starting coordinates based on horizontal alignment.
    int x = options.x;
    if (options.hAlign == ALIGN_CENTER) {
        x -= textWidth / 2;
    } else if (options.hAlign == ALIGN_RIGHT) {
        x -= textWidth;
    }
    
    // Adjust the starting coordinates based on vertical alignment.
    int y = options.y;
    if (options.vAlign == ALIGN_MIDDLE) {
        y -= textHeight / 2;
    } else if (options.vAlign == ALIGN_BOTTOM) {
        y -= textHeight;
    }
    
    // Draw each character of the formatted number.
    for (int i = 0; i < textLength; i++) {
        tft.drawChar(x + i * options.size * 6, y, buf[i], options.color, options.backgroundColor, options.size);
    }
}

//-------------------------------------------------
// New drawCircle implementation using CircleDrawOptions.
//-------------------------------------------------
void Drawer::drawCircle(Adafruit_RA8875 tft, const CircleDrawOptions &options) {
    // If filled, first fill the circle.
    if (options.fill) {
        tft.fillCircle(options.centerX, options.centerY, options.radius, options.fillColor);
    }
    // Simulate stroke thickness by drawing multiple circles.
    if (options.strokeThickness > 0) {
        int halfStroke = options.strokeThickness / 2;
        for (int r = options.radius - halfStroke; r <= options.radius + halfStroke; r++) {
            if (r > 0)
                tft.drawCircle(options.centerX, options.centerY, r, options.strokeColor);
        }
    }
}

//-------------------------------------------------
// New drawRect implementation using RectDrawOptions.
//-------------------------------------------------
void Drawer::drawRect(Adafruit_RA8875 tft, const RectDrawOptions &options) {
    // Calculate top-left corner based on the anchor alignment.
    int x = options.x;
    int y = options.y;
    if (options.hAlign == ALIGN_CENTER) {
        x -= options.width / 2;
    } else if (options.hAlign == ALIGN_RIGHT) {
        x -= options.width;
    }
    if (options.vAlign == ALIGN_MIDDLE) {
        y -= options.height / 2;
    } else if (options.vAlign == ALIGN_BOTTOM) {
        y -= options.height;
    }
    
    // If rounded corners are requested, use the RA8875 round rect functions.
    if (options.cornerRadius > 0) {
        // Fill the rectangle if requested.
        if (options.fill) {
            tft.fillRoundRect(x, y, options.width, options.height, options.cornerRadius, options.fillColor);
        }
        // Draw the rounded border with simulated thickness.
        if (options.strokeThickness > 0) {
            for (int i = 0; i < options.strokeThickness; i++) {
                tft.drawRoundRect(x - i, y - i,
                                  options.width + 2 * i, options.height + 2 * i,
                                  options.cornerRadius + i,
                                  options.strokeColor);
            }
        }
    }
    // Otherwise use the standard rectangle functions.
    else {
        if (options.fill) {
            tft.fillRect(x, y, options.width, options.height, options.fillColor);
        }
        if (options.strokeThickness > 0) {
            for (int i = 0; i < options.strokeThickness; i++) {
                tft.drawRect(x - i, y - i, options.width + 2 * i, options.height + 2 * i, options.strokeColor);
            }
        }
    }
}

//-------------------------------------------------
// New drawLine implementation using LineDrawOptions.
//-------------------------------------------------
void Drawer::drawLine(Adafruit_RA8875 tft, const LineDrawOptions &options) {
    if (options.thickness <= 1) {
        tft.drawLine(options.x1, options.y1, options.x2, options.y2, options.color);
    } else {
        // Calculate a perpendicular offset vector.
        float dx = options.x2 - options.x1;
        float dy = options.y2 - options.y1;
        float len = std::sqrt(dx * dx + dy * dy);
        if (len == 0) {
            tft.drawPixel(options.x1, options.y1, options.color);
            return;
        }
        float ndx = -dy / len; // normalized perpendicular x
        float ndy = dx / len;  // normalized perpendicular y
        int halfThickness = options.thickness / 2;
        for (int offset = -halfThickness; offset <= halfThickness; offset++) {
            int ox = static_cast<int>(std::round(ndx * offset));
            int oy = static_cast<int>(std::round(ndy * offset));
            tft.drawLine(options.x1 + ox, options.y1 + oy, options.x2 + ox, options.y2 + oy, options.color);
        }
    }
}

//-------------------------------------------------
// New drawTriangle implementation using TriangleDrawOptions.
//-------------------------------------------------
void Drawer::drawTriangle(Adafruit_RA8875 tft, const TriangleDrawOptions &options) {
    if (options.fill) {
        // Assuming the underlying library supports fillTriangle.
        tft.fillTriangle(options.x1, options.y1, options.x2, options.y2, options.x3, options.y3, options.fillColor);
    }
    // Draw the outline using the drawLine method.
    LineDrawOptions line1 { options.x1, options.y1, options.x2, options.y2, options.strokeThickness, options.strokeColor };
    LineDrawOptions line2 { options.x2, options.y2, options.x3, options.y3, options.strokeThickness, options.strokeColor };
    LineDrawOptions line3 { options.x3, options.y3, options.x1, options.y1, options.strokeThickness, options.strokeColor };
    Drawer::drawLine(tft, line1);
    Drawer::drawLine(tft, line2);
    Drawer::drawLine(tft, line3);
}
