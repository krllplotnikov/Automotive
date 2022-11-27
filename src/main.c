#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "BatteryManager.h"

BatteryManager b_manager = {2,   // capacity;
                            3.7, // nominalVoltage;
                            3,   // minVoltage;
                            4.2, // maxVoltage;
                            3.99, // currentVoltage;
                            15,  // maxOutputCurrent;
                            4.2, // chargingVoltage;
                            2,   // maxChargingCurrent;
                            0,   // isChargerConnected;
                            0};  // isLoadConnected;

//BatteryManager battery = { 2.f,0,0,0,0,15,0,0,0};

Power current = { 5,3,2,2 };

char ch[10];
char parsedCh[10][10];
void parse();



void *consoleTask()
{
    while (1)
    {
        scanf("%s", ch);
        parse();
    }
}

/*--command list--
c,(volt),(cur) - connect charger
d - disconnect charger
% - state of charge
v - battery voltage
l,(volt),(cur) - connect load
u - disconnect load
a - battey capacity A/h
p - print all information
*/

void *b_managerTask()
{
    float I = b_manager.maxOutputCurrent;

    double chargingVoltage, chargingCurrent;
    double loadVoltage, loadCurrent;
    float level = getStateOfCharge(&b_manager);
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
        case 'l':
        {
            loadVoltage = atof(parsedCh[1]);
            loadCurrent = atof(parsedCh[2]);
            connectLoad(&b_manager, loadVoltage, loadCurrent);
            parsedCh[0][0] = 0;
            break;
        }
        case 'u':
        {
            disconnectLoad(&b_manager);
            parsedCh[0][0] = 0;
            break;
        }
        case 'a':
        {
            printf("Battery capacity = %.3fA/h\n", getBatteryCapacity(&b_manager));
            parsedCh[0][0] = 0;
            break;
        }
        case 'p':
        {
            printf("----------\n");
            printf("Battery nominal voltage = %.2fV\n", b_manager.nominalVoltage);
            printf("Battery max voltage = %.2fV\n", b_manager.maxVoltage);
            printf("Battery min voltage = %.2fV\n", b_manager.minVoltage);
            printf("Battery max output current = %.2fA\n", b_manager.maxOutputCurrent);
            printf("Battery max charging current = %.2fA\n", b_manager.maxChargingCurrent);
            printf("Battery SoC = %.2f%%\n", getStateOfCharge(&b_manager));
            printf("Battery voltage = %.2fV\n", getBatteryVoltage(&b_manager));
            printf("Battery capacity = %.3fA/h\n", getBatteryCapacity(&b_manager));
            printf("Battery charger connected = %d\n", b_manager.isChargerConnected);
            printf("Battery load connected = %d\n", b_manager.isLoadConnected);
            printf("----------\n");
            parsedCh[0][0] = 0;
            break;
        }
        case 'e':
        {
            I = type_econom(&b_manager, &current, level);
            printf("I = %f A\n", I);
            parsedCh[0][0] = 0;
            break;
        }
        case 't':
        {

            time_work(&b_manager, I, level);
            parsedCh[0][0] = 0;
            break;
        }

        }

        if (b_manager.isChargerConnected)
        {
            chargeBattery(&b_manager, chargingVoltage, chargingCurrent);
        }
        if (b_manager.isLoadConnected)
        {
            unchargeBattery(&b_manager, loadVoltage, loadCurrent);
        }
    }
}

int main()
{
    pthread_t consoleThread, b_managerThread;
    pthread_create(&consoleThread, NULL, &consoleTask, NULL);
    pthread_create(&b_managerThread, NULL, &b_managerTask, NULL);
    pthread_join(consoleThread, NULL);
    pthread_join(b_managerThread, NULL);
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