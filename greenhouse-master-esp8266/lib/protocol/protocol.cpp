#include <Protocol.h>
#include <TaskManager.h>

#define TRANSMISSION_RATE 150

static void loopCommunicationProtocolTask()
{
    Protocol::getInstance().loopCommunicationProtocol();
}

Protocol &Protocol::getInstance()
{
    static Protocol instance;
    return instance;
}

Protocol::Protocol() : context(new Context())
{
    TaskManager::getInstance().createTask(TRANSMISSION_RATE, loopCommunicationProtocolTask);
}

Protocol::~Protocol()
{
    delete context;
}

void Protocol::loopCommunicationProtocol()
{
    context->execute();
}