import 'package:flutter/material.dart';

class Task{
  String taskName;
  TimeOfDay startTime;
  TimeOfDay endTime;
  bool isChecked = false;

  Task(this.taskName, this.startTime, this.endTime);
}