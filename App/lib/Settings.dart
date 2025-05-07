import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:async';
import 'package:pip_boi/taskProvider.dart';
import 'package:provider/provider.dart';

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

Future<String> getStatus() async {
  final response = await http.get(Uri.parse('http://localhost:3000/status'));

  if (response.statusCode == 200) {
    final data = json.decode(response.body);
    print('Status: $data');
    return "Success";
  } else {
    print('Failed to connect');
    return "Fail";
  }
}

Future<String> sendControlCommand(List<Object> inputList) async {
  // Album createdAlbum = Album(id: 1, title: 'Test Album');
  // Album createdAlbum2 = Album(id: 2, title: 'Test Album 2');
  // Album createdAlbum3 = Album(id: 3, title: 'Test Album 3');

  // List<Album> albums = [createdAlbum, createdAlbum2, createdAlbum3];

  

  final response = await http.post(
    Uri.parse('http://localhost:3000/control'),
    headers: <String, String>{
      'Content-Type': 'application/json; charset=UTF-8',
    },
    body: jsonEncode(inputList), // albums.map((album) => album.toJson()).toList()
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
  String text = "get function";

  @override
  Widget build(BuildContext context){

    var taskProvider = Provider.of<TaskProvider>(context);
    var tasks = taskProvider.convertToOutput();
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            FilledButton(
                onPressed: () {},
                child: Text('Connect to device')
            ),
            FilledButton(
              onPressed: () async {
                String text2 = await getStatus();
                setState(() {
                  text = text2;
                });
                },
              child: Text(text)
            ),
            FilledButton(
              onPressed: () => sendControlCommand(tasks),
              child: Text('post function')
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

