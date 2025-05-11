import 'package:flutter/material.dart';
int _lastId = 0;
int _getNextId() => ++_lastId;

class Task{
  String taskName;
  TimeOfDay startTime;
  TimeOfDay endTime;
  final int id;
  bool isChecked = false;
  bool isRecurring = false;

  Task({
    required this.taskName,
    required this.startTime,
    required this.endTime,
  }) : id = _getNextId();
}

class TaskOutput {
  String taskName;
  int id;
  int weekDay;
  int startHour;
  int startMinute;
  int duration;
  bool isComplete;
  bool isRecurring;

  TaskOutput({
    required this.taskName,
    required this.id,
    required this.weekDay,
    required this.startHour,
    required this.startMinute,
    required this.duration,
    this.isComplete = false,
    required this.isRecurring,
  });

  Map<String, dynamic> toJson() => {
    'taskName': taskName,
    'id': id,
    'weekDay': weekDay,
    'startHour': startHour,
    'startMinute': startMinute,
    'duration': duration,
    'isComplete': isComplete,
    'isRecurring': isRecurring
  };

  factory TaskOutput.fromJson(Map<String, dynamic> json) {
    return switch (json) {
      {
        'taskName' : String _taskName,
        'id' : int _id,
        'weekDay' : int _weekDay,
        'startHour' : int _startHour,
        'startMinute' : int _startMinute,
        'duration' : int _duration,
        'isComplete' : bool _isComplete,
        'isRecurring' : bool _isRecurring
      } => TaskOutput(
        taskName: _taskName,
        id: _id,
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