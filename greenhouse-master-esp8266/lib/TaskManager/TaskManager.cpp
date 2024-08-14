#include <TaskManager.h>
#include <TaskScheduler.h>
#include <Array.h>

#define TASK_COUNT_MAX 50

static Scheduler scheduler;
static Array<Task *, TASK_COUNT_MAX> tasks;

TaskManager &TaskManager::getInstance()
{
    static TaskManager instance;
    return instance;
}

TaskManager::TaskManager()
{
    scheduler.init();
}

TaskManager::~TaskManager()
{
    for (Task *task : tasks)
    {
        delete task;
    }
    tasks.clear();
}

void TaskManager::createTask(unsigned long interval, void (*callback)())
{

    Task *task = new Task(interval, TASK_FOREVER, callback);
    tasks.push_back(task);

    scheduler.addTask(*task);
    task->enable();
}

void TaskManager::execute()
{
    scheduler.execute();
}