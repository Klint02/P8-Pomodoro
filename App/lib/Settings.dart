import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

Future<void> getStatus() async {
  final response = await http.get(Uri.parse('http://localhost:3000/status'));

  if (response.statusCode == 200) {
    final data = json.decode(response.body);
    print('Status: $data');
  } else {
    print('Failed to connect');
  }
}

Future<void> sendControlCommand() async {
  final response = await http.post(
    Uri.parse('http://localhost:3000/control'),
    headers: {'Content-Type': 'application/json'},
    body: json.encode({'led': 'off'}),
  );

  if (response.statusCode == 200) {
    print('Command sent successfully');
  } else {
    print('Failed to send command');
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

