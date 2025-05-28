import 'package:flutter/material.dart';

class AchievementsTab extends StatefulWidget {
  // const AchievementsTab({super.key, required this.title});
  const AchievementsTab({super.key});

  // final String title;

  @override
  State<AchievementsTab> createState() => _AchievementsTabState();
}

class _AchievementsTabState extends State<AchievementsTab> {
  int _counter = 0;
  List<int> completedTasks = [];

  void _incrementCounter() {
    setState(() {
      _counter++;
    });
  }

  void _addAchievement() {
    setState(() {
      if(_counter == 1 || _counter%5 == 0){
        completedTasks.add(_counter);
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        // backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text('Achievements'),
      ),
      body: Column( children: [Center(
        child: Column(children: <Widget>[Container(
          width: 150,
          // height: 200,
          // margin: EdgeInsets.all(80),
          padding: EdgeInsets.all(10),
          decoration: BoxDecoration(
            color: Colors.white,
            border: Border.all(color: Colors.black, width: 2),
            borderRadius: BorderRadius.circular(8),
          ),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.start,
            children: <Widget>[
              // const Text('You have pushed the button this many times:'),
              // Text(
              //   '$_counter',
              //   style: Theme.of(context).textTheme.headlineMedium,
              // ),
              const Text('Streaks:'),
              const Divider(thickness: 2),
              for (var item in completedTasks) Text("Completed $item tasks."),
            ],
          ),
        ),
          Container(
          width: 150,
          // height: 200,
          // margin: EdgeInsets.all(80),
          padding: EdgeInsets.all(10),
          decoration: BoxDecoration(
            color: Colors.white,
            border: Border.all(color: Colors.black, width: 2),
            borderRadius: BorderRadius.circular(8),
          ),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.start,
            children: <Widget>[
              // const Text('You have pushed the button this many times:'),
              // Text(
              //   '$_counter',
              //   style: Theme.of(context).textTheme.headlineMedium,
              // ),
              const Text('Misc:'),
              const Divider(thickness: 2),
              Text('Write something here'),
              Text('A bit more'),
              Text('More'),
              Text('MORE'),
              Text('...'),
              Text('Maybe a bit much'),
            ],
          ),
        ),],
      ),),
      Row(
        children: [
          FilledButton(
            onPressed: () {_counter = 0;},
            child: const Text('Reset counter'),
          ),
          FilledButton(
          onPressed: () {_incrementCounter();_addAchievement();},
          child: const Icon(Icons.add),
        ),],
      ),
      ],),
    );
  }
}
