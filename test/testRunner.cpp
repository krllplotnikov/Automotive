#include <gtest/gtest.h>
#include <iostream>

extern "C"
{
#include "BatteryManager.h"
}

BatteryManager test_manager = {2,   // capacity;
                               3.7, // nominalVoltage;
                               3,   // minVoltage;
                               4.2, // maxVoltage;
                               3.7, // currentVoltage;
                               15,  // maxOutputCurrent;
                               4.2, // chargingVoltage;
                               2,   // maxChargingCurrent;
                               0,   // isChargerConnected;
                               0};  // isLoadConnected;

// ConnectCharger
TEST(TestConnectCharger, BasicTest)
{
    double voltage = 4.2;
    double current = 2;

    uint8_t connectStatus = connectCharger(&test_manager, voltage, current);

    ASSERT_EQ(CHARGER_CONNECTED, connectStatus);
}

TEST(TestConnectCharger, IncorrectCurrent)
{
    double voltage = 4.2;
    double current = 5;

    uint8_t connectStatus = connectCharger(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_CURRENT, connectStatus);
}

TEST(TestConnectCharger, IncorrectVoltage)
{
    double voltage = 5;
    double current = 2;

    uint8_t connectStatus = connectCharger(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_VOLTAGE, connectStatus);
}

// Charge
TEST(TestChargeBattery, BasicTest)
{
    double voltage = 4.2;
    double current = 2;

    uint8_t chargingStatus = chargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(BATTERY_CHARGING, chargingStatus);
}

TEST(TestChargeBattery, BatteryFull)
{
    double voltage = 4.2;
    double current = 2;
    test_manager.currentVoltage = 4.2;

    uint8_t chargingStatus; 
    while(chargingStatus != BATTERY_FULL)
        chargingStatus = chargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(BATTERY_FULL, chargingStatus);
}

TEST(TestChargeBattery, IncorrectCurrent)
{
    double voltage = 4.2;
    double current = 2;

    connectCharger(&test_manager, voltage, current);
    current = 5;
    uint8_t chargingStatus = chargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_CURRENT, chargingStatus);
}

TEST(TestChargeBattery, IncorrectVoltage)
{
    double voltage = 4.2;
    double current = 2;

    connectCharger(&test_manager, voltage, current);

    voltage = 3;
    uint8_t chargingStatus = chargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_VOLTAGE, chargingStatus);
}

// ConnectLoad
TEST(TestConnectLoad, BasicTest)
{
    double voltage = 3.7;
    double current = 5;

    uint8_t connectStatus = connectLoad(&test_manager, voltage, current);

    ASSERT_EQ(LOAD_CONNECTED, connectStatus);
}

TEST(TestConnectLoad, IncorrectCurrent)
{
    double voltage = 3.7;
    double current = 20;

    uint8_t connectStatus = connectLoad(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_CURRENT, connectStatus);
}

TEST(TestConnectLoad, IncorrectVoltage)
{
    double voltage = 5;
    double current = 5;

    uint8_t connectStatus = connectLoad(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_VOLTAGE, connectStatus);
}

// Uncharge
TEST(TestUnhargeBattery, BasicTest)
{
    double voltage = 3.7;
    double current = 5;

    connectLoad(&test_manager, voltage, current);
    uint8_t unchargingStatus = unchargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(BATTERY_UNCHARGING, unchargingStatus);
}

TEST(TestUnhargeBattery, BatteryLow)
{
    double voltage = 3.7;
    double current = 2;
    test_manager.currentVoltage = 2.8;

    connectLoad(&test_manager, voltage, current);

    uint8_t unchargingStatus; 
    while(unchargingStatus != BATTERY_LOW)
        unchargingStatus = unchargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(BATTERY_LOW, unchargingStatus);
}

TEST(TestUnhargeBattery, IncorrectCurrent)
{
    double voltage = 3.7;
    double current = 5;

    connectLoad(&test_manager, voltage, current);
    current = 20;
    uint8_t unchargingStatus = unchargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_CURRENT, unchargingStatus);
}

TEST(TestUnhargeBattery, IncorrectVoltage)
{
    double voltage = 3.7;
    double current = 5;

    connectLoad(&test_manager, voltage, current);

    voltage = 3;
    uint8_t unchargingStatus = unchargeBattery(&test_manager, voltage, current);

    ASSERT_EQ(INCORRECT_VOLTAGE, unchargingStatus);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}