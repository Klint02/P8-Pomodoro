#include <iostream>
#include <vector>
#include <Arduino_JSON.h>
#include "TaskControlService.hpp"

namespace TCS
{
    Task::Task(JSONVar obj) {
        String tmp_name = obj["taskName"];
        taskName = tmp_name.c_str(); 
        id = obj["id"];
        weekDay = obj["weekDay"];
        startHour = obj["startHour"];
        startMinute = obj["startMinute"];
        duration = obj["duration"];
        timesCompleted = obj["timesCompleted"];
        isComplete = obj["isComplete"];
        isRecurring = obj["isRecurring"];
    }

    std::ostream& operator<<(std::ostream& os, const Task& obj)
    {
        //This is slow as hell so only use it for saving to flash
        JSONVar tmp;
        String tmp_name = obj.taskName.c_str();
        tmp["taskName"] = tmp_name;
        tmp["id"] = (int) obj.id;
        tmp["weekDay"] = (int) obj.weekDay;
        tmp["startHour"] = (int) obj.startHour;
        tmp["startMinute"] = (int) obj.startMinute;
        tmp["duration"] = (int) obj.duration;
        tmp["timesCompleted"] = (int) obj.timesCompleted;
        tmp["isComplete"] = (bool) obj.isComplete;
        tmp["isRecurring"] = (bool) obj.isRecurring;
        os << JSON.stringify(tmp).c_str();
        return os;
    }
    void Task::init()
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


    void TaskControlService::TimerDone()
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

        int TaskControlService::NumberOfTasks(){
            amountOfTasks = tasks.size();
         
            return amountOfTasks;
        }

        void TaskControlService::initTasks()
        {
            for (auto &task : tasks)
            {
                task.init();
            }
        }

        int TaskControlService::TasksCompleted (){
            tasksComplete = 0;
            for (auto task : tasks)
            {
                if(task.isComplete){
                    tasksComplete++;
                } 
            }
            return tasksComplete;
        }

        void TaskControlService::NewTrophy()
        {
            trophys++;
        }

        void TaskControlService::debug()
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
        
        int TaskControlService::getIndex(){
            return index;
        }
        void TaskControlService::addTime(){
            // add 25 min to timer.
            tasks[index].timers.push_back(25);
            tasks[index].duration += 25;
        }
        void TaskControlService::taskDoneEarly(){
            // stops timer and completes task if done early.
            tasks[index].timers.clear();
            TimerDone();
        }

}
