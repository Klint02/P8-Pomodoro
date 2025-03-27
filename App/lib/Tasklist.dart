
import 'package:flutter/material.dart';

class Tasklist extends StatelessWidget {
  const Tasklist({super.key});

  @override
  Widget build(BuildContext context){
    return Scaffold(
      appBar: AppBar(title: const Text('Task list')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.end,
          children: [
            FilledButton(
              onPressed: () {
                Navigator.pop(
                    context
                );
              },
              child: Text('Back')
            ),
            SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}