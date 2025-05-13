#ifndef TASK_CONTROL_SERVICES
#define TASK_CONTROL_SERVICES
// #include <iostream>
// #include <vector>
namespace task {
     class Task
    {
    public:
        std::string taskName;
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

        void TimerDone();
        int NumberOfTasks();

        void initTasks();

        int TasksCompleted ();

        void NewTrophy();

        void debug();
        
        int getIndex();
        void addTime();
        void taskDoneEarly();
    };
}
#endif