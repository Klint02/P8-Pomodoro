import 'package:flutter/material.dart';

class Task{
  String taskName;
  TimeOfDay time;
  bool isChecked = false;

  Task(this.taskName, this.time);
}