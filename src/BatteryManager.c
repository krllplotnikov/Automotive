#include "BatteryManager.h"
#include <time.h>
#include <stdio.h>

#define DEBUG 1

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
#if DEBUG
        printf("Incorrect charging voltage\n");
#endif
        returnCode = INCORRECT_VOLTAGE;
        goto Exit;
    }
    else if (current > batteryManager->maxChargingCurrent)
    {
#if DEBUG
        printf("Incorrect charging current\n");
#endif
        returnCode = INCORRECT_CURRENT;
        goto Exit;
    }
    else
    {
#if DEBUG
        printf("Charger connected\n");
#endif
        batteryManager->isChargerConnected = 1;
        returnCode = CHARGER_CONNECTED;
        goto Exit;
    }

Exit:
    return returnCode;
}

uint8_t disconnectCharger(BatteryManager *batteryManager)
{
#if DEBUG
    printf("Charger disconnected\n");
#endif
    batteryManager->isChargerConnected = 0;
    return CHARGER_DISCONNECTED;
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
#if DEBUG
                    printf("Battery is full\n");
#endif
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
#if DEBUG
        printf("Incorrect load voltage\n");
#endif
        returnCode = INCORRECT_VOLTAGE;
        goto Exit;
    }
    else if (current > batteryManager->maxOutputCurrent)
    {
#if DEBUG
        printf("Incorrect load current\n");
#endif
        returnCode = INCORRECT_CURRENT;
        goto Exit;
    }
    else
    {
#if DEBUG
        printf("Load connected\n");
#endif
        batteryManager->isLoadConnected = 1;
        returnCode = LOAD_CONNECTED;
        goto Exit;
    }

Exit:
    return returnCode;
}
uint8_t disconnectLoad(BatteryManager *batteryManager)
{
#if DEBUG
    printf("Load disconnected\n");
#endif
    batteryManager->isLoadConnected = 0;
    return LOAD_DISCONNECTED;
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
#if DEBUG
                printf("Battery is low\n");
#endif
                returnCode = BATTERY_LOW;
                goto Exit;
            }
        }
    }

Exit:
    return returnCode;
}