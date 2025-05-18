#ifndef TASK_CONTROL_SERVICES
#define TASK_CONTROL_SERVICES
namespace TSC {
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
}
#endif