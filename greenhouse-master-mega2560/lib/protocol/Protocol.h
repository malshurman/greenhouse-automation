#pragma once

class Protocol {
public:
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    static Protocol& getInstance();

    void loopCommunicationProtocol();
    int getAutoPumpDurationSeconds();
    void writeDataToLCD();
private:
    Protocol();
};