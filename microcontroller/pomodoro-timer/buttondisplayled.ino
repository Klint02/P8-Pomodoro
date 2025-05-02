#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pomodoro.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// PINS AND STATE DEFINITIONS
const int buttonPin = D5; // Pin for the button
bool buttonState = HIGH; // Button state is set to HIGH by default
bool lastButtonState = HIGH; // Button state is set to HIGH by default

// Time variables
unsigned long pressTime = 0; // Time when the button was pressed
unsigned long releaseTime = 0; // Time when the button was released
unsigned long lastClickTime = 0; // Time of the last click
unsigned long lastToggleTime = 0; // Time of the last LED toggle (for blinking)
unsigned long currentTime = 0; // Current time in milliseconds
unsigned long lastTimerUpdateTime = 0; // Last time the timer was updated
int timerMinutes = 25; // Timer duration in minutes
int timerSeconds = 0; // Timer duration in seconds

// Indicators for button state
bool longPressHandled = false; // Indicates if a long press has been handled
bool singlePressPending = false; // Indicates if a single press is pending

// LED STATE, MODE and TIMER RUNNING STATE
bool ledState = true; // LED state (true = ON, false = OFF)
bool timerRunning = false; // Timer running state (true = running, false = not running)
// Mode of operation
// 0 = normal mode, 1 == fast blink, 2 == slow blink - default is normal mode
int mode = 0;

// TIMING CONSTANTS
const unsigned long debounceDelay = 50;
const unsigned long longPressThreshold = 800;    // ms
const unsigned long doubleClickThreshold = 300;  // ms

// Function to setup the button and LED
// and initialize the serial communication
// and set the LED to LOW WHICH INDICATES LED IS TURNED ON (BECAUSE OF ACTIVE LOW)
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }


  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  pinMode(BUILTIN_LED, OUTPUT);  // initialize onboard LED as output
  

  // Startup procedure
  showPomodoroStartScreen(); // Show the pomodoro image on the screen
  delay(2000); // Wait for 2 seconds
  digitalWrite(BUILTIN_LED, LOW); // Turn on the LED (active low)
  delay(2000);
}

// Main loop function
// It reads the button state, handles long press, single press, and updates LED mode


void loop() {
//    char buffer[32];
//    sprintf(buffer, "Now in mode: %d", mode);
//    showTextScreen(buffer);
    // this sequence of functions register the three different button interactions
    updateButtonState();
    handleLongPress();
    handleButtonRelease();
    handleSinglePressTimeout();
    // this function changes screen or triggers timer depending on button presses
    //updateLEDMode();
    updateTimer();
    updateScreenMode();
    finalizeButtonState();
    delay(10); // Small delay to avoid excessive CPU usage
}


void updateButtonState() {
    buttonState = digitalRead(buttonPin);
    currentTime = millis();
    
    if (lastButtonState == HIGH && buttonState == LOW) {
      pressTime = currentTime;
      longPressHandled = false;
    }
}

void handleLongPress() {
    if (buttonState == LOW && !longPressHandled && (currentTime - pressTime > longPressThreshold)) {
        Serial.println("Long press detected");
        if (timerRunning) {
            Serial.println("Stopping Pomodoro Timer");
            timerRunning = false; // Set timer running state to false
            showCenteredMessage("Timer stopped"); // Show message if timer is stopped
            delay(2000); // Wait for 2 seconds before showing the next screen
            mode = 0; // Switch back to normal mode
        }
        longPressHandled = true; // Mark long press as handled
    }
}

void handleButtonRelease() {
    if (buttonState == HIGH && lastButtonState == LOW) {
        releaseTime = currentTime;
        if (!longPressHandled) {
          if (releaseTime - lastClickTime < doubleClickThreshold) {
            Serial.println("Double press detected");
            mode = (mode +1) % 2; // Cycle through modes 0, 1, 2
            singlePressPending = false;
            lastClickTime = 0;
          } else {
            singlePressPending = true; // Set single press pending
            lastClickTime = currentTime; // Update last click time
          }
        }
    }
}

void handleSinglePressTimeout() {
    if (singlePressPending && (currentTime - lastClickTime > doubleClickThreshold)) {
        if (buttonState == HIGH) { // Only consider it a single press if the button is released
            Serial.println("Single press detected");

            if (mode == 1 && !timerRunning) {
                timerMinutes = 25; // Reset timer to 25 minutes
                timerSeconds = 0; // Reset seconds to 0
                lastTimerUpdateTime = millis(); // Reset last timer update time
                timerRunning = true; // Set timer running state to true
                Serial.println("Staring Pomodoro Timer");
            }

            singlePressPending = false; // Reset single press pending
            lastClickTime = 0;
        }
    }
}

/*
void updateLEDMode() {
    if (mode == 1) { // Fast blink mode
        if (currentTime - lastToggleTime >= 1000) { // Blink every 100 ms
            ledState = !ledState; // Toggle LED state
            digitalWrite(BUILTIN_LED, ledState ? LOW : HIGH); // Set LED state
            lastToggleTime = currentTime; // Update last toggle time
        }
    } else if (mode == 2) { // Slow blink mode
        if (currentTime - lastToggleTime >= 3000) { // Blink every 500 ms
            ledState = !ledState; // Toggle LED state
            digitalWrite(BUILTIN_LED, ledState ? LOW : HIGH); // Set LED state
            lastToggleTime = currentTime; // Update last toggle time
        }
    } else { // Normal mode
        digitalWrite(BUILTIN_LED, ledState ? LOW : HIGH); // Set LED state based on ledState
    }
}
*/

void pomodoroTimer() {
    int pomodoroDuration = 25 * 60; // 25 minutes in seconds
    int breakDuration = 5 * 60; // 5 minutes in seconds
    int longBreakDuration = 15 * 60; // 15 minutes in seconds
    int cycles = 4; // Number of pomodoro cycles before a long break
    int pomodoroCount = 0; // Counter for completed pomodoros
    int minutes = 25;
    int seconds = 0; // Initialize seconds to 0

    while ((minutes != 0 || seconds != 0) && timerRunning) {
        delay(1000); // Wait for 1 second
        showTimerScreen(minutes, seconds); // Update the timer screen

        if (seconds <= 0) {
            if (minutes > 0) {
                minutes--;
                seconds = 59; // Reset seconds to 59
            } else {
                break; // Exit the loop when time is up
            }
        } else {
            seconds--;
        }
    }

        if (!timerRunning) {
            showCenteredMessage("Timer stopped"); // Show message if timer is stopped
        } else {
            showCenteredMessage("Pomodoro finished!"); // Show message when timer is finished
        }

        delay(2000); // Wait for 2 seconds before showing the next screen
        timerRunning = false; // Set timer running state to false
        mode = 0; // Switch back to normal mode
        pomodoroCount++; // Increment the pomodoro count
        if (pomodoroCount >= cycles) {
            pomodoroCount = 0; // Reset the pomodoro count
            minutes = longBreakDuration / 60; // Set to long break duration
        } else {
            minutes = breakDuration / 60; // Set to short break duration
        }
        seconds = 0; // Reset seconds to 0
    }


void updateTimer() {
    if (timerRunning && millis() - lastTimerUpdateTime >= 1000) {
        lastTimerUpdateTime = millis(); 
        if (timerSeconds == 0) {
            if (timerMinutes == 0) {
                timerRunning = false; // Stop the timer when it reaches 0
                showCenteredMessage("Pomodoro done!");
                delay(2000); // Wait for 2 seconds
                mode = 0; // Switch back to normal mode
                return;
            } else {
                timerMinutes--;
                timerSeconds = 59; // Reset seconds to 59
        } } else {
            timerSeconds--;
        }
        showTimerScreen(timerMinutes, timerSeconds); // Update the timer screen
    }
}

void updateScreenMode() {
    if (timerRunning) return;
    switch (mode) {
        case 0: // Normal mode
            showTextScreen("Task info or choose task screen");
            break;
        case 1: // Fast blink mode
            showPomodoroTimerScreen();
        break;
        default:
            showTextScreen("Unknown mode");
            break;
    }
}

void finalizeButtonState() {
    lastButtonState = buttonState;
    delay(debounceDelay); // Debounce delay
}

void showIntroScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println("Welcome to you PomodoroTimer");
  display.display();
}

void showTextScreen(const char* msg) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(msg);
  display.display();
}

void showPomodoroStartScreen() {
  display.clearDisplay();
  display.drawBitmap(0, 0, pomodoropic, 128, 64, WHITE);
  display.display();
}

void showPomodoroTimerScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Pomodoro Timer");
  display.display();
}

void showTimerScreen(int min, int sec) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.print("Pomodoro: ");

  display.setTextSize(2);
  display.setCursor(0, 20);

  char buf[6];
  sprintf(buf, "%02d:%02d", min, sec);
  display.print(buf);
  display.display();
}

void showMessage(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

void showCenteredMessage(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  int16_t x, y;
  uint16_t w, h;
  display.getTextBounds(message, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);

  display.println(message);
  display.display();
}
