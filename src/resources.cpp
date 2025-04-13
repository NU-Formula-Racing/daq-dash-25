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

Resources::Resources() : soundDriver(PIEZO_INPUT) {
}


void Resources::update() {
    dataBus.update();
    driveBus.update();
}
