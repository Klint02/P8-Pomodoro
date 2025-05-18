#ifndef TASK_CONTROL_SERVICES
#include "Ds1302.h"
#define TASK_CONTROL_SERVICES


namespace task {
     class Task
    {
    public:
        Task(String, int,int,int,int,bool,bool);
        String taskName;
        int id;
        int weekDay;
        int startHour;
        int duration;
        bool isComplete;
        bool isRecurring;
        std::vector<int> timers;

        void init();
    };

    class TaskControlService
    {
    private:
        int amountOfTasks;
        int tasksComplete;
        int trophys;
        int index;

    public:
        std::vector<Task> tasks;
        int getTrophy();
        void TimerDone();
        int NumberOfTasks();
        void initTasks();
        int TasksCompleted ();
        void NewTrophy();
        void debug();
        int getIndex();
        void addTime();
        void taskDoneEarly();
        void initTrophys();
        void taskPriority(const Ds1302::DateTime& dt);
    };
}
#endif