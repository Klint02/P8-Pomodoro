#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pomodoro.h"
#include <vector>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// CONSTANTS RELATED TO DISPLAY
const int CHAR_WIDTH = 12; // approximate width for textsize 2
const int MAX_SCROLL_WIDTH = 124; // max scroll width for textsize 2
int scrollResetX;
int scrollX = 0; // X position for scrolling text

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// PINS AND STATE DEFINITIONS
const int buttonPin = D5; // Pin for the button
bool buttonState = HIGH; // Button state is set to HIGH by default
bool lastButtonState = HIGH; // Button state is set to HIGH by default

// Time variables
unsigned long pressTime = 0; // Time when the button was pressed
unsigned long releaseTime = 0; // Time when the button was released

unsigned long currentTime = 0; // Current time in milliseconds
unsigned long lastTimerUpdateTime = 0; // Last time the timer was updated
unsigned long secSinceStart = 0; // Seconds since the timer started
int timerMinutes = 1; // Timer duration in minutes
int timerSeconds = 0; // Timer duration in seconds

// Indicators for button state
bool longPressHandled = false; // Indicates if a long press has been handled

// LED STATE, MODE and TIMER RUNNING STATE
//bool ledState = true; // LED state (true = ON, false = OFF)
bool timerRunning = false; // Timer running state (true = running, false = not running)
// Mode of operation
int mode = 0;
int selectedIcon = 0; // 0 = left, 1 = center, 2 = right

// TIMING CONSTANTS
const unsigned long debounceDelay = 50;
const unsigned long longPressThreshold = 800;    // ms

// NETWORK CONSTANTS
const char* ssid = "My Super Network"; // WiFi SSID
const char* ip = "192.168.0.1"; // Server address

// Enum for menu selection 
enum MenuSelection {
    MENU_POMODORO,
    MENU_TROPHY,
    MENU_SETTINGS,
    NUM_MENU_ITEMS
};

// Enum for SCREEN STATES
enum AppState {
    INTRO_SCREEN,
    MENU_SCREEN,
    TASK_SCREEN,
    TIMER_RUNNING_SCREEN,
    TASK_DONE,
    SYNC_SCREEN,
    TROPHY_SCREEN
};

// String constants for tasks and trophies
std::vector<String> tasks = {
  "Buy birthday present", 
  "Cleaning", 
  "Pick up kids", 
  "Groceries"};
int taskNum = 0;
String question = "Click the button to start task";
int totalTrophies = 10;
int dailyTasksCompleted = 0;


AppState currentState = INTRO_SCREEN; // Initialize the current state to MENU_SCREEN

// Function to setup the button and display
// and initialize the serial communication

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }

  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up resistor

  // Startup procedure
  showPomodoroStartScreen(); // Show the pomodoro image on the screen
  delay(2000); // Wait for 2 seconds
  currentState = MENU_SCREEN; // Set the current state to MENU_SCREEN
  showMenuScreen(); // Show the menu screen  
}

// Main loop function
// It reads the button state, handles long press, single press, and updates timer and screen

void loop() {
    updateButtonState(); // Detect press start
    handleLongPress();   // Trigger long press action
    handleButtonRelease(); // Detect and handle short press
    updateTimer(); // Update the timer if running
    updateScreenMode(); // Update the screen based on the current mode
    finalizeButtonState(); // Debounce cleanup
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

        switch (currentState) {
            case MENU_SCREEN:
                selectedIcon = static_cast<MenuSelection>((selectedIcon + 1) % NUM_MENU_ITEMS); // Cycle through menu items
                Serial.print("Selected menu item: ");
                Serial.println(selectedIcon);
                break;
            case TASK_SCREEN:
            // Return to menu screen
                Serial.println("Returning to menu screen");
                currentState = MENU_SCREEN;
                break;
            case TIMER_RUNNING_SCREEN:
                Serial.println("Stopping Pomodoro Timer");
                timerRunning = false; // Set timer running state to false
                showCenteredMessage("Timer stopped"); // Show message if timer is stopped
                delay(2000); // Wait for 2 seconds before showing the next screen
                currentState = TASK_SCREEN; // Switch back to normal mode
                break;
            case TASK_DONE:
                Serial.println("Returning to task selection");
                taskNum = 0; // Reset task number
                currentState = MENU_SCREEN; // Switch back to menu screen
                break;
            case TROPHY_SCREEN:
                Serial.println("Returning to menu screen");
                currentState = MENU_SCREEN; // Switch back to menu screen
                break;
            case SYNC_SCREEN:
                Serial.println("Returning to menu screen");
                currentState = MENU_SCREEN; // Switch back to menu screen
                break;
        }
        longPressHandled = true; // Mark long press as handled
    }
}

void handleButtonRelease() {
    if (buttonState == HIGH && lastButtonState == LOW) {
        releaseTime = currentTime;
        if (!longPressHandled) {
            Serial.println("Single press detected");
            handleSinglePress();
          }
    }
}

void handleSinglePress() {
    Serial.println("Single press detected");

    switch (currentState) {

        case (MENU_SCREEN): {

            switch (selectedIcon) {
                case MENU_POMODORO:
                    Serial.println("Entering task screen");
                    prepareTaskScreen(); // Prepare the task screen
                    currentState = TASK_SCREEN; // Switch to task screen
                    break;
                case MENU_TROPHY:
                    Serial.println("Entering trophy screen");
                    currentState = TROPHY_SCREEN; // Switch to trophy screen
                    break;
                case MENU_SETTINGS:
                    Serial.println("Entering settings screen");
                    currentState = SYNC_SCREEN; // Switch to settings screen
                    break;
            }
            break;
        }
        case (TASK_SCREEN):

            if (timerRunning) {
                Serial.println("Timer is already running");
                showCenteredMessage("Switching to timer running screen");
                delay(2000); // Wait for 2 seconds
                currentState = TIMER_RUNNING_SCREEN; // Switch to timer running state
            } else {
                Serial.println("Starting Pomodoro timer");
                prepareTaskScreen(); // Prepare the task screen
                showCenteredMessage("Pomodoro timer started");
                timerMinutes = 1; // Reset timer to 25 minutes
                timerSeconds = 0; // Reset seconds to 0
                lastTimerUpdateTime = millis(); // Reset last timer update time
                timerRunning = true; // Start the timer
                currentState = TIMER_RUNNING_SCREEN; // Switch to timer running state
                Serial.println("Pomodoro timer started");
            }
            break;

        case (TIMER_RUNNING_SCREEN): 
            Serial.println("Timer running screen");
            if (timerRunning) {
                timerRunning = false; // Stop the timer
                showCenteredMessage("Pomodoro timer stopped");
                delay(2000); // Wait for 2 seconds
                startNextTask(); // Start the next task
            } else {
                Serial.println("Timer is not running");
                showCenteredMessage("Pomodoro timer not running");
                delay(2000); // Wait for 2 seconds
            }
            break;

        case (TASK_DONE): 
            Serial.println("All tasks done");
            showCenteredMessage("All tasks done");
            delay(2000); // Wait for 2 seconds
            currentState = MENU_SCREEN; // Switch back to menu screen
            break;
        
        case (SYNC_SCREEN): 
            Serial.println("Syncing tasks...");
            showCenteredMessage("Syncing tasks...");
            delay(2000); // Wait for 2 seconds
            currentState = MENU_SCREEN; // Switch back to menu screen
            break;
        
        case (TROPHY_SCREEN): 
            Serial.println("Trophy screen");
            showCenteredMessage("Trophy unlocked!");
            delay(2000); // Wait for 2 seconds
            currentState = MENU_SCREEN; // Switch back to menu screen
            break;
        
    } 
}


void startNextTask() {
    taskNum++;
    if (taskNum >= tasks.size()) {
        taskNum = tasks.size();
        currentState = TASK_DONE;
        return;
    }
    // Reset scroll position and timer
    scrollResetX = -CHAR_WIDTH * tasks[taskNum].length();
    scrollX = SCREEN_WIDTH;
    timerMinutes = 1;
    timerSeconds = 0;
    secSinceStart = 0;
    currentState = TASK_SCREEN;
}

int calcBarPct() {
    return round((float)secSinceStart / 1500 * 124);  // 1500 = 25*60 seconds
}

void prepareTaskScreen() {
    if (taskNum < tasks.size()) {
      int textWidth = CHAR_WIDTH * tasks[taskNum].length();
      scrollResetX = -textWidth;

      if (textWidth <= MAX_SCROLL_WIDTH) {
        scrollX = 0;
      } else {
        scrollX = SCREEN_WIDTH;
      }
    } else {
        scrollResetX = 0; // Reset scroll position
        scrollX = 0; // Reset scroll position
    }
}

void updateTimer() {
    if (timerRunning && millis() - lastTimerUpdateTime >= 1000) {
        lastTimerUpdateTime = millis();
        secSinceStart++;
        if (timerSeconds == 0) {
            if (timerMinutes == 0) {
                timerRunning = false; // Stop the timer when it reaches 0
                showCenteredMessage("Pomodoro done!");
                delay(2000); // Wait for 2 seconds
                dailyTasksCompleted++;
                totalTrophies++;
                currentState = TASK_SCREEN; // Switch back to task screen
                taskNum++;
                mode = 0; // Switch back to normal mode
                return;
            } else {
                timerMinutes--;
                timerSeconds = 59; // Reset seconds to 59
        } } else {
            timerSeconds--;
        }
        showTimerScreen(tasks[taskNum], timerMinutes, timerSeconds); // Update the timer screen
    }
}

void updateScreenMode() {
    if (timerRunning) return;
    switch (currentState) {
        case MENU_SCREEN: // Show menu screen
            showMenuScreen();
            break;
        case TASK_SCREEN: // Show task screen
            updateScrollPosition(tasks[taskNum]);
            showCurrentTask(tasks[taskNum]);
            break;
        case TIMER_RUNNING_SCREEN: // Show timer running screen
            showPomodoroTimerScreen();
            break;
        case TASK_DONE: // Show task done screen
            showAllTasksDone();
            break;
        case SYNC_SCREEN: // Show sync screen
            showSyncScreen();
            break;
        case TROPHY_SCREEN: // Show trophy screen
            showTrophyScreen();
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

void showTrophyScreen() {
    display.clearDisplay();

    // Display total trophy count as number
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(5, 10);
    display.println(totalTrophies);

    // Printing the number of tasks completed today out of all daily tasks

    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Tasks completed today: ");
    display.setCursor(0, 56);
    display.print(dailyTasksCompleted);
    display.print("/");
    display.print(tasks.size());


    // Draw trophy icon
    display.drawBitmap(90, 10, trophy, 32, 32, WHITE);

    display.display();
}

void showSyncScreen() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("SSID: ");
    display.print(ssid);
    display.setCursor(0, 30);
    display.println("IP: ");
    display.print(ip);
    display.setCursor(0, 50);
    display.println("Syncing...");
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
  int x = (SCREEN_WIDTH - 64) / 2;  // 32
  int y = (SCREEN_HEIGHT - 64) / 2; // 0
  display.drawBitmap(x, y, pomodoropic, 64, 64, WHITE);
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

void showTimerScreen(const String& taskName, int min, int sec) {
  display.clearDisplay();

  // Task name displayed at the top
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.print(taskName);

  // Timer in large font
  display.setTextSize(2);
  display.setCursor(11, 32);

  char buf[6];
  sprintf(buf, "%02d:%02d", min, sec);
  display.print(buf);

  // Draw hourglass icon
  display.drawBitmap(82, 20, hourglass, 30, 40, WHITE);
  
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

void showMenuScreen() {
    display.clearDisplay();
    display.drawBitmap(0, 0, menupic, 128, 42, WHITE);
    display.fillRect(0, 43, 128, 5, BLACK);

    // Draw selection line
    int iconWidth = 42;
    int xStart = selectedIcon * iconWidth + 5; 
    int xEnd = xStart + iconWidth - 10;
    int y = 47;

    display.drawLine(xStart, y, xEnd, y, WHITE);
    display.display();
}

void showAllTasksDone(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print("Great job!");
    display.setCursor(0,30);
    display.setTextWrap(true);
    display.print("All tasks complete");
    display.display();
    display.setTextWrap(false);
  }

void updateScrollPosition(const String& text) {
    if (CHAR_WIDTH * text.length() > MAX_SCROLL_WIDTH) {
        if (scrollX < scrollResetX) {
            scrollX = SCREEN_WIDTH;
        } else {
            scrollX--;
        } 
    } else {
        scrollX = 0; // Reset scroll position if text fits
    }
}   


void showCurrentTask(const String& taskText) {
    display.clearDisplay();

    // Label
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Next task:");

    // Task name, scrolling if needed
    display.setTextSize(2);
    display.setCursor(scrollX, 20);  // Better vertical alignment
    display.print(taskText);

    // Footer
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.print("Press to start task");

    display.display();
}