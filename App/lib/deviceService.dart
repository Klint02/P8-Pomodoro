import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:async';
import 'package:pip_boi/_task.dart';

Future<bool> getStatus({Duration timeout = const Duration(seconds: 10)}) async {
  const url = 'http://localhost:3000/status';
  try {
    final response = await http.get(Uri.parse(url)).timeout(timeout);

    return response.statusCode == 200;
  } on TimeoutException {
    print('Request to get status timed out.');
  } catch (e) {
    print("Error: $e");
  }

  return false;
}

Future<List<TaskOutput>> receiveTasks(
    {Duration timeout = const Duration(seconds: 10)}) async {
  const url = 'http://localhost:3000/receive';

  try {
    final response = await http.get(Uri.parse(url)).timeout(timeout);

    if (response.statusCode == 200) {
      final List<dynamic> data = json.decode(response.body);
      print('Received: $data');
      return data.map((item) => TaskOutput.fromJson(item)).toList();
    } else {
      print('Failed to receive tasks');
      return [];
    }
  } on TimeoutException {
    print('Request to receive timed out.');
  } catch (e) {
    print("Error: $e");
  }
  return [];
}

Future<bool> sendTasks(List<Object> inputList,
    {Duration timeout = const Duration(seconds: 10)}) async {
  const url = 'http://localhost:3000/send';

  try {
    final response = await http.post(
      Uri.parse(url),
      headers: <String, String>{
        'Content-Type': 'application/json; charset=UTF-8',
      },
      body: jsonEncode(inputList),
    ).timeout(timeout);

    return response.statusCode == 200;
  } on TimeoutException {
    print('Request to receive timed out.');
  } catch (e) {
    print("Error: $e");
  }

  return false;
}