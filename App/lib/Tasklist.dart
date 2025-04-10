import 'package:flutter/material.dart';
import 'package:pip_boi/_global.dart';
import 'package:pip_boi/_task.dart';

enum weekDays {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday}

class Tasklist extends StatefulWidget {
  const Tasklist({super.key});

  @override
  State<Tasklist> createState() => _TasklistState();
}


class _TasklistState extends State<Tasklist> {
  late TextEditingController controller;
  List<Task> _taskList = [];

  int selectedDay = 0;
  var date = DateTime.now();
  late var firstDay;


  TimeOfDay currentTime = TimeOfDay.now();


  void _addTask(Task input) {
    setState(() {
      week[selectedDay].add(input);
    });
    controller.clear();
  }

  void _changeTask(Task input, int index) {
    setState(() {
      week[selectedDay][index] = input;
    });
    controller.clear();
  }



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

  Future<Task?> openDialog() async {
    return showDialog<Task>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text("Task:"),
        content: Column(
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
                    setState(() {
                      currentTime = timeOfDay;
                    });
                  }
                },
                child: const Text("Select a time"))
          ],
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop(Task(controller.text, currentTime));
            },
            child: const Text("Enter"),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
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
                itemCount: week[selectedDay].length,
                itemBuilder: (context, index) {
                  return Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(
                          "${week[selectedDay][index].taskName} at ${week[selectedDay][index].time.format(context)}"),
                      const SizedBox(width: 10), // Fixed spacing issue
                      FilledButton(
                        onPressed: () async {
                          final inputTask = await openDialog();
                          if (inputTask != null &&
                              inputTask.taskName.isNotEmpty) {
                            _changeTask(inputTask, index);
                          }
                        },
                        child: const Icon(Icons.settings),
                      ),
                      const SizedBox(width: 10),
                      FilledButton(
                        onPressed: () {
                          setState(() {
                            week[selectedDay].removeAt(index);
                          });
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
                    final inputTask = await openDialog();
                    if (inputTask != null && inputTask.taskName.isNotEmpty) {
                      print(inputTask);
                      _addTask(inputTask);
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
