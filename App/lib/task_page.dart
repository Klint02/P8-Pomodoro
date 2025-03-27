import 'package:flutter/material.dart';

class TaskPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Center(
      child: ElevatedButton(onPressed: () {
        Navigator.pop(context);
      }, child: Text("Back")),
    );
  }
}