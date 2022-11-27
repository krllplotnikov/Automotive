#include <stdint.h>

typedef enum
{
    BATTERY_CHARGING,
    BATTERY_UNCHARGING,
    INCORRECT_VOLTAGE,
    INCORRECT_CURRENT,
    BATTERY_FULL,
    BATTERY_LOW,
    CHARGER_CONNECTED,
    CHARGER_DISCONNECTED,
    LOAD_CONNECTED,
    LOAD_DISCONNECTED
} BatteryManager_Responses;

typedef struct
{
    double capacity;
    double nominalVoltage;
    double minVoltage;
    double maxVoltage;
    double currentVoltage;
    double maxOutputCurrent;

    double chargingVoltage;
    double maxChargingCurrent;

    uint8_t isChargerConnected;
    uint8_t isLoadConnected;
} BatteryManager;
typedef struct
{
    float engine;
    float ligth;
    float heater;
    float cooler;
}Power;

double getBatteryVoltage(BatteryManager* batteryManager);
double getBatteryCapacity(BatteryManager* batteryManager);
double getStateOfCharge(BatteryManager* batteryManager);
float type_econom(BatteryManager* battery, Power* curr, float level1);
void time_work(BatteryManager* battery, double P, float level);


uint8_t connectCharger(BatteryManager *batteryManager, double voltage, double current);
uint8_t disconnectCharger(BatteryManager *batteryManager);
uint8_t chargeBattery(BatteryManager *batteryManager, double voltage, double current);

uint8_t connectLoad(BatteryManager *batteryManager, double voltage, double current);
uint8_t disconnectLoad(BatteryManager *batteryManager);
uint8_t unchargeBattery(BatteryManager *batteryManager, double voltage, double current);