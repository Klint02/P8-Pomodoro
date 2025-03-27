import 'package:flutter/material.dart';
import 'package:pip_boi/Achievements.dart';
import 'package:pip_boi/Settings.dart';
import 'package:pip_boi/Tasklist.dart';
//import 'package:provider/provider.dart';

class Home extends StatelessWidget {
  const Home({super.key});

  @override
  Widget build(BuildContext context){
    return Scaffold(
      appBar: AppBar(title: const Text('Home')),
      body: Center(child:
        Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          mainAxisAlignment: MainAxisAlignment.spaceAround,
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
      ),
    );
  }
}

class MyAppState extends ChangeNotifier {
 var tasks = <String>["Tag opvasken", "Lav lektier", "Task 4"]; // list of tasks
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

class TaskList extends StatelessWidget {
  @override
  Widget build(BuildContext context) {

    return Center(
      child: ListView(
        padding: const EdgeInsets.fromLTRB(100, 0, 100, 0),
        children: [
          // for (var task in context.watch<MyAppState>().tasks)
          //   CheckboxListTile(
          //     value: false,
          //     onChanged: (value) {
                
          //     },
          //     title: Text(task),
              
          //   ),
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
                MaterialPageRoute(builder: (context) => AchievementsTab()),
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
                MaterialPageRoute(builder: (context) => SettingsTab()),
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