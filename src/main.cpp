#include <Arduino.h>

#include <iostream>

#include "dash/dash.h"
#include "define.h"
#include "resources.h"
#include "songs.h"
#include "sound.h"

Dash dashboard;
Logger logger;

Song song{312 * 3, goU};

VirtualTimerGroup loggingTimer;

void logData() {
    // logger.log();
}

void setup() {
    // initialize serial communication, when done, turn internal LED off
    Serial.begin(9600);
    Serial.println("Starting setup");

    dashboard.initalize();
    // logger.initialize();

    // initialize sound driver
    song.shift(-2);
    Resources::instance().soundDriver.initialize();
    Resources::instance().soundDriver.setSong(song);
    // Resources::instance().soundDriver.playSong();

    Resources::instance().dataBus.initialize();
    Resources::instance().driveBus.initialize();

    // loggingTimer.AddTimer(1000, logData);
}

void loop() {
    Resources::instance().update();
    dashboard.update();

    // kind of a work around for the sound driver, cause it is not async
    if (Resources::instance().soundDriver.getState() == SoundDriverState::S_PLAYING) {
        // keep progressing the song
        Resources::instance().soundDriver.playSong();
    }

    // logging takes wayy too long right now
    // loggingTimer.Tick(millis());
}