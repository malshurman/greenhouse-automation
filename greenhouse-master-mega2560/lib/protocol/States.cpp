#include <Arduino.h>
#include <States.h>
#include <ButtonManager.h>
#include <Array.h>
#include <Pump.h>
#include <DHTModule.h>
#include <DataHandler.h>

#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#endif

#ifdef DEBUG_PRINT
#undef DEBUG_PRINT
#endif

#define DEBUG_SERIAL Serial
#define DEBUG_ENABLED false

#define DEBUG_PRINT(...) \
    if (DEBUG_ENABLED)   \
    DEBUG_SERIAL.printf(__VA_ARGS__)
#define DEBUG_PRINTLN(...) \
    if (DEBUG_ENABLED)     \
    DEBUG_SERIAL.println(__VA_ARGS__)

#define REQUEST_DATA_CHAR '?'
#define START_CHAR '>'
#define STOP_CHAR '<'
#define ACK 'A'
#define NAK 'N'

const int BUTTON_BUFFER_SIZE = 10;
const int AUTO_PUMP_BUFFER_SIZE = 21;
const int CURRENT_TIME_BUFFER_SIZE = 11;
const int DHT_BUFFER_SIZE = 11;
const int WIFI_SIGNAL_STRENGTH_BUFFER_SIZE = 5;
const int WEBSOCKET_CONNECTION_BUFFER_SIZE = 2;

const int ARRAY_SIZE = BUTTON_BUFFER_SIZE + AUTO_PUMP_BUFFER_SIZE + CURRENT_TIME_BUFFER_SIZE + DHT_BUFFER_SIZE + WIFI_SIGNAL_STRENGTH_BUFFER_SIZE + WEBSOCKET_CONNECTION_BUFFER_SIZE + 2; // Include start and stop characters
const int RETRANSMISSION_TIMEOUT = 1000;                                                                                                                                                  // Timeout after 1 second

const int MAX_ITERATION_COUNT = 10000;

static char incomingData[ARRAY_SIZE];
static char buttonBuffer[BUTTON_BUFFER_SIZE];
static char autoPumpBuffer[AUTO_PUMP_BUFFER_SIZE];
static char currentTimeBuffer[CURRENT_TIME_BUFFER_SIZE];
static char dhtBuffer[DHT_BUFFER_SIZE];
static char wifiSignalStrengthBuffer[WIFI_SIGNAL_STRENGTH_BUFFER_SIZE];
static char websocketConnectionBuffer[WEBSOCKET_CONNECTION_BUFFER_SIZE];

char *getButtonStatuses();

void State::setContext(Context *context)
{
    this->context = context;
}

Context::Context() : currentState(new RequestDataState())
{
    currentState->setContext(this);
    lastStateChangeTimeMillis = millis();
}

void Context::transitionTo(State *state)
{
    if (currentState != nullptr)
    {
        delete currentState;
    }
    currentState = state;
    currentState->setContext(this);
    lastStateChangeTimeMillis = millis();
}

void Context::execute()
{
    currentState->handle();
}

unsigned long Context::getLastStateChangeTimeMillis()
{
    return lastStateChangeTimeMillis;
}

void RequestDataState::handle()
{
    DEBUG_PRINTLN("Requesting data...");
    Serial.write(REQUEST_DATA_CHAR);
    context->transitionTo(new ReceiveDataState());
}

void SendDataState::handle()
{
    DEBUG_PRINTLN("Sending data...");

    Serial.write(START_CHAR);
    Serial.write(getButtonStatuses(), BUTTON_BUFFER_SIZE - 1);

    uint32_t autoPumpStartTime = Pump::getInstance().getAutoPumpStartTime();
    uint32_t autoPumpEndTime = Pump::getInstance().getAutoPumpEndTime();

    sprintf(autoPumpBuffer, "%010lu%010lu", autoPumpStartTime, autoPumpEndTime);
    Serial.write(autoPumpBuffer, AUTO_PUMP_BUFFER_SIZE - 1);

    uint32_t currentTime = millis();
    sprintf(currentTimeBuffer, "%010lu", currentTime);
    Serial.write(currentTimeBuffer, CURRENT_TIME_BUFFER_SIZE - 1);

    float temperature = DHTModule::getInstance().getTemperature();
    float humidity = DHTModule::getInstance().getHumidity();

    int tempInt = (int)(temperature * 10);
    int humInt = (int)(humidity * 10);

    sprintf(dhtBuffer, "%02d.%02d%02d.%02d", tempInt / 10, abs(tempInt % 10), humInt / 10, abs(humInt % 10));
    Serial.write(dhtBuffer, DHT_BUFFER_SIZE - 1);

    sprintf(wifiSignalStrengthBuffer, "-100");
    Serial.write(wifiSignalStrengthBuffer, WIFI_SIGNAL_STRENGTH_BUFFER_SIZE - 1);

    sprintf(websocketConnectionBuffer, "1");
    Serial.write(websocketConnectionBuffer, WEBSOCKET_CONNECTION_BUFFER_SIZE - 1);

    Serial.write(STOP_CHAR);

    context->transitionTo(new ReceiveDataState());
}

void ReceiveDataState::handle()
{
    DEBUG_PRINTLN("Receiving data...");
    if (millis() - context->getLastStateChangeTimeMillis() > RETRANSMISSION_TIMEOUT)
    {
        context->transitionTo(new RequestDataState());
        return;
    }

    int index = 0;
    bool receiving = false;

    while (Serial.available() && index < ARRAY_SIZE)
    {
        char receivedChar = Serial.read();
        incomingData[index++] = receivedChar;

        if (receivedChar == START_CHAR)
        {
            receiving = true;
        }
        else if (receivedChar == STOP_CHAR)
        {
            DEBUG_PRINTLN(incomingData);
            handleIncomingData(incomingData);
            context->transitionTo(new AcknowledgeState());
            return;
        }
        else if (receivedChar == ACK)
        {
            context->transitionTo(new RequestDataState());
            return;
        }
        else if (receivedChar == NAK)
        {
            context->transitionTo(new SendDataState());
            return;
        }
        else if (!receiving)
        {
            int cleanupIndex = 0;
            while (Serial.available() && cleanupIndex < MAX_ITERATION_COUNT)
            {
                Serial.read();
                cleanupIndex++;
            }
            context->transitionTo(new NegativeAcknowledgeState());
            return;
        }
    }

    context->transitionTo(new NegativeAcknowledgeState());
}

void AcknowledgeState::handle()
{
    DEBUG_PRINTLN("Acknowledging data...");
    Serial.write(ACK);
    context->transitionTo(new SendDataState());
}

void NegativeAcknowledgeState::handle()
{
    DEBUG_PRINTLN("Negative acknowledging data...");
    Serial.write(NAK);
    context->transitionTo(new ReceiveDataState());
}

char *getButtonStatuses()
{
    Array<Button *, BUTTON_COUNT_MAX> buttons = ButtonManager::getInstance().getAllButtons();
    for (int i = 0; i < BUTTON_COUNT_MAX - 1; i++)
    {
        buttonBuffer[i] = buttons.at(i)->isToggled() ? 'H' : 'L';
    }
    return buttonBuffer;
}