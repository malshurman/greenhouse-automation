#include <DataHandler.h>
#include <Pump.h>
#include <Valve.h>
#include <DHTModule.h>
#include <Curtains.h>
#include <LCDModule.h>
#include <ManualMode.h>

#define DEFAULT_AUTO_PUMP_DURATION_MINUTES 10

static int autoPumpTimeMinutes = DEFAULT_AUTO_PUMP_DURATION_MINUTES;

static long wifiSignalStrength = -100;
static bool socketConnected = false;

enum Action {
    LEFT_CURTAINS_UP = 'L',
    LEFT_CURTAINS_DOWN = 'l',
    RIGHT_CURTAINS_UP = 'R',
    RIGHT_CURTAINS_DOWN = 'r',
    PUMP_ON = 'P',
    PUMP_OFF = 'p',
    AUTO_PUMP_TOGGLE = 'A',
    VALVE_TOGGLE = 'V',
    RESET_ON = 'M',
    RESET_OFF = 'm',
    NO_OPERATION = 'X'
};

void handleIncomingData(char *data)
{
    int newAutoPumpTime;
    long newWifiSignalStrength;
    int socketStatus;
    char action;

    if (sscanf(data, ">%4ld%1d%4d%c<", &newWifiSignalStrength, &socketStatus, &newAutoPumpTime, &action) == 4)
    {
        autoPumpTimeMinutes = newAutoPumpTime;
        wifiSignalStrength = newWifiSignalStrength;
        socketConnected = (socketStatus == 1);

        switch (action)
        {
            case LEFT_CURTAINS_UP:
                Curtains::getInstance().leftCurtainUp();
                break;
            case LEFT_CURTAINS_DOWN:
                Curtains::getInstance().leftCurtainDown();
                break;
            case RIGHT_CURTAINS_UP:
                Curtains::getInstance().rightCurtainUp();
                break;
            case RIGHT_CURTAINS_DOWN:
                Curtains::getInstance().rightCurtainDown();
                break;
            case PUMP_ON:
                Pump::getInstance().pumpOn();
                break;
            case PUMP_OFF:
                Pump::getInstance().pumpOff();
                break;
            case AUTO_PUMP_TOGGLE:
                Pump::getInstance().autoPumpToggle();
                break;
                break;
            case VALVE_TOGGLE:
                Valve::getInstance().toggle();
                break;
            case RESET_ON:
                ManualMode::getInstance().turnOnAuto();
                break;
            case RESET_OFF:
                ManualMode::getInstance().turnOffAuto();
                break;
            default:
                break;
        }
    }
}

long getWifiSignalStrength()
{
    return wifiSignalStrength;
}

bool isSocketConnected()
{
    return socketConnected;
}

uint32_t getAutoPumpTimeMinutes()
{
    return autoPumpTimeMinutes;
}