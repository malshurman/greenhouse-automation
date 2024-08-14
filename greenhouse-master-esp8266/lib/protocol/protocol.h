#pragma once

#include <States.h>

class Protocol
{
public:
    Protocol(const Protocol &) = delete;
    Protocol &operator=(const Protocol &) = delete;

    static Protocol &getInstance();

    void loopCommunicationProtocol();

private:
    Protocol();
    ~Protocol();
    Context *context;
};