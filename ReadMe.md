# Проект "Менеджер батареї"
## Зміст
1. [Опис](#Опис)
2. [Структура менеджера батареї](#Структура-менеджера-батареї)
3. [Функції менеджера батареї](#Функції-менеджера-батареї)
4. [Результати роботи](#Результати-роботи)
____
## Опис
Менеджер батареї дозволяє контролювати стан роботи батареї, виконувати безпечну зарядку та розряджання. Є захист від перерозряду батареї.

Робота менеджера відбувається у безкінечному циклі. За допомогою консолі імітуються різні дії з батареєю, наприклад, підключення зарядного пристрою. Також через консоль можна отримувати інформацію про стан батареї та різні її параметри.
____
## Структура менеджера батареї
Структура менеджера батареї складається з таких полів:
```C
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
```
*capacity* - номінальна ємність батареї.

*nominalVoltage* - номінальна напруга батареї.

*minVoltage* - мінімальна допустима напруга батареї.

*maxVoltage* - максимально допустима напруга батареї.

*currentVoltage* - поточна напруга батареї.

*maxOutputCurrent* - максимальний вихідний струм батареї.

*chargingVoltage* - номінальна напруга зарядки.

*maxChargingCurrent* - максимальний струм заряджання.

*isChargerConnected* - прапор підключення зарядного пристрою.

*isLoadConnected* - прапор підключення навантаження.
___
## Функції менеджера батареї
```C
double getBatteryVoltage(BatteryManager* batteryManager);
double getBatteryCapacity(BatteryManager* batteryManager);
double getStateOfCharge(BatteryManager* batteryManager);

uint8_t connectCharger(BatteryManager* batteryManager, double voltage, double current);
void disconnectCharger(BatteryManager* batteryManager);
uint8_t chargeBattery(BatteryManager* batteryManager, double voltage, double current);

uint8_t connectLoad(BatteryManager* batteryManager, double voltage, double current);
void disconnectLoad(BatteryManager* batteryManager);
uint8_t unchargeBattery(BatteryManager* batteryManager, double voltage, double current);
```
*getBatteryVoltage()* - отримання поточної напруги батареї.

*getBatteryCapacity()* - отримання номінальної ємності батареї.

*getStateOfCharge()* - отримання рівня заряду батареї у відсотках.

*connectCharger()* - підключення зарядного пристрою.
 - При підключенні зарядного пристрою перевіряється напруга та максимальний струм заряджання. Якщо параметри пристрою не відповідають допустимим, зарядний пристрій не підключиться.

*disconnectCharger()* - вимкнення зарядного пристрою.

*chargeBattery()* - заряджання батареї.
 - Під час заряджання перевіряється напруга та струм заряджання. Якщо параметри не відповідають допустимим, заряджання припиниться і зарядний пристрій вимкнеться. При низькому заряді батареї вона заряджається невеликим струмом. Коли батарея зарядиться повністю, зарядний пристрій вимкнеться.

*connectLoad()* - підключення навантаження до батареї.
 - При підключенні навантаження перевіряється напруга та максимальне споживання. Якщо параметри навантаження не відповідають допустимим, то навантаження не підключиться.

*disconnectLoad()* - відключення навантаження від батареї.

*unchargeBattery()* - розрядка батареї.
 - Під час розряду батареї перевіряється напруга та струм розряду. Якщо параметри не відповідають допустимим, навантаження відключиться. При низькому заряді батареї навантаження автоматично вимкнеться.
___
## Результати роботи
### Підключення зарядного пристрою
<img src="./raw/conn_ch.png"/>

### Підключення навантаження
<img src="./raw/conn_load.png"/>

### Некоректна напруга заряджання
<img src="./raw/inc_ch_volt.png"/>

### Виведення всієї інформації про батарею
<img src="./raw/all_info.png"/>

____
