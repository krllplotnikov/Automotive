#include "BatteryManager.h"
#include <time.h>

double interpolation(float x1, float x2, float fx1, float fx2, float x){
	return fx1 + (x - x1)*((fx2 - fx1)/(x2 - x1));
}

double getBatteryVoltage(BatteryManager *batteryManager)
{
    return batteryManager->currentBatteryVoltage;
}

double getStateOfCharge(BatteryManager *batteryManager)
{
    return interpolation(batteryManager->minBatteryVoltage, batteryManager->maxBatteryVoltage, 0, 100, batteryManager->currentBatteryVoltage);
}

double getBatteryCapacity(BatteryManager *batteryManager)
{
    return batteryManager->maxCapacity;
}

uint8_t connectCharger(BatteryManager *batteryManager, double voltage, double current)
{
    uint8_t returnCode = 0;
    if (voltage < (batteryManager->chargingVoltage - 0.1) || voltage > (batteryManager->chargingVoltage + 0.1))
    {
        printf("Incorrect charging voltage\n");
        returnCode = INCORRECT_VOLTAGE;
        goto Exit;
    }
    else if (current > batteryManager->maxChargingCurrent)
    {
        printf("Incorrect charging current\n");
        returnCode = INCORRECT_CURRENT;
        goto Exit;
    }
    else if (batteryManager->currentBatteryVoltage >= batteryManager->maxBatteryVoltage)
    {
        printf("Battery is full\n");
        returnCode = BATTERY_FULL;
        goto Exit;
    }
    else
    {
        printf("\rCharger connected\n");
        batteryManager->isChargerConnected = 1;
    }

Exit:
    return returnCode;
}

void disconnectCharger(BatteryManager *batteryManager)
{
    printf("\rCharger disconnected\n");
    batteryManager->isChargerConnected = 0;
}

uint8_t chargeBattery(BatteryManager *batteryManager, double voltage, double current)
{
    uint8_t returnCode = 0;
    static time_t sec = 0;
    if (batteryManager->isChargerConnected)
    {
        if (voltage < (batteryManager->chargingVoltage - 0.1) || voltage > (batteryManager->chargingVoltage + 0.1))
        {
            returnCode = INCORRECT_VOLTAGE;
            goto Exit;
        }
        else if (current > batteryManager->maxChargingCurrent)
        {
            returnCode = INCORRECT_CURRENT;
            goto Exit;
        }
        else if (batteryManager->currentBatteryVoltage >= batteryManager->maxBatteryVoltage)
        {
            returnCode = BATTERY_FULL;
            goto Exit;
        }
        else
        {
            if (time(NULL) > sec)
            {
                batteryManager->currentBatteryVoltage += current / batteryManager->maxCapacity  * 0.0002777;
                sec = time(NULL);
                returnCode = BATTERY_CHARGING;
            }
        }
    }

Exit:
    return returnCode;
}