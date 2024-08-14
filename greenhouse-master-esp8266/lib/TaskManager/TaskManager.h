#pragma once

class TaskManager
{
public:
    TaskManager(const TaskManager &) = delete;
    TaskManager &operator=(const TaskManager &) = delete;

    static TaskManager &getInstance();

    void createTask(unsigned long interval, void (*callback)());
    void execute();

private:
    TaskManager();
    ~TaskManager();
};