#include <stdint.h>

#define BATTERY_CHARGING 0
#define INCORRECT_VOLTAGE 1
#define INCORRECT_CURRENT 2
#define BATTERY_FULL 3

typedef struct 
{
    double maxCapacity;
    double minBatteryVoltage;
    double maxBatteryVoltage;
    double currentBatteryVoltage;
    double maxBatteryCurrent;

    double chargingVoltage;
    double maxChargingCurrent;

    uint8_t isChargerConnected;
} BatteryManager;

double getBatteryVoltage(BatteryManager* batteryManager);
double getBatteryCapacity(BatteryManager* batteryManager);
double getStateOfCharge(BatteryManager* batteryManager);
uint8_t connectCharger(BatteryManager* batteryManager, double voltage, double current);
void disconnectCharger(BatteryManager* batteryManager);
uint8_t chargeBattery(BatteryManager* batteryManager, double voltage, double current);
uint8_t unchargeBattery(BatteryManager* batteryManager, double voltage, double current);