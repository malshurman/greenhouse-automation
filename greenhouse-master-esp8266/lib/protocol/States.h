#pragma once

class State;

class Context
{
private:
    State *currentState;
    State *nextState;
    unsigned long lastStateChangeTimeMillis;

public:
    Context();
    unsigned long getLastStateChangeTimeMillis();
    void transitionTo(State *state);
    void execute();
};

class State
{
protected:
    Context *context;

public:
    virtual ~State() = default;
    virtual void handle() = 0;
    void setContext(Context *context);
};

class SendDataState : public State
{
public:
    void handle() override;
};

class ReceiveDataState : public State
{
public:
    void handle() override;
};

class AcknowledgeState : public State
{
public:
    void handle() override;
};

class NegativeAcknowledgeState : public State
{
public:
    void handle() override;
};