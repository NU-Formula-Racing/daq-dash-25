#include <Arduino.h>
#include <iostream>

#include "resources.h"
#include "dash/dash.h"
#include "define.h"
#include "sound.h"
#include "songs.h"

Dash dashboard;
Logger logger;

Song song{312*2, goU};

VirtualTimerGroup loggingTimer;

void logData() {
    // logger.log();
}

void setup() {
    // initialize serial communication, when done, turn internal LED off
    Serial.begin(9600);
    Serial.println("Starting setup");

    dashboard.initalize();
    logger.initialize();

    // initialize sound driver
    song.shift(-2);
    Resources::instance().soundDriver.initialize();
    Resources::instance().soundDriver.setSong(song);

    Resources::instance().dataBus.initialize();
    Resources::instance().driveBus.initialize();

    // loggingTimer.AddTimer(1000, logData);
}

void loop() {
    Resources::instance().update();
    dashboard.update();
    // logging takes wayy too long right now
    // loggingTimer.Tick(millis());
}