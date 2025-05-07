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
  bool isComplete = false;
  bool isRecurring;

  TaskOutput(this.taskName, this.weekDay, this.startHour, this.startMinute, this.isRecurring);

  Map<String, dynamic> toJson() => {'taskname':taskName, 'weekday': weekDay, 'starthour': startHour, 'startminute': startMinute, 'iscomplete': isComplete, 'isrecurring': isRecurring};
}