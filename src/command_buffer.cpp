#include "command_buffer.h"

#include <iostream>
void DrawCommand::draw(Adafruit_RA8875 &screen) {
    switch (this->type) {
        case RECTANGLE:
            screen.fillRect(this->options.rectangle.x, this->options.rectangle.y, this->options.rectangle.width, this->options.rectangle.height, this->options.rectangle.color);
            break;
        case RECTANGLE_OUTLINE:
            screen.drawRect(this->options.rectangle.x, this->options.rectangle.y, this->options.rectangle.width, this->options.rectangle.height, this->options.rectangle.color);
            break;
        case CIRCLE:
            screen.fillCircle(this->options.circle.x, this->options.circle.y, this->options.circle.radius, this->options.circle.color);
            break;
        case CIRCLE_OUTLINE:
            screen.drawCircle(this->options.circle.x, this->options.circle.y, this->options.circle.radius, this->options.circle.color);
            break;
        case TEXT:
            // std::cout << "Drawing text is not implemented yet!" << std::endl;
            screen.textMode();
            screen.textSetCursor(this->options.options.x, this->options.options.y);
            screen.textEnlarge(this->options.options.size);
            screen.textColor(this->options.options.color, RA8875_BLACK);
            screen.textWrite(this->options.options.text);
            break;
        case FILL_SCREEN:
            screen.fillScreen(this->options.fill.color);
            break;
    }
}

void CommandBuffer::addCommand(DrawCommand command) {
    this->commands[this->tailIndex] = command;
    this->tailIndex = (this->tailIndex + 1) % COMMAND_BUFFER_SIZE;
}

void CommandBuffer::addCommands(std::vector<DrawCommand> commands) {
    for (DrawCommand command : commands) {
        this->addCommand(command);
    }
}

void CommandBuffer::drawNext(Adafruit_RA8875 &screen) {
    if (this->headIndex == this->tailIndex) {
        return;
    }

    this->commands[this->headIndex].draw(screen);
    this->headIndex = (this->headIndex + 1) % COMMAND_BUFFER_SIZE;
}