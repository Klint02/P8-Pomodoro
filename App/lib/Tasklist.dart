import 'package:flutter/material.dart';
import 'package:pip_boi/_task.dart';
import 'package:provider/provider.dart';

class TaskProvider extends ChangeNotifier {
  List<List<Task>> week = List.generate(7, (_) => []);

  void addTask(Task input, int dayIndex) {
    week[dayIndex].add(input);
    notifyListeners();
  }

  void changeTask(Task input, int dayIndex, int taskIndex) {
    week[dayIndex][taskIndex] = input;
    notifyListeners();
  }

  void removeTask(int dayIndex, int taskIndex) {
    week[dayIndex].removeAt(taskIndex);
    notifyListeners();
  }

  List<Task> getTaskForDay(int dayIndex){
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
    selectedDay = currentDay.weekday-1;
    firstDay = DateTime.now().subtract(Duration(days:currentDay.weekday-1));
  }

  @override
  void dispose() {
    controller.dispose();
    super.dispose();
  }

  Future<Task?> openDialog(bool changeTask, int index) async {
    var taskProvider = Provider.of<TaskProvider>(context, listen: false);
    String title = changeTask? "Change task" : "Create task";
    TimeOfDay currentTime = TimeOfDay.now();

    return showDialog<Task>(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(title),
        content: StatefulBuilder(
          builder: (context, setStateDialog) {
            return Column(
              children: [
                TextField(
                  autofocus: true,
                  decoration: const InputDecoration(hintText: "Enter your task"),
                  controller: controller,
                ),
                Text("${currentTime.hour}:${currentTime.minute}"),
                TextButton(
                    onPressed: () async {
                      final TimeOfDay? timeOfDay = await showTimePicker(
                          context: context,
                          initialTime: currentTime,
                          initialEntryMode: TimePickerEntryMode.dial);
                      if (timeOfDay != null) {
                        setStateDialog(() {
                          currentTime = timeOfDay;
                        });
                      }
                    },
                    child: const Text("Select a time")
                ),
                if (changeTask)
                  TextButton(

                    onPressed: () {
                      taskProvider.removeTask(selectedDay, index);
                      Navigator.of(context).pop(Task("", currentTime));
                      controller.clear();
                    },
                    child: const Text(
                      "Remove task",
                      style: TextStyle(color: Colors.red),
                    ),
                  ),
              ],
            );
          }
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop(Task("", currentTime));
              controller.clear();
            },
            child: const Text("Cancel"),
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context).pop(Task(controller.text, currentTime));
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
    var tasks = taskProvider.getTaskForDay(selectedDay);

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
                    '${firstDay.add(Duration(days:index)).day}/${firstDay.add(Duration(days:index)).month}', // Displaying 1 to 7 for days
                    style: TextStyle(
                      fontWeight: selectedDay == index ? FontWeight.bold : FontWeight.normal,
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
                          "${tasks[index].taskName} at ${tasks[index].time.format(context)}"),
                      const SizedBox(width: 10),
                      FilledButton(
                        onPressed: () async {
                          final inputTask = await openDialog(true, index);
                          if (inputTask != null &&
                              inputTask.taskName.isNotEmpty) {
                            taskProvider.changeTask(inputTask, selectedDay, index);
                          }
                        },
                        child: const Icon(Icons.settings),
                      ),
                      const SizedBox(width: 10),
                      FilledButton(
                        onPressed: () {
                          taskProvider.removeTask(selectedDay, index);
                        },
                        child: const Icon(Icons.remove),
                      ),
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
