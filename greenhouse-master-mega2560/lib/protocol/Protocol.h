#pragma once

#include <States.h>

class Protocol {
public:
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    static Protocol& getInstance();

    void writeConnectionStatusToLCD();
    void loopCommunicationProtocol();
    int getAutoPumpDurationMinutes();
private:
    Protocol();
    ~Protocol();
    Context* context;
};