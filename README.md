# P8-Pomodoro Timer - Microcontroller code

This sketch runs on a Wemos D1 Mini and the following controls:

- A button (single, long and double press)
- An OLED display (SSD1306 I2C) - 128x64 pixels
- An onboard LED for visual feedback

## Wiring

| Component | Pin |
|-----------|-----|
| Button    |  D5 |
| OLED SDA  |  D2 |
| OLED SCL  |  D1 |
| OLED VCC  | 3V3 |
| OLED GND  |  G  |

## Dependencies 

- Adafruit GFX Library
- Adafruit SSD1306 Library