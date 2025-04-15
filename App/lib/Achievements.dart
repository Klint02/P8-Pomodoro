import 'dart:convert';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class AchievementsTab extends StatefulWidget {
  const AchievementsTab({super.key});

  @override
  State<AchievementsTab> createState() => _AchievementsTabState();
}

class _AchievementsTabState extends State<AchievementsTab> {
  int _counter = 0;
  List<int> completedTasks = [];
  List _items = [];
  int _taskCompleted = 0;

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

  Future<void> readJson() async {
    final String response = await rootBundle.loadString('assets/tasks.json');
    final data = await json.decode(response);
    setState(() {
      _items = data["tasks"];
    });
    // print(_items[1]["name"]);
  }

  void countTasks() {
    for (var item in _items) {
      if (item["completed"].toString() == "Yes") {
        _taskCompleted++;
      }
    }
  }

  @override
  void initState() {
    readJson();
    print("hello");
    for(var item in _items){
      print(item);
    }
    print(_items);
    countTasks();
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Achievements'),
      ),

      body: Column(
        children: [
          Expanded(
            child: ListView(
              padding: const EdgeInsets.all(8),
              children: <Widget>[
                Column(
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: <Widget>[
                    Container(
                      width: 300,
                      margin: EdgeInsets.all(10),
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
                      width: 300,
                      margin: EdgeInsets.all(10),
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
                          const Text('Tasks completed:'),
                          const Divider(thickness: 2),
                          Text('1 task completed, just getting started.'),
                          Text('5 tasks completed, getting warmed up.'),
                          Text('10 tasks completed, now were talking.'),
                          Text('25 tasks completed, your on fire.'),
                        ],
                      ),
                    ),
                    Container(
                      width: 300,
                      margin: EdgeInsets.all(10),
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
                          const Text('Load from json:'),
                          const Divider(thickness: 2),
                          FutureBuilder(
                              future: readJson(),
                              builder: (context, snapshot) {
                                if (snapshot.connectionState == ConnectionState.waiting) {
                                  return const CircularProgressIndicator();
                                }
                                if (snapshot.hasError) {
                                  return Center(
                                    child: Text("An error occurred"),
                                  );
                                } else {
                                  return Center(child: Text("works"),
                                  );
                                }
                              }
                          ),
                          // for (var i=1; i<=_taskCompleted; i++) Text("$i tasks completed."),
                        ],
                      ),
                    ),
                    Container(
                      width: 300,
                      margin: EdgeInsets.all(10),
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
                          const Text('Misc:'),
                          const Divider(thickness: 2),
                          Text('Write something here'),
                          Text('A bit more'),
                          Text('More'),
                          Text('MORE!'),
                          Text('...'),
                          Text('Maybe a bit overkill'),
                        ],
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),

          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.end,
            children: [
              FilledButton(
                onPressed: () {_counter = 0;},
                child: const Text('Reset counter'),
              ),

              FilledButton(
                onPressed: () {
                  _incrementCounter();
                  _addAchievement();
                },
                child: const Icon(Icons.add),
              ),
            ],
          )
        ],
      ),
    );
  }
}
