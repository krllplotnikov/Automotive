#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "BatteryManager.h"

BatteryManager b_manager = {2,   // maxCapacity;
                            2.5, // minBatteryVoltage;
                            4.2, // maxBatteryVoltage;
                            4,   // currentBatteryVoltage;
                            15,  // maxBatteryCurrent;
                            4.2, // chargingVoltage;
                            2,   // maxChargingCurrent;
                            0};  // isChargerConnected;

void parse();
char ch[10];
char parsedCh[10][10];

void *consoleTask()
{
    while (1)
    {
        scanf("%s", ch);
        parse(ch, parsedCh);
    }
}

void *b_managerTask()
{
    double chargingVoltage, chargingCurrent;
    while (1)
    {
        switch (parsedCh[0][0])
        {
        case '%':
        {
            printf("Battery SoC = %.2f%%\n", getStateOfCharge(&b_manager));
            parsedCh[0][0] = 0;
            break;
        }
        case 'c':
        {
            chargingVoltage = atof(parsedCh[1]);
            chargingCurrent = atof(parsedCh[2]);
            connectCharger(&b_manager, chargingVoltage, chargingCurrent);
            parsedCh[0][0] = 0;
            break;
        }
        case 'd':
        {
            disconnectCharger(&b_manager);
            parsedCh[0][0] = 0;
            break;
        }
        case 'v':
        {
            printf("Battery voltage = %.2fV\n", getBatteryVoltage(&b_manager));
            parsedCh[0][0] = 0;
            break;
        }
        }

        if (b_manager.isChargerConnected)
        {
            chargeBattery(&b_manager, chargingVoltage, chargingCurrent);
        }
    }
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &consoleTask, NULL);
    pthread_create(&t2, NULL, &b_managerTask, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}



void parse()
{
    int x = 0;
    int y = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            parsedCh[i][j] = 0;
        }
    }
    for (int i = 0; ch[i] != 0; i++)
    {
        if (ch[i] != ',')
        {
            parsedCh[x][y] = ch[i];
            y++;
        }
        else
        {
            x++;
            y = 0;
        }
    }
}