import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:pip_boi/taskProvider.dart';
import 'package:provider/provider.dart';
import 'package:pip_boi/_task.dart';
import 'package:pip_boi/deviceService.dart';
import 'dart:ui' as ui;

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
    var taskProvider = Provider.of<TaskProvider>(context);
    var tasks = taskProvider.convertToOutput();

    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: Column(
        children: [
          Expanded(
            child: Center(
              child: SizedBox(
                width: 200,
                height: 200,
                child: FilledButton(
                  onPressed: () async {
                    var responseList = await receiveTasks();
                    taskProvider.updateFromInput(responseList);

                    bool response = await sendTasks(tasks);
                    setState(() {
                      sendResponseText = response
                          ? "Successfully synchronized tasks with device"
                          : "Error sending tasks, check connection with device";
                    });
                  },
                  style: FilledButton.styleFrom(
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(24),
                    ),
                  ),
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      const Text(
                        'Synchronize Tasks',
                        textAlign: TextAlign.center,
                      ),
                      const SizedBox(height: 8),
                      Text(
                        sendResponseText,
                        textAlign: TextAlign.center,
                        style: const TextStyle(fontSize: 12),
                      ),
                    ],
                  ),
                ),
              ),
            ),
          ),
          Expanded(
            child: Center(
              child: SizedBox(
                width: 200,
                height: 200,
                child: FilledButton(
                  onPressed: () async {
                    bool response = await getStatus();
                    setState(() {
                      statusResponseText = response
                          ? "You are connected to the device"
                          : "No connection to device. Make sure:\n"
                          "- Device is powered on\n"
                          "- Phone is on same Wi-Fi";
                    });
                  },
                  style: FilledButton.styleFrom(
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(24),
                    ),
                  ),
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      const Text(
                        'Check Connection',
                        textAlign: TextAlign.center,
                      ),
                      const SizedBox(height: 8),
                      Text(
                        statusResponseText,
                        textAlign: TextAlign.center,
                        style: const TextStyle(fontSize: 12),
                      ),
                    ],
                  ),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

}

// import 'package:http/http.dart' as http;
// import 'dart:convert';
