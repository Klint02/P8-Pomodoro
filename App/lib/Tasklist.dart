import 'package:flutter/material.dart';
import 'package:pip_boi/_task.dart';
import 'package:provider/provider.dart';

class TaskProvider extends ChangeNotifier {
  List<List<Task>> week = List.generate(7, (_) => []);

  void addTask(Task input, int dayIndex) {
    if (week[dayIndex].isEmpty) {
      week[dayIndex].add(input);
    }
    else {
      for (int i = 0; i < week[dayIndex].length; i++) {
        if (input.startTime.isBefore(week[dayIndex][i].startTime)) {

          week[dayIndex].insert(i, input);
          break;
        }
        else if (input.startTime.isAfter(week[dayIndex].last.startTime)){
          week[dayIndex].add(input);
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
      print("Test");
    }
    else if (input.startTime.isAfter(week[dayIndex].last.startTime)) {
      week[dayIndex].removeAt(taskIndex);
      week[dayIndex].add(input);
    }
    else {
      for (int i = 0; i < week[dayIndex].length; i++) {
        if (input.startTime.isBefore(week[dayIndex][i].startTime)) {
          _moveTask(i, taskIndex, dayIndex, input);
          print("Test2");
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
}

class Tasklist extends StatefulWidget {
  const Tasklist({super.key});

  @override
  State<Tasklist> createState() => _TasklistState();
}

class _TasklistState extends State<Tasklist> {
  late TextEditingController controller;

  int selectedDay = 0;
  var date = DateTime.now();
  late var firstDay;

  @override
  void initState() {
    super.initState();
    controller = TextEditingController();
    var currentDay = date;
    selectedDay = currentDay.weekday - 1;
    firstDay = DateTime.now().subtract(Duration(days: currentDay.weekday - 1));
  }

  @override
  void dispose() {
    controller.dispose();
    super.dispose();
  }

  Future<Task?> openDialog(bool changeTask, int index) async {
    var taskProvider = Provider.of<TaskProvider>(context, listen: false);
    var tasks = taskProvider.getTasksForDay(selectedDay);

    String title = changeTask ? "Change task" : "Create task";
    var defaultStartTime = TimeOfDay.now();
    var defaultEndTime = TimeOfDay(
        hour: defaultStartTime.hour + 1 > 23 ? 0 : defaultStartTime.hour + 1,
        minute: defaultStartTime.minute);

    TimeOfDay startTime =
        changeTask ? tasks[index].startTime : defaultStartTime;
    TimeOfDay endTime = changeTask ? tasks[index].endTime : defaultEndTime;

    if (changeTask) {
      controller.text = tasks[index].taskName;
    }

    return showDialog<Task>(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(title),
        content: StatefulBuilder(builder: (context, setStateDialog) {
          return Column(
            children: [
              TextField(
                autofocus: true,
                decoration: const InputDecoration(hintText: "Enter your task"),
                controller: controller,
              ),
              TextButton(
                  onPressed: () async {
                    final TimeOfDay? timeOfDay = await showTimePicker(
                        context: context,
                        initialTime: startTime,
                        initialEntryMode: TimePickerEntryMode.dial);
                    if (timeOfDay != null) {
                      setStateDialog(() {
                        startTime = timeOfDay;
                        if (!changeTask || startTime.isAfter(endTime)) {
                          endTime = TimeOfDay(
                              hour: startTime.hour + 1 > 23
                                  ? 0
                                  : startTime.hour + 1,
                              minute: startTime.minute);
                        }
                      });
                    }
                  },
                  child: const Text("Select the start time")),
              Text("Start time: ${startTime.hour}:${startTime.minute}"),
              TextButton(
                  onPressed: () async {
                    final TimeOfDay? timeOfDay = await showTimePicker(
                        context: context,
                        initialTime: endTime,
                        initialEntryMode: TimePickerEntryMode.dial);
                    if (timeOfDay != null) {
                      setStateDialog(() {
                        if (timeOfDay.isBefore(startTime)) {
                          endTime = TimeOfDay(
                              hour: startTime.hour + 1 > 23
                                  ? 0
                                  : startTime.hour + 1,
                              minute: startTime.minute);
                        } else {
                          endTime = timeOfDay;
                        }
                      });
                    }
                  },
                  child: const Text("Select the end time")),
              Text("End time: ${endTime.hour}:${endTime.minute}"),
              Spacer(),
              if (changeTask)
                TextButton(
                  onPressed: () {
                    taskProvider.removeTask(selectedDay, index);
                    Navigator.of(context).pop(Task("", startTime, endTime));
                    controller.clear();
                  },
                  child: const Text(
                    "Remove task",
                    style: TextStyle(color: Colors.red),
                  ),
                ),
            ],
          );
        }),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop(Task("", startTime, endTime));
              controller.clear();
            },
            child: const Text("Cancel"),
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context)
                  .pop(Task(controller.text, startTime, endTime));
              controller.clear();
            },
            child: const Text("Enter"),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    var taskProvider = Provider.of<TaskProvider>(context);
    var tasks = taskProvider.getTasksForDay(selectedDay);

    return Scaffold(
      appBar: AppBar(title: const Text('Task list')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: List.generate(7, (index) {
                return TextButton(
                  onPressed: () {
                    setState(() {
                      selectedDay = index;
                    });
                  },
                  child: Text(
                    '${firstDay.add(Duration(days: index)).day}/${firstDay.add(Duration(days: index)).month}', // Displaying 1 to 7 for days
                    style: TextStyle(
                      fontWeight: selectedDay == index
                          ? FontWeight.bold
                          : FontWeight.normal,
                      color: selectedDay == index ? Colors.blue : Colors.black,
                    ),
                  ),
                );
              }),
            ),
            const Text("Task for today:"),
            Expanded(
              child: ListView.builder(
                itemCount: tasks.length,
                itemBuilder: (context, index) {
                  return Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(
                          "${tasks[index].taskName} at ${tasks[index].startTime.format(context)} "
                          "to ${tasks[index].endTime.format(context)}"),
                      const SizedBox(width: 10),
                      FilledButton(
                        onPressed: () async {
                          final inputTask = await openDialog(true, index);
                          if (inputTask != null &&
                              inputTask.taskName.isNotEmpty) {
                            taskProvider.changeTask(
                                inputTask, selectedDay, index);
                          }
                        },
                        child: const Icon(Icons.settings),
                      ),
                      const SizedBox(width: 10),
                    ],
                  );
                },
              ),
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                FilledButton(
                  onPressed: () async {
                    final inputTask = await openDialog(false, 0);
                    if (inputTask != null && inputTask.taskName.isNotEmpty) {
                      taskProvider.addTask(inputTask, selectedDay);
                    }
                  },
                  child: const Text("Add"),
                ),
              ],
            ),
            FilledButton(
              onPressed: () {
                Navigator.pop(context);
              },
              child: const Text('Back'),
            ),
            const SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}
