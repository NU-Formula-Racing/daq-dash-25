#include <Arduino.h>
#include <dash.h>
#include <pins_arduino.h>

#include <iostream>

#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "define.h"

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
Dash dashboard;


void setup() {
    // initialize serial communication, when done, turn internal LED off
    Serial.begin(9600);

    Serial.println("Starting setup");
    // RA8875 Setup
    Serial.println("RA8875 start");

    int numAttempts = 0;
    while (!tft.begin(RA8875_800x480)) {
        numAttempts++;
        Serial.printf("Attempt %d to initialize RA8875 failed\n", numAttempts);
        delay(500);
    }

    Serial.println("Found RA8875");

    tft.displayOn(true);
    tft.GPIOX(true);                               // Enable TFT - display enable tied to GPIOX
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);  // PWM output for backlight
    tft.PWM1out(255);
    dashboard.Initialize();
    dashboard.DrawBackground(tft);
}

void loop() {
    dashboard.GetCAN();
    dashboard.UpdateDisplay(tft);
}