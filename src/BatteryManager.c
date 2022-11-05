#include "BatteryManager.h"
#include <time.h>

double getBatteryVoltage() {
    return 0;
}

double getStateOfCharge(BatteryManager* batteryManager) {
    return batteryManager -> currentCapacity / batteryManager -> maxCapacity * 100;
}

double getBatteryCapacity(BatteryManager* batteryManager) {
    return batteryManager -> currentCapacity;
}

void connectCharger(BatteryManager* batteryManager) {
    batteryManager -> isCharging = 1;
}

void disconnectCharger(BatteryManager* batteryManager) {
    batteryManager -> isCharging = 0;
}

uint8_t chargeBattery(BatteryManager* batteryManager, double voltage, double current) {
    uint8_t returnCode = 0;
    static time_t sec = 0;
    if (batteryManager -> isCharging) {
        if (voltage < (batteryManager -> chargingVoltage - 0.1) || voltage >(batteryManager -> chargingVoltage + 0.1)) {
            returnCode = INCORRECT_VOLTAGE;
            goto Exit;
        }
        else if (current > batteryManager -> maxChargingCurrent) {
            returnCode = INCORRECT_CURRENT;
            goto Exit;
        }
        else if (batteryManager -> currentCapacity >= batteryManager -> maxCapacity) {
            returnCode = BATTERY_FULL;
            goto Exit;
        }
        else {
            if (time(NULL) > sec) {
                batteryManager -> currentCapacity += current * 0.001;
                sec = time(NULL);
                returnCode = BATTERY_CHARGING;
            }
        }
    }

Exit:
    return returnCode;
}