#include <iostream>
#include <vector>
#include <Arduino_JSON.h>
#include "TaskControlService.hpp"

namespace TSC
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
}