#include <Protocol.h>
#include <TaskManager.h>
#include <LCDModule.h>
#include <DataHandler.h>

#define TRANSMISSION_RATE 150
#define LCD_UPDATE_RATE 1000
#define DEFAULT_AUTO_PUMP_DURATION_SECONDS 600

static char lcdBuffer[21];

static void loopCommunicationProtocolTask()
{
    Protocol::getInstance().loopCommunicationProtocol();
}

static void writeConnectionStatusToLCDTask()
{
    Protocol::getInstance().writeConnectionStatusToLCD();
}

Protocol& Protocol::getInstance() {
    static Protocol instance;
    return instance;
}

Protocol::Protocol() : context(new Context()) {
    TaskManager::getInstance().createTask(TRANSMISSION_RATE, loopCommunicationProtocolTask);
    TaskManager::getInstance().createTask(LCD_UPDATE_RATE, writeConnectionStatusToLCDTask);
}

Protocol::~Protocol() {
    delete context;
}

void Protocol::loopCommunicationProtocol()
{
    context->execute();
}

int Protocol::getAutoPumpDurationMinutes()
{
    return getAutoPumpTimeMinutes();
}

void Protocol::writeConnectionStatusToLCD()
{
    long wifiSignalStrength = getWifiSignalStrength();
    bool socketConnected = isSocketConnected();

    sprintf(lcdBuffer, "WiFi: %3ld%% Soc: %s", 100 - abs(wifiSignalStrength), socketConnected ? " ON" : "OFF");

    LCDModule::getInstance().writeToLCD(0, 3, lcdBuffer);
}