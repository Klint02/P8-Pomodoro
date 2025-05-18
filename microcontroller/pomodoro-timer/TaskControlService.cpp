#include <iostream>
#include <vector>
#include <Arduino.h>
#include <Ds1302.h>
#include "TaskControlService.hpp"
namespace task
{
    Task::Task(String _taskName, int _id, int _weekDay, int _startHour, int _duration, bool _isComplete, bool _isRecurring)
    {
        taskName = _taskName;
        id = _id;
        weekDay = _weekDay;
        startHour = _startHour;
        duration = _duration;
        isComplete = _isComplete;
        isRecurring = _isRecurring;
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
            tasks[index].timers.erase(this->tasks[index].timers.begin());
            if (tasks[index].timers.empty())
            {
                tasks[index].isComplete = true;
                index++;
            }
        }
    }

    int TaskControlService::NumberOfTasks()
    {
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

    int TaskControlService::TasksCompleted()
    {
        tasksComplete = 0;
        for (auto task : tasks)
        {
            if (task.isComplete)
            {
                tasksComplete++;
            }
        }
        return tasksComplete;
    }
    int TaskControlService::getTrophy()
    {
        return trophys;
    }

    void TaskControlService::NewTrophy()
    {
        trophys++;
    }

    void TaskControlService::debug()
    {
        std::cout << "Task timers:\n";
        for (int i = 0; i < tasks.size(); i++)
        {
            std::cout << "Task " << i << ": ";
            for (int j = 0; j < this->tasks[i].timers.size(); j++)
            {
                std::cout << this->tasks[i].timers[j] << " ";
            }
            std::cout << std::endl;
        }
    }

    int TaskControlService::getIndex()
    {
        return index;
    }
    void TaskControlService::addTime()
    {
        // add 25 min to timer.
        tasks[index].timers.push_back(25);
        tasks[index].duration += 25;
    }
    void TaskControlService::taskDoneEarly()
    {
        // stops timer and completes task if done early.
        tasks[index].timers.clear();
        TimerDone();
    }
    void TaskControlService::initTrophys()
    {
        trophys = 0;
    }
    void TaskControlService::taskPriority(const Ds1302::DateTime &dt)
    {
        for (int i = 0; i < tasks.size(); i++)
        {
            if (tasks[i].isComplete == false && tasks[i].startHour > dt.hour)
            {
                index = i;
                return;
            }
        }

        for (int i = 0; i < tasks.size(); i++)
        {
            if (tasks[i].isComplete == true)
            {
                index = i;
                return;
            }
        }
    }
}
