import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:async';

class Album {
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

Future<void> getStatus() async {
  final response = await http.get(Uri.parse('http://localhost:3000/status'));

  if (response.statusCode == 200) {
    final data = json.decode(response.body);
    print('Status: $data');
  } else {
    print('Failed to connect');
  }
  //return http.get(Uri.parse('http://localhost:3000/status'));
}

Future<Album> sendControlCommand() async {
  Album createdAlbum = Album(id: 1, title: 'Test Album');
  Album createdAlbum2 = Album(id: 2, title: 'Test Album 2');
  Album createdAlbum3 = Album(id: 3, title: 'Test Album 3');

  List<Album> albums = [createdAlbum, createdAlbum2, createdAlbum3];

  final response = await http.post(
    Uri.parse('http://localhost:3000/control'),
    headers: <String, String>{
      'Content-Type': 'application/json; charset=UTF-8',
    },
    body: jsonEncode(albums), // albums.map((album) => album.toJson()).toList()
  );

  if (response.statusCode == 200) {
    // If the server did return a 201 CREATED response,
    // then parse the JSON.
    print("response: ${response.body}");
    return Album.fromJson(jsonDecode(response.body) as Map<String, dynamic>);
  } else {
    // If the server did not return a 201 CREATED response,
    // then throw an exception.
    throw Exception('Failed to create album.');
  }
}


class SettingsTab extends StatelessWidget {
  const SettingsTab({super.key});

  @override
  Widget build(BuildContext context){
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
              onPressed: () => getStatus(),
              child: Text('get function')
            ),
            FilledButton(
              onPressed: () => sendControlCommand(),
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

