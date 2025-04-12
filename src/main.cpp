#include <Arduino.h>
#include <iostream>


#include "dash/dash.h"
#include "define.h"

#include "sound.h"
#include "songs.h"

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
Dash dashboard;

Song song{312*2, goU};
SoundDriver soundDriver{PIEZO_INPUT};

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

    dashboard.initalize();
    soundDriver.initialize();
    
    song.shift(-2);
    soundDriver.setSong(song);
}

void loop() {
    dashboard.update();
}