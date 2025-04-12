#include <Arduino.h>
#include <iostream>


#include "dash/dash.h"
#include "define.h"

#include "sound.h"
#include "songs.h"

Dash dashboard;

Song song{312*2, goU};
SoundDriver soundDriver{PIEZO_INPUT};

void setup() {
    // initialize serial communication, when done, turn internal LED off
    Serial.begin(9600);

    Serial.println("Starting setup");

    dashboard.initalize();
    soundDriver.initialize();

    song.shift(-2);
    soundDriver.setSong(song);
}

void loop() {
    dashboard.update();
}