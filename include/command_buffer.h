#ifndef __COMMAND_BUFFER_H__
#define __COMMAND_BUFFER_H__

#define COMMAND_BUFFER_SIZE 30

#include <stdint.h>
#include <string>

#include "Adafruit_RA8875.h"

enum DrawType {
    RECTANGLE,
    RECTANGLE_OUTLINE,
    CIRCLE,
    CIRCLE_OUTLINE,
    TEXT,
    FILL_SCREEN
};

struct RectangleOptions {
    int x;
    int y;
    int width;
    int height;
    uint16_t color;
};

struct CircleOptions {
    int x;
    int y;
    int radius;
    uint16_t color;
};

enum TextDirection {
    LEFT_TO_RIGHT,
    UP_TO_DOWN,
};

struct TextOptions {
    int x;
    int y;
    int size;
    uint16_t color;
    char *text;
    TextDirection direction;
};

struct FillOptions {
    uint16_t color;
};

class DrawCommand {
   public:
    DrawType type;
    union {
        RectangleOptions rectangle;
        CircleOptions circle;
        TextOptions options;
        FillOptions fill;
    } options;

    void draw(Adafruit_RA8875 &screen);

    static DrawCommand fillRect(int x, int y, int width, int height, uint16_t color) {
        DrawCommand command;
        command.type = RECTANGLE;
        command.options.rectangle.x = x;
        command.options.rectangle.y = y;
        command.options.rectangle.width = width;
        command.options.rectangle.height = height;
        command.options.rectangle.color = color;
        return command;
    }

    static DrawCommand drawRect(int x, int y, int width, int height, uint16_t color) {
        DrawCommand command;
        command.type = RECTANGLE_OUTLINE;
        command.options.rectangle.x = x;
        command.options.rectangle.y = y;
        command.options.rectangle.width = width;
        command.options.rectangle.height = height;
        command.options.rectangle.color = color;
        return command;
    }

    static DrawCommand fillCircle(int x, int y, int radius, uint16_t color) {
        DrawCommand command;
        command.type = CIRCLE;
        command.options.circle.x = x;
        command.options.circle.y = y;
        command.options.circle.radius = radius;
        command.options.circle.color = color;
        return command;
    }
    static DrawCommand drawCircle(int x, int y, int radius, uint16_t color) {
        DrawCommand command;
        command.type = CIRCLE_OUTLINE;
        command.options.circle.x = x;
        command.options.circle.y = y;
        command.options.circle.radius = radius;
        command.options.circle.color = color;
        return command;
    }

    static DrawCommand drawText(int x, int y, int size, uint16_t color, char *text, TextDirection direction) {
        DrawCommand command;
        command.type = TEXT;
        command.options.options.x = x;
        command.options.options.y = y;
        command.options.options.size = size;
        command.options.options.color = color;
        command.options.options.text = text;
        command.options.options.direction = direction;
        return command;
    }

    static DrawCommand fillScreen(uint16_t color) {
        DrawCommand command;
        command.type = FILL_SCREEN;
        command.options.fill.color = color;
        return command;
    }

    DrawCommand() {}
    ~DrawCommand() {}
};

class CommandBuffer {
   public:
    void addCommand(DrawCommand command);
    void addCommands(std::vector<DrawCommand> commands);
    void drawNext(Adafruit_RA8875 &screen);

    CommandBuffer() : headIndex(0), tailIndex(0) {}
    ~CommandBuffer() {}

   private:
    DrawCommand commands[COMMAND_BUFFER_SIZE];
    size_t headIndex;
    size_t tailIndex;
};

#endif  // __COMMAND_BUFFER_H__