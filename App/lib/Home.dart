import 'package:flutter/material.dart';
import 'package:pip_boi/Achievements.dart';
import 'package:pip_boi/Settings.dart';
import 'package:pip_boi/Tasklist.dart';
import 'package:pip_boi/_task.dart';
import 'package:provider/provider.dart';
import 'package:pip_boi/taskProvider.dart';
//import 'package:provider/provider.dart';

class Home extends StatelessWidget {
  const Home({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Home')),
      body: Center(child:
      Column(
        crossAxisAlignment: CrossAxisAlignment.center,
        mainAxisAlignment: MainAxisAlignment.spaceAround,
        children: [
          Expanded(
            child: Container(
              color: Theme
                  .of(context)
                  .colorScheme
                  .primaryContainer, // const Color.fromARGB(1, 45, 111, 255),)
              child: TaskButton(),
            ),
          ),
          Expanded(
            child: Container(
              color: Theme
                  .of(context)
                  .colorScheme
                  .primaryContainer,
              child: TaskList(),
            ),
          ),
          Expanded(
            child: Container(
              color: Theme
                  .of(context)
                  .colorScheme
                  .secondaryContainer,
              child: ButtomButtons(),
            ),
          ),
        ],
      ),
      ),
    );
  }
}

class TaskButton extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: ElevatedButton.icon(
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (context) => Tasklist()),
          );
        },
        label: Text('Tasks'),
        icon: Icon(Icons.arrow_forward),
        style: ElevatedButton.styleFrom(
            backgroundColor: const Color.fromARGB(255, 33, 114, 253),
            foregroundColor: Colors.black
        ),
        iconAlignment: IconAlignment.end,
      ),
    );
  }
}

class TaskList extends StatefulWidget {
  @override
  State<TaskList> createState() => _TaskListState();
}

class _TaskListState extends State<TaskList> {

  int today = DateTime.now().weekday-1;

  @override
  void initState() {
    super.initState();

    //today = DateTime.now().weekday-1;
  }

  @override
  Widget build(BuildContext context) {
    var taskProvider = Provider.of<TaskProvider>(context);
    var tasks = taskProvider.getTasksForDay(today);

    return Center(
      child: ListView.builder(
        padding: const EdgeInsets.fromLTRB(100, 0, 100, 0),
        itemCount: tasks.length,
        itemBuilder: (BuildContext context, int index) {
          return CheckboxListTile(
              value: tasks[index].isComplete,
              onChanged: (bool? value) {
                setState(() {
                  tasks[index].isComplete = value!;
                });
              },
              title: Text("${tasks[index].taskName} at ${tasks[index].startTime.format(context)}")
          );
        },
      ),
    );
  }
}

class ButtomButtons extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    //var appState = context.watch<MyAppState>();

    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [

          ElevatedButton(
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => AchievementsTab()),
              );
            },
            style: ElevatedButton.styleFrom(
                backgroundColor: const Color.from(
                    alpha: 1, red: 0, green: 0, blue: 0),
                foregroundColor: Colors.white
            ),
            child: Text('Achievements'),
          ),
          //SizedBox(width: 50),
          Padding(padding: const EdgeInsets.all(10),),
          ElevatedButton(
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => SettingsTab()),
              );
            },
            style: ElevatedButton.styleFrom(
                backgroundColor: const Color.from(
                    alpha: 1, red: 0, green: 0, blue: 0),
                foregroundColor: Colors.white
            ),
            child: Text('Settings'),
          ),
        ],
      ),
    );
  }
}