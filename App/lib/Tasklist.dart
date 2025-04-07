import 'package:flutter/material.dart';
import '_task.dart';

class Tasklist extends StatefulWidget {
  const Tasklist({super.key});

  @override
  State<Tasklist> createState() => _TasklistState();
}

class _TasklistState extends State<Tasklist> {
  late TextEditingController controller;
  List<Task> _taskList = [];
  List<List<Task>> week = [[],[]];
  int day = 0;
  TimeOfDay currentTime = TimeOfDay.now();


  void _addTask(Task input) {
    setState(() {
      week[day].add(input);
    });
  }

  void _changeTask(Task input, int index) {
    setState(() {
      week[day][index] = input;
    });
  }

  @override
  void initState() {
    super.initState();
    controller = TextEditingController();
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
                      initialEntryMode: TimePickerEntryMode.dial
                  );
                  if (timeOfDay != null) {
                    setState(() {
                      currentTime = timeOfDay;
                    });
                  }
                },
                child: const Text("Select a time")
            )
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
              children: [
                TextButton(onPressed: () {
                  setState(() {
                    day = 0;
                  });

                }, child: Text(
                    "1",
                  style: TextStyle(
                    fontWeight: day == 0 ? FontWeight.bold : FontWeight.normal,
                    color: day == 0 ? Colors.blue : Colors.black,
                  ),
                )
                ),
                TextButton(onPressed: () {
                  setState(() {
                    day = 1;
                  });
                }, child: Text(
                  "2",
                  style: TextStyle(
                    fontWeight: day == 1 ? FontWeight.bold : FontWeight.normal,
                    color: day == 1 ? Colors.blue : Colors.black,
                  ),
                )),
              ],
            ),
            const Text("Task for today:"),
            Expanded(
              child: ListView.builder(
                itemCount: week[day].length,
                itemBuilder: (context, index) {
                  return Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text("${week[day][index].taskName} at ${week[day][index].time.format(context)}"),
                      const SizedBox(width: 10), // Fixed spacing issue
                      FilledButton(
                        onPressed: () async {
                          final inputTask = await openDialog();
                          if (inputTask != null && inputTask.taskName.isNotEmpty) {
                            _changeTask(inputTask, index);
                          }
                        },
                        child: const Icon(Icons.settings),
                      ),
                      const SizedBox(width: 10),
                      FilledButton(
                        onPressed: () {
                          setState(() {
                            week[day].removeAt(index);
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
