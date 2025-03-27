import 'package:flutter/material.dart';
import 'package:pip_boi/achievements_page.dart';
import 'package:pip_boi/settings_page.dart';
import 'package:pip_boi/task_page.dart';
import 'package:provider/provider.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (context) => MyAppState(),
      child: MaterialApp(
        title: 'Pip-Boy',
        theme: ThemeData(
          useMaterial3: true,
          colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepOrange),
        ),
        home: MyHomePage(),
      ),
    );
  }
}

class MyAppState extends ChangeNotifier {
  var tasks = <String>["Tag opvasken", "Lav lektier", "Task 4"]; // list of tasks

}

class MyHomePage extends StatefulWidget {
  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      body: Column(
        children: [
          Expanded(
            child: Container(
              color: Theme.of(context).colorScheme.primaryContainer, // const Color.fromARGB(1, 45, 111, 255),)
              child: TaskButton(),
            ),
          ),
          Expanded(
            child: Container(
              color: Theme.of(context).colorScheme.primaryContainer,
              child: TaskList(),
            ),
          ),
          Expanded(
            child: Container(
              color: Theme.of(context).colorScheme.secondaryContainer,
              child: ButtomButtons(),
            ),
          ),
        ],
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
                MaterialPageRoute(builder: (context) => AchievementsPage()),
              );
            },
            style: ElevatedButton.styleFrom(
                backgroundColor: const Color.from(alpha: 1, red: 0, green: 0, blue: 0),
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
                MaterialPageRoute(builder: (context) => SettingsPage()),
              );
            },
            style: ElevatedButton.styleFrom(
                backgroundColor: const Color.from(alpha: 1, red: 0, green: 0, blue: 0),
                foregroundColor: Colors.white
              ),
            child: Text('Settings'),
          ),
        ],
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
            MaterialPageRoute(builder: (context) => TaskPage()),
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

class TaskList extends StatelessWidget {


  @override
  Widget build(BuildContext context) {
    var appState = context.watch<MyAppState>();

    return Center(
      child: ListView(
        padding: const EdgeInsets.fromLTRB(100, 0, 100, 0),
        children: [
          for (var task in appState.tasks)
            CheckboxListTile(
              value: false,
              onChanged: (value) {
                
              },
              title: Text(task),
              
            ),
        ],
      ),
    );
  }
}
