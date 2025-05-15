#include "dash/rotary_encoder.h"
#include "define.h"
#include "resources.h"

void RotaryEncoder::registerPush(std::function<void()> push){

}

void RotaryEncoder::registerL(std::function<void()> left){
    // how to access highlighted?
    // highlighted = (highlighted + 1) % labels.size()
}

void RotaryEncoder::registerR(std::function<void()> right){
    // highlighted = (highlighted - 1) % labels.size()
}

void RotaryEncoder::update(){
    int rotaryleft = digitalRead(ROTARY_OUTPUT_A);
    int rotaryright = digitalRead(ROTARY_OUTPUT_B);
    //int button = digitalRead(ROTARY_BUTTON);
}