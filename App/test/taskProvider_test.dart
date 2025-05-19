import 'package:flutter/material.dart';
import 'package:pip_boi/taskProvider.dart';
import 'package:test/test.dart';
import 'package:pip_boi/_task.dart';

void main() {
  group("taskProvider Tests", () {
    late TaskProvider taskProvider;

    setUp(() {
      taskProvider = TaskProvider();
    });

    test("Add task to monday", () {
      final task = Task(
        taskName: 'Task 1',
        startTime: const TimeOfDay(hour: 9, minute: 0),
        endTime: const TimeOfDay(hour: 10, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task, 0);

      final tasks = taskProvider.getTasksForDay(0);
      expect(tasks.length, 1);
    });
  });
}
