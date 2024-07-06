#include <Protocol.h>
#include <ButtonManager.h>
#include <TaskManager.h>
#include <Array.h>
#include <Pump.h>

const int DEFAULT_AUTO_PUMP_DURATION_SECONDS = 600;

const int TRANSMISSION_RATE = 500;
const int BUTTON_BUFFER_SIZE = 10;
const int AUTO_PUMP_BUFFER_SIZE = 21;
const int ARRAY_SIZE = BUTTON_BUFFER_SIZE + AUTO_PUMP_BUFFER_SIZE + 2; // Include start and stop characters
const int RETRANSMISSION_TIMEOUT = 100; // Timeout after 1 second

const char START_CHAR = '>';
const char STOP_CHAR = '<';
const char ACK = 'A';
const char NAK = 'N';

static char buttonBuffer[BUTTON_BUFFER_SIZE];
static char autoPumpBuffer[AUTO_PUMP_BUFFER_SIZE];
static bool shouldToSendData(true);

void trySendData();
void tryReceiveData();
void resetState();

static void loopCommunicationProtocolTask()
{
    Protocol::getInstance().loopCommunicationProtocol();
}

Protocol& Protocol::getInstance() {
    static Protocol instance;
    return instance;
}

Protocol::Protocol() {
    resetState();
    TaskManager::getInstance().createTask(TRANSMISSION_RATE, loopCommunicationProtocolTask);
}

void Protocol::loopCommunicationProtocol()
{
    if (shouldToSendData)
    {
        trySendData();
    } else {
        tryReceiveData();
    }
}

char* getButtonStatuses()
{
    Array<Button*, BUTTON_COUNT_MAX> buttons = ButtonManager::getInstance().getAllButtons();
    for (int i = 0; i < BUTTON_COUNT_MAX - 1; i++)
    {
        buttonBuffer[i] = buttons.at(i)->isToggled() ? 'H' : 'L';
    }
    return buttonBuffer;
}

void trySendData()
{
    Serial.write(START_CHAR);
    Serial.write(getButtonStatuses(), BUTTON_BUFFER_SIZE - 1);

    uint32_t autoPumpStartTime = Pump::getInstance().getAutoPumpStartTime();
    uint32_t autoPumpEndTime = Pump::getInstance().getAutoPumpEndTime();
    
    sprintf(autoPumpBuffer, "%010lu%010lu", autoPumpStartTime, autoPumpEndTime);
    Serial.write(autoPumpBuffer, AUTO_PUMP_BUFFER_SIZE - 1);

    Serial.write(STOP_CHAR);

    // Wait for ACK or NAK
    unsigned long startTime = millis();
    while (!Serial.available())
    {
        if (millis() - startTime > RETRANSMISSION_TIMEOUT)
        {
            resetState();
            return;
        }
    }

    char response = Serial.read();
    if (response == NAK) // NAK received, retransmit
    {
        resetState();
    }
}

void tryReceiveData() {
    char incomingData[ARRAY_SIZE];
    int index = 0;
    bool startCharFound = false;
    while(Serial.available() && index < ARRAY_SIZE) {
        char receivedChar = Serial.read();
        if(receivedChar == START_CHAR) {
            startCharFound = true;
        } else if(receivedChar == STOP_CHAR && startCharFound) {
            // Complete message received, send ACK
            Serial.write(ACK);
            Serial.write(incomingData, BUTTON_BUFFER_SIZE);
            return;
        } else if(startCharFound) {
            // Add received character to buffer
            incomingData[index++] = receivedChar;
        }
    }
    // Incomplete or corrupted message received, send NAK
    Serial.write(NAK);

    resetState();
}

void resetState() {
    shouldToSendData = true;
    memset(buttonBuffer, 0, sizeof(buttonBuffer));
    memset(autoPumpBuffer, 0, sizeof(autoPumpBuffer));
}

int Protocol::getAutoPumpDurationSeconds()
{
    return DEFAULT_AUTO_PUMP_DURATION_SECONDS;
}