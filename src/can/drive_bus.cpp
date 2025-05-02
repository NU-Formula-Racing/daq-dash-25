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

void DriveBus::sendLUTCan(uint8_t numPairs, uint8_t interpType, uint8_t LUTId, std::map<int16_t, float> lut) {
    this->num_lut_pairs = numPairs;
    this->interp_type = interpType;
    this->lut_id = LUTId;
    std::vector<int16_t> x_values;
    std::vector<float> y_values;
    for (const auto& [key, value] : lut) {
        x_values.push_back(key);
        y_values.push_back(value);
    }
    for (int i = 0 ; i < MAX_THROTTLE_LUT_PAIRS - numPairs ; i++) {
        x_values.push_back(0);
        y_values.push_back(0.0f);
    }
    this->x_zero = x_values.at(0);
    this->x_one = y_values.at(1);
    this->x_two = x_values.at(2);
    this->x_three = y_values.at(3);
    this->x_four = x_values.at(4);
    this->x_five = y_values.at(5);
    this->x_six = x_values.at(6);
    this->x_seven = y_values.at(7);
    this->x_eight = x_values.at(8);
    this->x_nine = y_values.at(9);
    this->x_ten = x_values.at(10);
    this->x_eleven = x_values.at(11);
    this->x_twelve = x_values.at(12);
    this->x_thirteen = x_values.at(13);
    this->x_fourteen = x_values.at(14);
    this->x_fifteen = x_values.at(15);
    this->x_sixteen = x_values.at(16);
    this->x_seventeen = x_values.at(17);
    this->x_eighteen = x_values.at(18);
    this->x_nineteen = x_values.at(19);
    this->x_twenty = x_values.at(20);
    this->x_twenty_one = x_values.at(21);
    this->x_twenty_two = x_values.at(22);
    this->x_twenty_three = x_values.at(23);
    this->x_twenty_four = x_values.at(24);
    this->x_twenty_five = x_values.at(25);
    this->x_twenty_six = x_values.at(26);
    this->x_twenty_seven = x_values.at(27);
    this->x_twenty_eight = x_values.at(28);
    this->x_twenty_nine = x_values.at(29);

    this->y_zero = y_values.at(0);
    this->y_one = y_values.at(1);
    this->y_two = y_values.at(2);
    this->y_three = y_values.at(3);
    this->y_four = y_values.at(4);
    this->y_five = y_values.at(5);
    this->y_six = y_values.at(6);
    this->y_seven = y_values.at(7);
    this->y_eight = y_values.at(8);
    this->y_nine = y_values.at(9);
    this->y_ten = y_values.at(10);
    this->y_eleven = y_values.at(11);
    this->y_twelve = y_values.at(12);
    this->y_thirteen = y_values.at(13);
    this->y_fourteen = y_values.at(14);
    this->y_fifteen = y_values.at(15);
    this->y_sixteen = y_values.at(16);
    this->y_seventeen = y_values.at(17);
    this->y_eighteen = y_values.at(18);
    this->y_nineteen = y_values.at(19);
    this->y_twenty = y_values.at(20);
    this->y_twenty_one = y_values.at(21);
    this->y_twenty_two = y_values.at(22);
    this->y_twenty_three = y_values.at(23);
    this->y_twenty_four = y_values.at(24);
    this->y_twenty_five = y_values.at(25);
    this->y_twenty_six = y_values.at(26);
    this->y_twenty_seven = y_values.at(27);
    this->y_twenty_eight = y_values.at(28);
    this->y_twenty_nine = y_values.at(29);
}