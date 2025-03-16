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
    // turn internal LED on
    pinMode(INTERNAL_LED, OUTPUT);
    digitalWrite(INTERNAL_LED, HIGH);

    // initialize serial communication, when done, turn internal LED off
    Serial.begin(9600);
    // wait for serial to initialize
    while (!Serial) {
        delay(10);
    }

    Serial.println("Starting setup");
    // RA8875 Setup
    Serial.println("RA8875 start");
    /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
    int numAttempts = 0;
    while (!tft.begin(RA8875_800x480)) {
        numAttempts++;
        Serial.printf("Attempt %d to initialize RA8875 failed\n", numAttempts);
        delay(500);
    }

    Serial.println("Found RA8875");

    // write high to the lite pin to turn on the backlight
    pinMode(RA8875_LITE, OUTPUT);
    digitalWrite(RA8875_LITE, HIGH);

    tft.displayOn(true);
    tft.GPIOX(true);                               // Enable TFT - display enable tied to GPIOX
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);  // PWM output for backlight
    tft.PWM1out(255);
    // Initialize dashboard
    // disalbe interrupts

    dashboard.Initialize();
    // With hardware accelleration this is instant
    dashboard.DrawBackground(tft);
}

void loop() {
    // put your main code here, to run repeatedly:
    dashboard.GetCAN();
    dashboard.UpdateDisplay(tft);
}