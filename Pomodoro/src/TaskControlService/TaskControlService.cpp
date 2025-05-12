#include <vector>
#include <iostream>
#include <string>
namespace task
{
    /*Class der skal gemme task info, manipulere tasks og trofæer*/

    class Task
    {
    public:
        std::string taskName;
        int id;
        int weekDay = 1;
        int startHour = 12;
        int duration = 45;
        bool isComplete = false;
        bool isRecurring = true;
        std::vector<int> timers = {};

        void init()
        {
            int totalTime = this->duration;
            this->timers.clear();
            while (totalTime > 0)
            {
                if (totalTime >= 25)
                {
                    this->timers.push_back(25);
                    totalTime -= 25;
                }
                else
                {
                    this->timers.push_back(totalTime);
                    totalTime -= totalTime;
                }
            }
        }
    };

    class TaskControlService
    {
    private:
        int amountOfTasks=0;
        int tasksComplete = 0;
        int trophys = 0;
        int index = 0;

    public:
        std::vector<Task> tasks = {};

        void TimerDone()
        {
            if (tasks[index].timers.size() > 0)
            {
                tasks[index].timers.erase(tasks[index].timers.begin());
                if (tasks[index].timers.empty())
                {
                    tasks[index].isComplete = true;
                    index++;
                }
            }
        }

        int NumberOfTasks(){
            amountOfTasks = tasks.size();
         
            return amountOfTasks;
        }

        void initTasks()
        {
            for (auto &task : tasks)
            {
                task.init();
            }
        }

        int TasksCompleted (){
            tasksComplete = 0;
            for (auto task : tasks)
            {
                if(task.isComplete){
                    tasksComplete++;
                } 
            }
            return tasksComplete;
        }

        void NewTrophy()
        {
            trophys++;
        }

        void debug()
        {
            std::cout << "Task timers:\n";
            for (int i = 0; i < this->tasks.size(); i++)
            {
                std::cout << "Task " << i << ": ";
                for (int j = 0; j < this->tasks[i].timers.size(); j++)
                {
                    std::cout << this->tasks[i].timers[j] << " ";
                }
                std::cout << std::endl;
            }
        }
        
        int debug2(){
            return index;
        }
        void addTime(){
            // add 25 min to timer.
            tasks[index].timers.push_back(25);
            tasks[index].duration += 25;
        }
        void taskDoneEarly(){
            // stops timer and completes task if done early.
            tasks[index].timers.clear();
            TimerDone();
        }
    };
}

int main()
{

    task::Task obj1;
    task::Task obj2;
    task::Task obj3;
    task::TaskControlService t;

    obj1.duration = 100;
    obj2.duration = 45;
    obj3.duration = 93;

    t.tasks.push_back(obj1);
    t.tasks.push_back(obj2);
    t.tasks.push_back(obj3);

    t.initTasks();

    t.debug();
    t.TimerDone();
    t.debug();
    t.TimerDone();
    t.debug();
    t.TimerDone();
    t.debug();
    t.TimerDone();
    std::cout<<t.tasks[0].isComplete<<std::endl;
    t.debug();
    t.TimerDone();
    t.debug();
    std::cout<<t.tasks.size()<<std::endl;
    
    return 17;
}