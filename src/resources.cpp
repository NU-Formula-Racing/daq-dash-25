#include "resources.h"

Resources& Resources::instance() {
    static Resources r;
    return r;
}

const DataBusData &Resources::dataBusData() {
    return instance().dataBus.getData();
}

const DriveBusData &Resources::driveBusData() {
    return instance().driveBus.getData();
}

const DriveBusData &Resources::prevDriveBusData() {
    return instance().driveBus.getPrevData();
}

float Resources::deltaTimeMs() {
    return instance()._deltaTime;
}

Resources::Resources() : soundDriver(PIEZO_INPUT), dataBus(), driveBus() {
}



void Resources::update() {
    dataBus.update();
    driveBus.update();
    
    long long now = millis();
    _deltaTime = now - _lastTime;
    _lastTime = now;
}
