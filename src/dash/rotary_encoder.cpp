#include "dash/rotary_encoder.h"
#include "define.h"
#include "resources.h"

void RotaryEncoder::registerPush(std::function<void()> cb){
    onPush = cb;
}

void RotaryEncoder::registerL(std::function<void()> cb){
    onLeft = cb;
}

void RotaryEncoder::registerR(std::function<void()> cb){
    onRight = cb;
}

void RotaryEncoder::update(){
    int rotaryleft = digitalRead(ROTARY_OUTPUT_A);
    int rotaryright = digitalRead(ROTARY_OUTPUT_B);
    int button = digitalRead(ROTARY_BUTTON);
    if (rotaryleft != lastA) {
        if (rotaryright != rotaryleft) {
            if (onRight) onRight();
        } else {
            if (onLeft) onLeft();
        }
    }
    lastA = rotaryleft;
    lastB = rotaryright;

    // Detect button press (simple debounce logic can be added)
    if (button == LOW && lastButton == HIGH) {
        if (onPush) onPush();
    }
    lastButton = button;
}

