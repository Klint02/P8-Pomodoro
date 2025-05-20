#ifndef TASK_CONTROL_SERVICES
#define TASK_CONTROL_SERVICES
namespace TCS {
    class Task
        {
        public:
            Task(JSONVar obj);
            std::string taskName;
            int id;
            int weekDay;
            int startHour;
            int startMinute;
            int duration;
            int timesCompleted;
            bool isComplete;
            bool isRecurring;
            std::vector<int> timers;

            void init();

            friend std::ostream& operator<<(std::ostream& os, const Task& obj);
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
        int getIndex();
        void addTime();
        void taskDoneEarly();
        int getTrophys();
        void taskPriority(int hour);
    };

}
#endif