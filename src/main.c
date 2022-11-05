#include <stdio.h>
#include "BatteryManager.h"

int main()
{
    BatteryManager b_manager = {3.6, 1.5, 2.5, 4.2, 4, 15, 4.2, 2, 0};

    connectCharger(&b_manager);
    while (1) {
        chargeBattery(&b_manager, 4.2, 2);
        printf("\r%.2f%%", getStateOfCharge(&b_manager));
    }
}