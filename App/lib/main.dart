import 'package:flutter/material.dart';
import 'package:pip_boi/Home.dart';



void main() {
  runApp(const MaterialApp(
    // home: AchievementsTab(title: 'Achievements'),
    // home: Tasklist(),
    home: Home(),
    // home: Sandbox(),
  ));
}

class Sandbox extends StatelessWidget {
  const Sandbox({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sandbox'),
        backgroundColor: Colors.grey,
      ),
      body: Text('test'),
    );
  }
}

