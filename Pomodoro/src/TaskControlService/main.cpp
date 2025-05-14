#include <iostream>
#include "TaskControlService.hpp"

int main(){

    task::Task task1;
    task::Task task2;
    task::Task task3;
    task::Task task4;
    task1.duration = 60;

    std::cout << task1.duration;

}