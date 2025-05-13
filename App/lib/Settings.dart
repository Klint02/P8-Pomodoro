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
  String statusResponseText = "";
  String sendResponseText = "";

  @override
  Widget build(BuildContext context) {
    List<TaskOutput> responseList = [];
    var taskProvider = Provider.of<TaskProvider>(context);
    var tasks = taskProvider.convertToOutput();

    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            Column(
              children: [
                FilledButton(
                    onPressed: () async {
                      responseList = await receiveTasks();
                      taskProvider.updateFromInput(responseList);

                      bool response = await sendTasks(tasks);
                      setState(() {
                        if (response) {
                          sendResponseText =
                          "Tasks sent to device";
                        } else {
                          sendResponseText =
                          "Error sending tasks, check connection with device";
                        }
                      });
                    },
                    child: Text('Synchronize tasks with device')),
                Text(sendResponseText),
              ],
            ),
            Column(
              children: [
                FilledButton(
                    onPressed: () async {
                      bool response = await getStatus();
                      setState(() {
                        if (response) {
                          statusResponseText =
                              "You are connected to the device";
                        } else {
                          statusResponseText =
                              "No connection to device, ensure the following: \n"
                              " - The device is powered on \n"
                              " - The phone is on the same wifi as the device";
                        }
                      });
                    },
                    child: Text("Check device connection")),
                Text(statusResponseText),
              ],
            ),
          ],
        ),
      ),
    );
  }
}

// import 'package:http/http.dart' as http;
// import 'dart:convert';
