import 'package:flutter/material.dart';
import 'package:pip_boi/_task.dart';
import 'package:provider/provider.dart';

class TaskProvider extends ChangeNotifier {
  List<List<Task>> week = List.generate(7, (_) => []);
  List<Task> completedTasks = [];

  void addTask(Task input, int dayIndex) {
    if (week[dayIndex].isEmpty || input.startTime.isAfter(week[dayIndex].last.startTime)){
      week[dayIndex].add(input);
    }
    else {
      for (int i = 0; i < week[dayIndex].length; i++) {
        if (input.startTime.isBefore(week[dayIndex][i].startTime) ||
            input.startTime.isAtSameTimeAs(week[dayIndex][i].startTime)) {

          week[dayIndex].insert(i, input);
          break;
        }
      }
    }
    notifyListeners();
  }

  void changeTask(Task input, int dayIndex, int taskIndex) {

    var oldStartTime = week[dayIndex][taskIndex].startTime;
    late int newIndex;

    if (week[dayIndex].length == 1 || oldStartTime.isAtSameTimeAs(input.startTime)) {
      week[dayIndex][taskIndex] = input;
    }
    else if (input.startTime.isAfter(week[dayIndex].last.startTime)) {
      week[dayIndex].removeAt(taskIndex);
      week[dayIndex].add(input);
    }
    else {
      for (int i = 0; i < week[dayIndex].length; i++) {
        if (input.startTime.isBefore(week[dayIndex][i].startTime) ||
            input.startTime.isAtSameTimeAs(week[dayIndex][i].startTime)) {
          _moveTask(i, taskIndex, dayIndex, input);
          break;
        }
      }
    }
    notifyListeners();
  }

  void _moveTask(int newIndex, int oldIndex, int dayIndex, input) {

    if (newIndex < oldIndex) {
      week[dayIndex].removeAt(oldIndex);
      week[dayIndex].insert(newIndex, input);
    }
    else {
      week[dayIndex].insert(newIndex, input);
      week[dayIndex].removeAt(oldIndex);
    }
  }

  void removeTask(int dayIndex, int taskIndex) {
    week[dayIndex].removeAt(taskIndex);
    notifyListeners();
  }

  List<Task> getTasksForDay(int dayIndex) {
    return week[dayIndex];
  }

  int _findDuration(TimeOfDay start, TimeOfDay end) {
    int startInMinutes = start.hour * 60 + start.minute;
    int endInMinutes = end.hour * 60 + end.minute;

    int difference = endInMinutes - startInMinutes;
    return difference;
  }

  List<TaskOutput> convertToOutput() {
    List<TaskOutput> temp = [];
    for (int i = 0; i < week.length; i++) {
      for (var task in week[i]) {
        temp.add(TaskOutput(
          taskName: task.taskName,
          id: task.id,
          weekDay: i,
          startHour: task.startTime.hour,
          startMinute: task.startTime.minute,
          duration: _findDuration(task.startTime, task.endTime),
          isRecurring: task.isRecurring,
        ));
      }
    }

    return temp;
  }


  void updateFromInput(List<TaskOutput> inputList) {
    for (var task in inputList) {
      print('Updating: '
          'taskName=${task.taskName}, id=${task.id}, '
          'weekDay=${task.weekDay}, start=${task.startHour}:${task.startMinute}, '
          'duration=${task.duration}, isComplete=${task.isComplete}');

      if (task.isComplete) {
        final dayTasks = week[task.weekDay];
        final index = dayTasks.indexWhere((t) => t.id == task.id);

        if (index != -1) {
          dayTasks[index].isComplete = true;
          completedTasks.add(dayTasks[index]);
          removeTask(task.weekDay, index);
        } else {
          print('Task with id=${task.id} not found in week[${task.weekDay}].');
        }
      }
    }
  }
}