
import 'package:flutter/material.dart';
import 'package:pip_boi/taskProvider.dart';
import 'package:provider/provider.dart';

class SettingsTab extends StatelessWidget {
  const SettingsTab({super.key});



  @override
  Widget build(BuildContext context){
    var taskProvider = Provider.of<TaskProvider>(context);
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            FilledButton(
                onPressed: () {taskProvider.convertToOutput();},
                child: Text('Connect to device')
            ),
            SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}