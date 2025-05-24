#include <string>
#include <vector>
#include "define.h"
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H
class RotaryEncoder {
    public:
     void registerPush(std::function<void()> cb); 
     void registerL(std::function<void()> cb);
     void registerR(std::function<void()> cb);
     void update();

    private:
    std::function<void()> onPush;
    std::function<void()> onLeft;
    std::function<void()> onRight;

    int lastA = 0;
    int lastB = 0;
    int lastButton = 0;
};
#endif