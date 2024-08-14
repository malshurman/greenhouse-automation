#include <ESP8266WiFi.h>
#include <States.h>
#include <WebSocketHandler.h>

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
const int DEFAULT_PUMP_TIME_BUFFER_SIZE = 5;
const int WIFI_SIGNAL_STRENGTH_BUFFER_SIZE = 5;
const int WEBSOCKET_CONNECTION_BUFFER_SIZE = 2;

const int ARRAY_SIZE = BUTTON_BUFFER_SIZE + AUTO_PUMP_BUFFER_SIZE + CURRENT_TIME_BUFFER_SIZE + DHT_BUFFER_SIZE + WIFI_SIGNAL_STRENGTH_BUFFER_SIZE + WEBSOCKET_CONNECTION_BUFFER_SIZE + 2; // Include start and stop characters
const int RETRANSMISSION_TIMEOUT = 1000;                                                                                                                                                  // Timeout after 1 second

const int MAX_ITERATION_COUNT = 10000;

static char incomingData[ARRAY_SIZE];
static char formattedData[ARRAY_SIZE];
static char autoPumpTimeBuffer[DEFAULT_PUMP_TIME_BUFFER_SIZE];
static char wifiSignalStrengthBuffer[WIFI_SIGNAL_STRENGTH_BUFFER_SIZE];
static char websocketConnectionBuffer[WEBSOCKET_CONNECTION_BUFFER_SIZE];

void State::setContext(Context *context)
{
    this->context = context;
}

Context::Context() : currentState(new ReceiveDataState())
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

void SendDataState::handle()
{
    DEBUG_PRINTLN("Sending data...");
    Serial.write(START_CHAR);

    long wifiSignalStrength = WiFi.RSSI();
    sprintf(wifiSignalStrengthBuffer, "%04ld", wifiSignalStrength);
    Serial.write(wifiSignalStrengthBuffer, WIFI_SIGNAL_STRENGTH_BUFFER_SIZE - 1);

    
    bool websocketStatus = WebSocketHandler::getInstance().isConnected();
    sprintf(websocketConnectionBuffer, "%d", websocketStatus ? 1 : 0);
    Serial.write(websocketConnectionBuffer, WEBSOCKET_CONNECTION_BUFFER_SIZE - 1);

    int autoPumpTimeMinutes = WebSocketHandler::getInstance().getAutoPumpTimeMinutes();
    sprintf(autoPumpTimeBuffer, "%04d", autoPumpTimeMinutes);
    Serial.write(autoPumpTimeBuffer, DEFAULT_PUMP_TIME_BUFFER_SIZE - 1);

    char command = WebSocketHandler::getInstance().getNextCommand();
    Serial.write(&command, 1);
    
    Serial.write(STOP_CHAR);
    context->transitionTo(new ReceiveDataState());
}

void ReceiveDataState::handle()
{
    DEBUG_PRINTLN("Receiving data...");

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
            context->transitionTo(new AcknowledgeState());

            long wifiSignalStrength = WiFi.RSSI();
            bool websocketStatus = WebSocketHandler::getInstance().isConnected();

            char formattedData[ARRAY_SIZE];
            snprintf(formattedData, ARRAY_SIZE, "%.*s%ld%d<", index - 6, incomingData, wifiSignalStrength, websocketStatus ? 1 : 0);

            WebSocketHandler::getInstance().sendMessageToWebSocket(formattedData);
            return;
        }
        else if (receivedChar == ACK)
        {
            context->transitionTo(new ReceiveDataState());
            return;
        }
        else if (receivedChar == NAK)
        {
            context->transitionTo(new SendDataState());
            return;
        }
        else if (receivedChar == REQUEST_DATA_CHAR)
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
}

void AcknowledgeState::handle()
{
    DEBUG_PRINTLN("Acknowledging data...");
    Serial.write(ACK);
    context->transitionTo(new ReceiveDataState());
}

void NegativeAcknowledgeState::handle()
{
    DEBUG_PRINTLN("Negative acknowledging data...");
    Serial.write(NAK);
    context->transitionTo(new ReceiveDataState());
}