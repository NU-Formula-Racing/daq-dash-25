#include <Arduino.h>

#include <iostream>

#include "dash/dash.h"
#include "define.h"
#include "resources.h"
#include "songs.h"
#include "sound.h"

Song song{312 * 2, goU};

VirtualTimerGroup loggingTimer;

long long lastUpdateTime = 0;

void logData() {
    // Serial.println("Logging timer func!");
    Resources::instance().logger.log();
    Resources::instance().logger.writeMileCounter();
}

void setup() {
    // initialize serial communication, when done, turn internal LED off
    Serial.begin(9600);
    pinMode(ROTARY_OUTPUT_A, INPUT);
    pinMode(ROTARY_OUTPUT_B, INPUT);
    pinMode(ROTARY_BUTTON, INPUT);
    Serial.println("Starting setup");

    
    Resources::instance().logger.initialize();
    Resources::instance().milageCounter = Resources::instance().logger.readMileCounter();
    
    // initialize sound driver
    song.shift(-2);
    Resources::instance().soundDriver.initialize();
    Resources::instance().soundDriver.setSong(song);
    // Resources::instance().soundDriver.playSong();
    
    Resources::instance().dataBus.initialize();
    Resources::instance().driveBus.initialize();
    
    Resources::instance().dash.initalize();
    
    loggingTimer.AddTimer(1000, logData);
}

void loop() {
    long long currentTime = millis();

    if(currentTime - lastUpdateTime >= 50){
        Resources::instance().update();
        Resources::instance().dash.update();
        lastUpdateTime = currentTime;
    }

    // logging takes wayy too long right now
    loggingTimer.Tick(millis());

    // kind of a work around for the sound driver, cause it is not async
    if (Resources::instance().soundDriver.getState() == SoundDriverState::S_PLAYING) {
        // keep progressing the song
        Resources::instance().soundDriver.playSong();
    }
}