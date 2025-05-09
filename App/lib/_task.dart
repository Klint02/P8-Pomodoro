import 'package:flutter/material.dart';

class Task{
  String taskName;
  TimeOfDay startTime;
  TimeOfDay endTime;
  bool isChecked = false;
  bool isRecurring = false;

  Task(this.taskName, this.startTime, this.endTime);


}

class TaskOutput {
  String taskName;
  int weekDay;
  int startHour;
  int startMinute;
  int duration;
  bool isComplete;
  bool isRecurring;

  TaskOutput({
    required this.taskName,
    required this.weekDay,
    required this.startHour,
    required this.startMinute,
    required this.duration,
    this.isComplete = false,
    required this.isRecurring,
  });

  Map<String, dynamic> toJson() => {
    'taskName': taskName,
    'weekDay': weekDay,
    'startHour': startHour,
    'startMinute': startMinute,
    'isComplete': isComplete,
    'isRecurring': isRecurring
  };

  factory TaskOutput.fromJson(Map<String, dynamic> json) {
    return switch (json) {
      {
        'taskName' : String _taskName,
        'weekDay' : int _weekDay,
        'startHour' : int _startHour,
        'startMinute' : int _startMinute,
        'duration' : int _duration,
        'isComplete' : bool _isComplete,
        'isRecurring' : bool _isRecurring
      } => TaskOutput(
        taskName: _taskName,
        weekDay: _weekDay,
        startHour: _startHour,
        startMinute: _startMinute,
        duration: _duration,
        isComplete: _isComplete,
        isRecurring: _isRecurring
      ),
      _ => throw const FormatException('Failed to map json to task.'),
    };
  }
}