#include "can/drive_bus.h"

#include "define.h"
#include "resources.h"

const DriveBusData& DriveBus::getData() const {
    return _data;
}

const DriveBusData& DriveBus::getPrevData() const {
    return _prevData;
}

void DriveBus::initialize() {
    _driveBus.Initialize(ICAN::BaudRate::kBaud500K);

    _driveBus.RegisterRXMessage(rx_fl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_fr_wheel_speed);
    _driveBus.RegisterRXMessage(rx_bl_wheel_speed);
    _driveBus.RegisterRXMessage(rx_br_wheel_speed);
    _driveBus.RegisterRXMessage(rx_drive_state);
    _driveBus.RegisterRXMessage(rx_hv_battery);
    _driveBus.RegisterRXMessage(rx_lv_voltage);
    _driveBus.RegisterRXMessage(rx_bms_faults);
    _driveBus.RegisterRXMessage(rx_ecu_implausibility);
    _driveBus.RegisterRXMessage(rx_bms_status);
    _driveBus.RegisterRXMessage(rx_inverter_fault_status);
    _driveBus.RegisterRXMessage(rx_inverter_motor_status);

    // lowkey mad annoying but we gotta pull the imd status to be high
    bms_status_imd_state = 1; // drake why can't you be normal
}

// Helper: Generate a random float between min and max.
static float randomFloat(float min, float max) {
    // Generate a value between 0 and 9999, then scale
    long r = random(0, 10000);
    return min + (max - min) * (r / 10000.0);
}

void DriveBus::update() {
    this->_driveBus.Tick();
    // update the previous data
    this->_prevData = this->_data;

#ifdef DRIVE_DEBUG
    // Initialize debug timers if not set
    unsigned long now = millis();

    if (this->_debugStartTime == 0) {
        this->_debugStartTime = now;
        this->_debugLastFaultUpdate = now;
    }

    // Use random values for all numeric fields
    for (int i = 0; i < 4; i++) {
        // Set each wheel speed to a random value (for example, 0 to 100 RPM)
        this->_data.wheelSpeeds[i] = randomFloat(0.0f, 100.0f);
    }
    this->_data.driveState = random(0, 3);
    this->_data.HVVoltage = randomFloat(300.0f, 400.0f);
    this->_data.LVVoltage = randomFloat(12.0f, 15.0f);
    this->_data.batteryTemp = randomFloat(20.0f, 40.0f);
    this->_data.maxCellTemp = randomFloat(25.0f, 50.0f);
    this->_data.minCellTemp = randomFloat(15.0f, 30.0f);

    // Now update the fault arrays.
    // For the first 5 seconds, leave all fault flags false.
    if (now - this->_debugStartTime < 500UL) {
        for (int i = 0; i < BMS_FAULT_COUNT; i++) {
            this->_data.bmsFaults[i] = false;
        }
        for (int i = 0; i < ECU_FAULT_COUNT; i++) {
            this->_data.ecuFaults[i] = false;
        }
    }
    // After 5 seconds, every 10 seconds randomly update the fault flags.
    else if (now - this->_debugLastFaultUpdate >= 10000UL) {
        for (int i = 0; i < BMS_FAULT_COUNT; i++) {
            // Randomly set each BMS fault flag (true or false)
            this->_data.bmsFaults[i] = (random(0, 2) == 1);
        }
        for (int i = 0; i < ECU_FAULT_COUNT; i++) {
            // Randomly set each ECU fault flag
            this->_data.ecuFaults[i] = (random(0, 2) == 1);
        }
        this->_debugLastFaultUpdate = now;
    }

#else
    // Regular non-debug update: assign all signals as provided.
    // this->_data.wheelSpeeds[0] = fl_wheel_speed_signal;
    // this->_data.wheelSpeeds[1] = fr_wheel_speed_signal;
    // this->_data.wheelSpeeds[2] = bl_wheel_speed_signal;
    // this->_data.wheelSpeeds[3] = br_wheel_speed_signal;

    this->_data.wheelSpeeds[0] = (float)inverter_motor_status_rpm;
    this->_data.wheelSpeeds[1] = (float)inverter_motor_status_rpm;
    this->_data.wheelSpeeds[2] = (float)inverter_motor_status_rpm;
    this->_data.wheelSpeeds[3] = (float)inverter_motor_status_rpm;

    this->_data.driveState = drive_state_signal;
    this->_data.HVVoltage = hv_voltage_signal;
    this->_data.LVVoltage = lv_voltage_signal;
    this->_data.bmsState = bms_status_bms_state;
    if (this->_data.imdState == 0) // latch this
        this->_data.imdState = bms_status_imd_state;
    this->_data.maxCellTemp = bms_status_max_cell_temp;
    this->_data.minCellTemp = bms_status_min_cell_temp;
    this->_data.maxCellVoltage = bms_status_max_cell_voltage;
    this->_data.minCellVoltage = bms_status_min_cell_voltage;
    this->_data.bmsSOC = bms_status_bms_soc;

    this->_data.inverterStatus = inverter_fault_status_fault_code_signal;

    this->_data.bmsFaults[BMS_FAULT_SUMMARY] = (static_cast<bool>(bms_fault_summary_signal));
    this->_data.bmsFaults[BMS_FAULT_UNDER_VOLTAGE] = static_cast<bool>(bms_fault_under_voltage_signal);
    this->_data.bmsFaults[BMS_FAULT_OVER_VOLTAGE] = static_cast<bool>(bms_fault_over_voltage_signal);
    this->_data.bmsFaults[BMS_FAULT_UNDER_TEMP] = static_cast<bool>(bms_fault_under_temperature_signal);
    this->_data.bmsFaults[BMS_FAULT_OVER_TEMP] = static_cast<bool>(bms_fault_over_temperature_signal);
    this->_data.bmsFaults[BMS_FAULT_OVER_CURRENT] = static_cast<bool>(bms_fault_over_current_signal);
    this->_data.bmsFaults[BMS_FAULT_EXTERNAL_KILL] = static_cast<bool>(bms_fault_external_kill_signal);
    this->_data.bmsFaults[BMS_FAULT_OPEN_WIRE] = static_cast<bool>(bms_fault_open_wire_signal);

    this->_data.ecuFaults[ECU_FAULT_PRESENT] = static_cast<bool>(ecu_implausibility_present_signal);
    this->_data.ecuFaults[ECU_FAULT_APPSS_DISAGREEMENT] = static_cast<bool>(ecu_implausibility_appss_disagreement_imp_signal);
    this->_data.ecuFaults[ECU_FAULT_BPPC] = static_cast<bool>(ecu_implausibility_bppc_imp_signal);
    this->_data.ecuFaults[ECU_FAULT_BRAKE_INVALID] = static_cast<bool>(ecu_implausibility_brake_invalid_imp_signal);
    this->_data.ecuFaults[ECU_FAULT_APPPS_INVALID] = static_cast<bool>(ecu_implausibility_appss_invalid_imp_signal);

    this->_data.LVVoltage = static_cast<float>(lv_voltage_signal);

#endif
}

void DriveBus::playReadyToDriveSound() {
    // check if we are changing into neutral

    uint8_t current = drive_state_signal;

    // are we channging into ON?
    if (current != DriveState::DS_ON) {
        return;  // no need to play the sound
    }

    // we only play the sound if we are transitioning from neutral to on 
    // have to check current and previous, just in case
    // this is called during an interrupt
    if (Resources::driveBusData().driveState == DriveState::DS_NEUTRAL ||
        Resources::prevDriveBusData().driveState == DriveState::DS_NEUTRAL) {
        // we must be transitioining neutral -> on
        // Serial.println("Playing ready to drive!");
        Resources::instance().soundDriver.playSong();
    }
}

void DriveBus::sendLUTCan(bool filesPresent, uint8_t numPairs, uint8_t interpType, uint8_t LUTId, std::vector<int16_t> xVals, std::vector<float> yVals) {

    if (!filesPresent) {
        this->files_present = false;
        this->num_lut_pairs = 0;
        this->interp_type = 0;
        this->lut_id = 0;
    } else {
        this->files_present = true;
        this->num_lut_pairs = numPairs;
        this->interp_type = interpType;
        this->lut_id = LUTId;

    }

    this->x_zero = xVals.at(0);
    this->x_one = xVals.at(1);
    this->x_two = xVals.at(2);
    this->x_three = xVals.at(3);
    this->x_four = xVals.at(4);
    this->x_five = xVals.at(5);
    this->x_six = xVals.at(6);
    this->x_seven = xVals.at(7);
    this->x_eight = xVals.at(8);
    this->x_nine = xVals.at(9);
    this->x_ten = xVals.at(10);
    this->x_eleven = xVals.at(11);
    this->x_twelve = xVals.at(12);
    this->x_thirteen = xVals.at(13);
    this->x_fourteen = xVals.at(14);
    this->x_fifteen = xVals.at(15);
    this->x_sixteen = xVals.at(16);
    this->x_seventeen = xVals.at(17);
    this->x_eighteen = xVals.at(18);
    this->x_nineteen = xVals.at(19);
    this->x_twenty = xVals.at(20);
    this->x_twenty_one = xVals.at(21);
    this->x_twenty_two = xVals.at(22);
    this->x_twenty_three = xVals.at(23);
    this->x_twenty_four = xVals.at(24);
    this->x_twenty_five = xVals.at(25);
    this->x_twenty_six = xVals.at(26);
    this->x_twenty_seven = xVals.at(27);
    this->x_twenty_eight = xVals.at(28);
    this->x_twenty_nine = xVals.at(29);

    this->y_zero = yVals.at(0);
    this->y_one = yVals.at(1);
    this->y_two = yVals.at(2);
    this->y_three = yVals.at(3);
    this->y_four = yVals.at(4);
    this->y_five = yVals.at(5);
    this->y_six = yVals.at(6);
    this->y_seven = yVals.at(7);
    this->y_eight = yVals.at(8);
    this->y_nine = yVals.at(9);
    this->y_ten = yVals.at(10);
    this->y_eleven = yVals.at(11);
    this->y_twelve = yVals.at(12);
    this->y_thirteen = yVals.at(13);
    this->y_fourteen = yVals.at(14);
    this->y_fifteen = yVals.at(15);
    this->y_sixteen = yVals.at(16);
    this->y_seventeen = yVals.at(17);
    this->y_eighteen = yVals.at(18);
    this->y_nineteen = yVals.at(19);
    this->y_twenty = yVals.at(20);
    this->y_twenty_one = yVals.at(21);
    this->y_twenty_two = yVals.at(22);
    this->y_twenty_three = yVals.at(23);
    this->y_twenty_four = yVals.at(24);
    this->y_twenty_five = yVals.at(25);
    this->y_twenty_six = yVals.at(26);
    this->y_twenty_seven = yVals.at(27);
    this->y_twenty_eight = yVals.at(28);
    this->y_twenty_nine = yVals.at(29);
}

void DriveBus::tickTimerCANTx() {
    _timerGroup.Tick(millis());
}