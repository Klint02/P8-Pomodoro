
import 'package:flutter/material.dart';
import 'package:pip_boi/Achievements.dart';
import 'package:pip_boi/Settings.dart';
import 'package:pip_boi/Tasklist.dart';

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
            FilledButton(
              onPressed: () {
                Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => Tasklist())
                );
              },
              child: Text('Task list')
            ),

            FilledButton(
              onPressed: () {
                Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => AchievementsTab())
                );
              },
              child: Text('Achievements')
            ),

            FilledButton(
              onPressed: () {
                Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => SettingsTab())
                );
              },
              child: Text('Settings')
            )
          ],
        ),
      ),
    );
  }
}