#include "dash/rotary_encoder.h"
#include "define.h"
#include "resources.h"
#include <vector>
#include <iostream>

#include <Arduino.h>

volatile bool left = false;
volatile bool right = false;
volatile bool button = false;

static void onRisingEdgeLeft()
{
    left = true;
}

static void onRisingEdgeRight()
{
    right = true;
}

static void onRisingEdgeButton()
{
    button = true;
}

void RotaryEncoder::registerPush(std::function<void()> cb)
{
    onPush.push_back(cb);
}

void RotaryEncoder::registerL(std::function<void()> cb)
{
    onLeft.push_back(cb);
}

void RotaryEncoder::registerR(std::function<void()> cb)
{
    onRight.push_back(cb);
}

void RotaryEncoder::initialize()
{
    Serial.println("rotary encoder initialize");

    attachInterrupt(
        digitalPinToInterrupt(ROTARY_OUTPUT_A),
        onRisingEdgeLeft,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(ROTARY_OUTPUT_B),
        onRisingEdgeRight,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(ROTARY_BUTTON),
        onRisingEdgeRight,
        RISING);
}

void RotaryEncoder::update()
{
    int rotaryleft = digitalRead(ROTARY_OUTPUT_A);
    int rotaryright = digitalRead(ROTARY_OUTPUT_B);
    int button = digitalRead(ROTARY_BUTTON);

    if (left)
    {
        for (auto fn : onLeft) {
            fn();
        }
        Serial.begin(9600);
        Serial.println("left");
        left = false;
    }

    if (right)
    {
        for (auto fn : onRight) {
            fn();
        }
        Serial.begin(9600);
        Serial.println("right");
        right = false;
    }

    if (button)
    {
        for (auto fn : onPush) {
            fn();
        }
        Serial.begin(9600);
        Serial.println("button");
        button = false;
    }
}
