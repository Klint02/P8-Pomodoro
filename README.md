# P8-PomodoroBoy - Firmware and App code

This repository contains the firmware for PomodoroBoy, a tangible Pomodoro timer prototype designed to help users stay focused by physically separating the timer functionality from digital distractions like smartphones or computers.

The sketch runs on a Wemos D1 Mini and the following controls:

- A button for user interaction (single press, long press)
- An OLED display (SSD1306 I2C, 128x64 pixels) for status updates
- An RTC module (DS1307) to keep track of real-world time
- A battery-powered supply for portability

## Wiring

| Component      | Pin |
|----------------|-----|
| Button         |  D5 |
| OLED SDA       |  D2 |
| OLED SCL       |  D1 |
| OLED VCC       | 3V3 |
| OLED GND       |   G |
| RTC RST        |  D8 |
| RTC DAT        |  D7 |
| RTC CLK        |  D6 |
| RTC GND        |  G  |
| RTC VCC        | 3V3 |
| Battery Pack + |  5V |
| Battery Pack - | GND |

## Dependencies 

- Adafruit GFX Library
- Adafruit SSD1306 Library
- ESP8266WebServer Library
- ThreeWire Library
- RtcDS1302 Library

## Setup and Installation
### Pomodoro setup
1. Connect the hardware as per the wiring indicated above (or using the wirering diagram from the paper)
2. Install the required libraries in your Arduino IDE
3. Open the sketch in Arduino IDE and select the correct board (Wemos D1 Mini) and port.
4. Upload the sketch to the board.

### App setup
compilation commands:
android: flutter build apk --split-per-abi
Windows: flutter build windows

step by step
1. Open the `App` folder and use either the windows or android compilation command  
2. navigate to the output folder specified by flutter itself and run open the app

There are precompiled apk's and binaries given with the release, so you can skip compilation of the app

**NOTE**: If there is a problem connecting to the Pomodoro using the an phone, then use the windows application
