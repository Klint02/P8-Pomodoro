#include "Arduino.h"
#include "Speaker.hpp"

namespace speaker {

    void generateTone(int compilation) {
        pinMode(buzzer, OUTPUT);

        switch (compilation) {
            case congratulations:
                tone(buzzer, 1046, 200); // C6
                delay(200);
                tone(buzzer, 987, 200); // B5
                delay(200);
                tone(buzzer, 880, 200); // A5
                delay(200);
                tone(buzzer, 784, 400); // G5
                delay(400);
                tone(buzzer, 698, 200); // F5
                delay(200);
                tone(buzzer, 659, 200); // E5
                delay(200);
                tone(buzzer, 587, 600); // D5
                delay(600);
            break;
            case tada:
                tone(buzzer, 1046, 200); // C6
                delay(200);
                tone(buzzer, 987, 200); // B5
                delay(200);
                tone(buzzer, 880, 200); // A5
                delay(200);
                tone(buzzer, 784, 400); // G5
                delay(400);
                tone(buzzer, 698, 200); // F5
                delay(200);
                tone(buzzer, 659, 200); // E5
                delay(200);
                tone(buzzer, 587, 600); // D5
                delay(600);
            break;
            case woopwoop:
                tone(buzzer, 1318, 100); // C6
                delay(100);
                tone(buzzer, 1175, 100); // B5
                delay(100);
                tone(buzzer, 1046, 100); // C6
                delay(100);
                tone(buzzer, 987, 200); // B5
                delay(200);
                tone(buzzer, 880, 200); // A5
                delay(200);
                tone(buzzer, 784, 400); // G5
                delay(400);
                tone(buzzer, 698, 200); // F5
                delay(200);
                tone(buzzer, 659, 200); // E5
                delay(200);
                tone(buzzer, 587, 600); // D5
                delay(600);
            break;
        }
    }
}