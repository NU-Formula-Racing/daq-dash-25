#include <string>
#include <vector>
#include "define.h"
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

class RotaryEncoder {
    public:

    void initialize();

     void registerPush(std::function<void()> cb); 
     void registerL(std::function<void()> cb);
     void registerR(std::function<void()> cb);
     void update();

    private:
    std::vector<std::function<void()>> onPush;
    std::vector<std::function<void()>> onLeft;
    std::vector<std::function<void()>> onRight;

    int lastA = digitalRead(ROTARY_OUTPUT_A);
    int lastButton = digitalRead(ROTARY_BUTTON);
};
#endif