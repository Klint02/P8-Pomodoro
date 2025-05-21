import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:pip_boi/taskProvider.dart';
import 'package:pip_boi/_task.dart';

void main() {
  group("TaskProvider Tests", () {
    late TaskProvider taskProvider;

    setUp(() {
      taskProvider = TaskProvider();
    });

    test("Add single task to Monday", () {
      final task = Task(
        taskName: 'Morning Task',
        startTime: const TimeOfDay(hour: 9, minute: 0),
        endTime: const TimeOfDay(hour: 10, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task, 0);
      final tasks = taskProvider.getTasksForDay(0);

      expect(tasks.length, 1);
      expect(tasks[0].taskName, 'Morning Task');
    });

    test("Add multiple tasks in order", () {
      final task1 = Task(
        taskName: 'First',
        startTime: const TimeOfDay(hour: 8, minute: 0),
        endTime: const TimeOfDay(hour: 9, minute: 0),
        isRecurring: false,
      );
      final task2 = Task(
        taskName: 'Second',
        startTime: const TimeOfDay(hour: 10, minute: 0),
        endTime: const TimeOfDay(hour: 11, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task2, 1);
      taskProvider.addTask(task1, 1);

      final tasks = taskProvider.getTasksForDay(1);
      expect(tasks[0].taskName, 'First');
      expect(tasks[1].taskName, 'Second');
    });

    test("Change task start time and reorders correctly", () {
      final task1 = Task(
        taskName: 'Task A',
        startTime: const TimeOfDay(hour: 9, minute: 0),
        endTime: const TimeOfDay(hour: 10, minute: 0),
        isRecurring: false,
      );
      final task2 = Task(
        taskName: 'Task B',
        startTime: TimeOfDay(hour: 11, minute: 0),
        endTime: const TimeOfDay(hour: 12, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task1, 2);
      taskProvider.addTask(task2, 2);

      final updated = Task(
        taskName: task2.taskName,
        startTime: const TimeOfDay(hour: 8, minute: 30),
        endTime: task2.endTime,
        isRecurring: task2.isRecurring,
        timesCompleted: task2.timesCompleted,
      );

      taskProvider.changeTask(updated, 2, 1);

      final tasks = taskProvider.getTasksForDay(2);

      expect(tasks[0].taskName, 'Task B');
      expect(tasks[1].taskName, 'Task A');
    });

    test("Remove task", () {
      final task = Task(
        taskName: 'To be removed',
        startTime: const TimeOfDay(hour: 10, minute: 0),
        endTime: const TimeOfDay(hour: 11, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task, 3);
      expect(taskProvider.getTasksForDay(3).length, 1);

      taskProvider.removeTask(3, 0);
      expect(taskProvider.getTasksForDay(3).length, 0);
    });

    test("Complete a non-recurring task and remove it", () {
      final task = Task(
        taskName: 'One time task',
        startTime: const TimeOfDay(hour: 7, minute: 0),
        endTime: const TimeOfDay(hour: 8, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task, 4);

      final output = TaskOutput(
        taskName: task.taskName,
        id: task.id,
        weekDay: 4,
        startHour: 7,
        startMinute: 0,
        duration: 60,
        timesCompleted: 0,
        isRecurring: false,
        isComplete: true,
      );

      taskProvider.updateFromInput([output]);

      expect(taskProvider.getTasksForDay(4).isEmpty, true);
      expect(taskProvider.completedTasks.length, 1);
    });

    test("Complete a recurring task and keep it", () {
      final task = Task(
        taskName: 'Recurring task',
        startTime: const TimeOfDay(hour: 7, minute: 0),
        endTime: const TimeOfDay(hour: 8, minute: 0),
        isRecurring: true,
      );

      taskProvider.addTask(task, 5);

      final output = TaskOutput(
        taskName: task.taskName,
        id: task.id,
        weekDay: 5,
        startHour: 7,
        startMinute: 0,
        duration: 60,
        timesCompleted: 1,
        isRecurring: true,
        isComplete: true,
      );

      taskProvider.updateFromInput([output]);

      expect(taskProvider.getTasksForDay(5).length, 1);
      expect(taskProvider.getTasksForDay(5)[0].timesCompleted, 1);
    });

    test("Convert tasks to output format", () {
      final task = Task(
        taskName: 'Export Me',
        startTime: const TimeOfDay(hour: 12, minute: 0),
        endTime: const TimeOfDay(hour: 13, minute: 0),
        isRecurring: false,
      );

      taskProvider.addTask(task, 6);

      final output = taskProvider.convertToOutput();
      expect(output.length, 1);
      expect(output[0].taskName, 'Export Me');
      expect(output[0].duration, 60);
      expect(output[0].weekDay, 6);
    });

    test("Do not fail silently when updating unknown task", () {
      final task = TaskOutput(
        taskName: 'Ghost Task',
        id: 999,
        weekDay: 0,
        startHour: 9,
        startMinute: 0,
        duration: 60,
        timesCompleted: 0,
        isRecurring: false,
        isComplete: true,
      );

      taskProvider.updateFromInput([task]);

      expect(taskProvider.completedTasks.isEmpty, true);
      expect(taskProvider.getTasksForDay(0).isEmpty, true);
    });

    test("Completing a task increases its timesCompleted count", () {
      // Step 1: Create and add a task
      final task = Task(
        taskName: 'Repeatable Task',
        startTime: const TimeOfDay(hour: 9, minute: 0),
        endTime: const TimeOfDay(hour: 10, minute: 0),
        isRecurring: true,
      );

      taskProvider.addTask(task, 1); // Add to Tuesday

      // Step 2: Simulate marking the task as complete by creating a TaskOutput
      final output = TaskOutput(
        taskName: task.taskName,
        id: task.id,
        weekDay: 1,
        startHour: task.startTime.hour,
        startMinute: task.startTime.minute,
        duration: 60,
        timesCompleted: 0, // initial value (will be incremented)
        isRecurring: true,
        isComplete: true, // Mark it complete
      );

      // Step 3: Update the provider with the output
      taskProvider.updateFromInput([output]);

      // Step 4: Fetch the updated task
      final updatedTask = taskProvider.getTasksForDay(1)
          .firstWhere((t) => t.id == task.id, orElse: () => throw Exception('Task not found'));

      // Step 5: Verify timesCompleted is incremented
      expect(updatedTask.timesCompleted, 1);
      expect(updatedTask.isComplete, true);
    });

  });
}
