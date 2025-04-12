#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <Adafruit_RA8875.h>
#include <string>
#include <cmath>

//-------------------------------------------------
// Enums used for directional drawing and alignments.
//-------------------------------------------------
enum Direction {
    LEFT_TO_RIGHT,
    UP_TO_DOWN
};

enum HorizontalAlignment {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

enum VerticalAlignment {
    ALIGN_TOP,
    ALIGN_MIDDLE,
    ALIGN_BOTTOM
};

//-------------------------------------------------
// Option structs for text and number drawing.
//-------------------------------------------------
struct TextDrawOptions {
    int x;                   // Anchor x-position
    int y;                   // Anchor y-position
    int size;                // Font size multiplier
    int16_t color;           // Foreground (text) color
    int16_t backgroundColor; // Background color
    Direction direction = LEFT_TO_RIGHT;  // Direction to draw text
    HorizontalAlignment hAlign = ALIGN_LEFT; // Horizontal alignment of the anchor relative to text
    VerticalAlignment vAlign = ALIGN_TOP;      // Vertical alignment of the anchor relative to text
};

struct NumberDrawOptions {
    int x;                   // Anchor x-position
    int y;                   // Anchor y-position
    int size;                // Font size multiplier
    int16_t color;           // Foreground (number) color
    int16_t backgroundColor; // Background color
    int precision = 1;       // Decimal precision
    HorizontalAlignment hAlign = ALIGN_LEFT; // Horizontal alignment
    VerticalAlignment vAlign = ALIGN_TOP;      // Vertical alignment (see note below)
};

//-------------------------------------------------
// New option structs for geometric primitives.
//-------------------------------------------------

// Options for drawing a circle.
struct CircleDrawOptions {
    int centerX;             // Center x-coordinate
    int centerY;             // Center y-coordinate
    int radius;              // Radius of the circle
    bool fill = false;       // If true, fill the circle with fillColor
    int strokeThickness = 1; // Thickness (in pixels) of the outline
    uint16_t strokeColor;    // Color for the outline
    uint16_t fillColor;      // Fill color (only used if fill is true)
};

// Options for drawing a rectangle.
struct RectDrawOptions {
    int x;                   // Anchor x-position (see hAlign)
    int y;                   // Anchor y-position (see vAlign)
    int width;               // Width of the rectangle
    int height;              // Height of the rectangle
    bool fill = false;       // If true, fill the rectangle with fillColor
    int strokeThickness = 0; // Thickness (in pixels) of the outline
    uint16_t strokeColor;    // Outline color
    uint16_t fillColor;      // Fill color (only used if fill is true)
    int cornerRadius = 0;    // Corner radius for rounded corners (0 for square corners)
    HorizontalAlignment hAlign = ALIGN_LEFT; // Horizontal alignment of the anchor relative to the rectangle.
    VerticalAlignment vAlign = ALIGN_TOP;      // Vertical alignment of the anchor relative to the rectangle.
};

// Options for drawing a line.
struct LineDrawOptions {
    int x1;                  // Starting x-coordinate
    int y1;                  // Starting y-coordinate
    int x2;                  // Ending x-coordinate
    int y2;                  // Ending y-coordinate
    int thickness = 1;       // Line thickness (in pixels)
    uint16_t color;          // Line color
};

// Options for drawing a triangle.
struct TriangleDrawOptions {
    int x1;                  // First vertex x-coordinate
    int y1;                  // First vertex y-coordinate
    int x2;                  // Second vertex x-coordinate
    int y2;                  // Second vertex y-coordinate
    int x3;                  // Third vertex x-coordinate
    int y3;                  // Third vertex y-coordinate
    bool fill = false;       // If true, fill the triangle with fillColor
    int strokeThickness = 1; // Outline thickness (in pixels)
    uint16_t strokeColor;    // Outline color
    uint16_t fillColor;      // Fill color (only used if fill is true)
};

//-------------------------------------------------
// The Drawer class with legacy and new drawing methods.
//-------------------------------------------------
class Drawer {
   public:
    // Legacy methods for backward compatibility.
    static void drawString(Adafruit_RA8875 tft, std::string message, int startX, int startY, int size, int16_t color, int16_t backgroundColor, Direction dir = LEFT_TO_RIGHT);
    static void drawNum(Adafruit_RA8875 tft, float num, int startX, int startY, int size, int16_t color, int16_t backgroundColor);

    // New methods using option structures.
    static void drawString(Adafruit_RA8875 tft, std::string message, const TextDrawOptions &options);
    static void drawNum(Adafruit_RA8875 tft, float num, const NumberDrawOptions &options);

    static void drawCircle(Adafruit_RA8875 tft, const CircleDrawOptions &options);
    static void drawRect(Adafruit_RA8875 tft, const RectDrawOptions &options);
    static void drawLine(Adafruit_RA8875 tft, const LineDrawOptions &options);
    static void drawTriangle(Adafruit_RA8875 tft, const TriangleDrawOptions &options);
};

#endif  // __DRAWER_H__
