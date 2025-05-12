import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:async';
import 'package:pip_boi/taskProvider.dart';
import 'package:provider/provider.dart';
import 'package:pip_boi/_task.dart';

/*
class Album { // test class for json mapping
  final int id;
  final String title;

  const Album({required this.id, required this.title});

  factory Album.fromJson(Map<String, dynamic> json) {
    return switch (json) {
      {'id': int id, 'title': String title} => Album(id: id, title: title),
      _ => throw const FormatException('Failed to map json to album.'),
    };
  }

  Map<String, dynamic> toJson() => {'id': id, 'title': title};
}
 */

Future<bool> getStatus() async {
  try {
    final response = await http.get(Uri.parse('http://localhost:3000/status'));
    if (response.statusCode == 200) {
      return true;
    } else {
      print('Failed to connect');
      return false;
    }
  } catch (e) {
    print(e);
    return false;
  }
}

Future<List<TaskOutput>> receiveTasks() async {
  final response = await http.get(Uri.parse('http://localhost:3000/receive'));

  if (response.statusCode == 200) {
    final List<dynamic> data = json.decode(response.body);
    print('Received: $data');
    return data.map((item) => TaskOutput.fromJson(item)).toList();;
  } else {
    print('Failed to connect');
    return [];
  }
}

Future<String> sendTasks(List<Object> inputList) async {

  final response = await http.post(
    Uri.parse('http://localhost:3000/send'),
    headers: <String, String>{
      'Content-Type': 'application/json; charset=UTF-8',
    },
    body: jsonEncode(inputList),
  );

  if (response.statusCode == 200) {
    // If the server did return a 201 CREATED response,
    // then parse the JSON.
    print("response: ${response.body}");
    return "Success";
  } else {
    // If the server did not return a 201 CREATED response,
    // then throw an exception.
    throw Exception('Failed to send tasks to watch');
  }
}


class SettingsTab extends StatefulWidget {
  const SettingsTab({super.key});


  @override
  State<SettingsTab> createState() => _SettingsTabState();
}

class _SettingsTabState extends State<SettingsTab> {
  String responseText = "";

  @override
  Widget build(BuildContext context){

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
                child: Text('Synchronize tasks with device')
            ),
            FilledButton(
              onPressed: () async {
                bool response = await getStatus();
                setState(() {
                  if(response) {
                    responseText = "You are connected to the device";
                  } else {
                    responseText = "No connection to device"
                        "Make sure the device is powered on and on the same wifi";
                  }
                });
                },
              child: Text("Check device connection")
            ),
            Text(responseText),
            FilledButton(
              onPressed: () => sendTasks(tasks),
              child: Text('Send tasks to device')
            ),
            SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}



// import 'package:http/http.dart' as http;
// import 'dart:convert';

