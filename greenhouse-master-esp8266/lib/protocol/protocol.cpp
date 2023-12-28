#include <protocol.h>
#include <CRC8.h>

CRC8 crc;

bool isReadyToSendData(true);

void trySendData();
void tryReceiveData();

void loopCommunicationProtocol()
{
    trySendData();
    tryReceiveData();
}

void trySendData()
{
    if (!isReadyToSendData) return;
    
    char helloWorld[] = "Hello world";
    uint8_t array[40] = { 0 };
    int i;

    array[0] = '>';
    for (i = 1; i < 12; i++)
    {
        array[i] = helloWorld[i-1];
        crc.add(array[i]);
    }

    array[i] = crc.calc();
    array[i + 1] = '\n';

    for (i = 0; i < 14; i++)
    {
        // Serial.write(array[i]);
        Serial.write(array[i]);
    }

    crc.reset();
    isReadyToSendData = false;
}

void tryReceiveData() {
    while (!Serial.available());
    
    while (Serial.available())
    {
        Serial.read();
    }

    isReadyToSendData = true;
}