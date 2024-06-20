#include <Protocol.h>
#include <Ticker.h>

const int BUTTON_BUFFER_SIZE = 9;
const int ARRAY_SIZE = BUTTON_BUFFER_SIZE + 2; // Include start and stop characters
const int RETRANSMISSION_TIMEOUT = 100; // Timeout after 1 second

const char START_CHAR = '>';
const char STOP_CHAR = '<';
const char ACK = 'A';
const char NAK = 'N';

char buffer[] = "HHHHHHHHH";
bool shouldToSendData(true);

void loopCommunicationProtocol();
void trySendData();
void tryReceiveData();
void resetState();

Ticker communicationTicker(loopCommunicationProtocol, 500);

void initializeCommunicationProtocol()
{
    communicationTicker.start();
}

void updateCommunication()
{
    communicationTicker.update();
}

void loopCommunicationProtocol()
{
    if (shouldToSendData)
    {
        trySendData();
    } else {
        tryReceiveData();
    }
}

void trySendData()
{
    Serial.write(START_CHAR);
    Serial.write(buffer, BUTTON_BUFFER_SIZE);
    Serial.write(STOP_CHAR);

    // Wait for ACK or NAK
    unsigned long startTime = millis();
    while (!Serial.available())
    {
        if (millis() - startTime > RETRANSMISSION_TIMEOUT)
        {
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
    memset(buffer, 0, sizeof(buffer));
}