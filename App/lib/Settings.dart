import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:pip_boi/taskProvider.dart';
import 'package:provider/provider.dart';
import 'package:pip_boi/_task.dart';
import 'package:pip_boi/deviceService.dart';

class SettingsTab extends StatefulWidget {
  const SettingsTab({super.key});

  @override
  State<SettingsTab> createState() => _SettingsTabState();
}

class _SettingsTabState extends State<SettingsTab> {
  String responseText = "";

  @override
  Widget build(BuildContext context) {
    List<TaskOutput> responseList = [];
    var taskProvider = Provider.of<TaskProvider>(context);
    var tasks = taskProvider.convertToOutput();

    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            FilledButton(
                onPressed: () async {
                  responseList = await receiveTasks();
                  taskProvider.updateFromInput(responseList);
                },
                child: Text('Synchronize tasks with device')),
            FilledButton(
                onPressed: () async {
                  bool response = await getStatus();
                  setState(() {
                    if (response) {
                      responseText = "You are connected to the device";
                    } else {
                      responseText = "No connection to device"
                          "Make sure the device is powered on and on the same wifi";
                    }
                  });
                },
                child: Text("Check device connection")),
            Text(responseText),
            FilledButton(
                onPressed: () => sendTasks(tasks),
                child: Text('Send tasks to device')),
            SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}

// import 'package:http/http.dart' as http;
// import 'dart:convert';
