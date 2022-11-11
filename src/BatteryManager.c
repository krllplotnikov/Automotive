#include "BatteryManager.h"
#include <time.h>

double interpolation(float x1, float x2, float fx1, float fx2, float x)
{
    return fx1 + (x - x1) * ((fx2 - fx1) / (x2 - x1));
}

double getBatteryVoltage(BatteryManager *batteryManager)
{
    return batteryManager->currentVoltage;
}

double getStateOfCharge(BatteryManager *batteryManager)
{
    return interpolation(batteryManager->minVoltage, batteryManager->maxVoltage, 0, 100, batteryManager->currentVoltage);
}

double getBatteryCapacity(BatteryManager *batteryManager)
{
    return batteryManager->capacity;
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
    else if (batteryManager->currentVoltage >= batteryManager->maxVoltage)
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
    double currentCurrent = current;
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
        else
        {
            if (time(NULL) > sec)
            {
                if (getStateOfCharge(batteryManager) < 5)
                {
                    if (current > batteryManager->maxChargingCurrent / 10)
                        currentCurrent = batteryManager->maxChargingCurrent / 10;
                    else
                        currentCurrent = current;
                }
                if (getStateOfCharge(batteryManager) > 5 && getStateOfCharge(batteryManager) < 85)
                {
                    currentCurrent = current;
                }
                if (getStateOfCharge(batteryManager) > 85)
                {
                    if (current > batteryManager->maxChargingCurrent / 5)
                        currentCurrent = batteryManager->maxChargingCurrent / 5;
                    else
                        currentCurrent = current;
                }
                if ((batteryManager->currentVoltage + currentCurrent / batteryManager->capacity * 0.0002777) < batteryManager->maxVoltage)
                {
                    batteryManager->currentVoltage += currentCurrent / batteryManager->capacity * 0.0002777;
                    sec = time(NULL);
                    returnCode = BATTERY_CHARGING;
                }
                else
                {
                    disconnectCharger(batteryManager);
                    printf("Battery is full\n");
                    returnCode = BATTERY_FULL;
                    goto Exit;
                }
            }
        }
    }

Exit:
    return returnCode;
}

uint8_t connectLoad(BatteryManager *batteryManager, double voltage, double current)
{
    uint8_t returnCode = 0;
    static time_t sec = 0;
    if (voltage < (batteryManager->nominalVoltage - 0.1) || voltage > (batteryManager->nominalVoltage + 0.1))
    {
        printf("Incorrect load voltage\n");
        returnCode = INCORRECT_VOLTAGE;
        goto Exit;
    }
    else if (current > batteryManager->maxOutputCurrent)
    {
        printf("Incorrect load current\n");
        returnCode = INCORRECT_CURRENT;
        goto Exit;
    }
    else if (batteryManager->currentVoltage <= batteryManager->minVoltage)
    {

        printf("Battery is low\n");
        returnCode = BATTERY_LOW;
        goto Exit;
    }
    else
    {
        printf("\rLoad connected\n");
        batteryManager->isLoadConnected = 1;
    }

Exit:
    return returnCode;
}
void disconnectLoad(BatteryManager *batteryManager)
{
    printf("\rLoad disconnected\n");
    batteryManager->isLoadConnected = 0;
}

uint8_t unchargeBattery(BatteryManager *batteryManager, double voltage, double current)
{
    uint8_t returnCode = 0;
    static time_t sec = 0;
    if (voltage < (batteryManager->nominalVoltage - 0.1) || voltage > (batteryManager->nominalVoltage + 0.1))
    {
        returnCode = INCORRECT_VOLTAGE;
        goto Exit;
    }
    else if (current > batteryManager->maxOutputCurrent)
    {
        returnCode = INCORRECT_CURRENT;
        goto Exit;
    }
    else
    {
        if (time(NULL) > sec)
        {
            if ((batteryManager->currentVoltage - current / batteryManager->capacity * 0.0002777) > batteryManager->minVoltage)
            {
                batteryManager->currentVoltage -= current / batteryManager->capacity * 0.0002777;
                sec = time(NULL);
                returnCode = BATTERY_UNCHARGING;
            }
            else
            {
                disconnectLoad(batteryManager);
                printf("Battery is low\n");
                returnCode = BATTERY_LOW;
                goto Exit;
            }
        }
    }

Exit:
    return returnCode;
}