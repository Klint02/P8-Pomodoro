#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <iostream>
#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "src/bitmaps/bitmaps.h"
#include "src/filesystem/filesystem.hpp"
#include "src/RTCService/RTCService.hpp"
#include "src/LoggingService/LoggingService.hpp"
#include "src/NetworkService/NetworkService.hpp"
#include "src/TaskControlService/TaskControlService.hpp"

auto RTC_service = RTC::RTCService();
auto logging_service = logging::LoggingService(RTC_service);
auto central_logger = new logging::LoggingWrapper("Central Service", logging_service);
auto task_control_service = TCS::TaskControlService();
auto network_service = new network::networkService(logging_service, RTC_service);
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
int timerMinutes = task_control_service.tasks[task_control_service.getIndex()].timers[0]; // Timer duration in minutes
int timerSeconds = 0; // Timer duration in seconds

// Indicators for button state
bool longPressHandled = false; // Indicates if a long press has been handled
bool singlePressHandled = false; // Indicates if a single press has been handled

// LED STATE, MODE and TIMER RUNNING STATE
//bool ledState = true; // LED state (true = ON, false = OFF)
bool timerRunning = false; // Timer running state (true = running, false = not running)
// Mode of operation
int mode = 0;
int selectedIcon = 0; // 0 = left, 1 = center, 2 = right

// TIMING CONSTANTS
const unsigned long debounceDelay = 75;
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
    TROPHY_SCREEN,
    TASK_COMPLETE_QUERY_SCREEN,
    INTERRUPT_QUERY_SCREEN
};

enum InterruptChoice {
    INTERRUPT_PAUSE,
    INTERRUPT_DONE,
    INTERRUPT_RESUME,
    NUM_INTERRUPT_CHOICES
};

enum TaskCompleteQuery {
    TASK_COMPLETE,
    TASK_EXTEND,
    NUM_TASK_COMPLETE_CHOICES
};

TaskCompleteQuery taskCompleteQuery = TASK_COMPLETE; // Default choice for task completion
InterruptChoice interruptChoice = INTERRUPT_PAUSE; // Default choice for interrupt handling

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
    Wire.begin(D3,D4);
    Serial.begin(115200);
    std::cout << std::endl;
    central_logger->log("PomoOS starting up", logging::levels::INFO);
    network_service->initWiFi();
    network_service->initRest();
    delay(1000);

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
    central_logger->log("PomoOS is now running", logging::levels::INFO);

}

// Main loop function
// It reads the button state, handles long press, single press, and updates timer and screen

void loop() {
    network_service->handleRestClient();

    currentTime = millis();
    buttonState = digitalRead(buttonPin);

    updateButtonState(); // Detect press start
    handleLongPress();   // Trigger long press action
    handleButtonRelease(); // Detect and handle short press
    updateTimer(); // Update the timer if running
    updateScreenMode(); // Update the screen based on the current mode
    finalizeButtonState(); // Debounce cleanup
    delay(10); // Small delay to avoid excessive CPU usage
}

    
void updateButtonState() {
    
    if (lastButtonState == HIGH && buttonState == LOW) {
      pressTime = currentTime;
      longPressHandled = false;
      singlePressHandled = false;
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
                Serial.println("Pomodoro Interrupted");
                timerRunning = false; // Set timer running state to false
                currentState = INTERRUPT_QUERY_SCREEN; // Switch to interrupt query screen
                interruptChoice = INTERRUPT_PAUSE; // Reset interrupt choice
                break;
            case TASK_DONE:
                Serial.println("Returning to task selection");
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
            case TASK_COMPLETE_QUERY_SCREEN:
                if (taskCompleteQuery == TASK_COMPLETE) {
                    Serial.println("Task completed");
                    showCenteredMessage("Task completed");
                    delay(1000); // Wait for 2 seconds
                    timerRunning = false; // Stop the timer
                    task_control_service.TimerDone();
                    task_control_service.NewTrophy();
                    startNextTask(); // Start the next task
                } else if (taskCompleteQuery == TASK_EXTEND) {
                    Serial.println("Task extended");
                    showCenteredMessage("Task extended");
                    delay(1000); // Wait for 2 seconds
                    task_control_service.addTime();
                    task_control_service.TimerDone();
                    timerMinutes = task_control_service.tasks[task_control_service.getIndex()].timers[0]; // Reset timer to 25 minutes
                    timerSeconds = 0; // Reset seconds to 0
                    secSinceStart = 0; // Reset seconds since start
                    
                    timerRunning = true; // Stop the timer
                    lastTimerUpdateTime = millis(); // Reset last timer update time
                    currentState = TIMER_RUNNING_SCREEN; // Switch to timer running state
                 }
                break;
            case INTERRUPT_QUERY_SCREEN:
                Serial.println("Interrupt choice selected");
                switch (interruptChoice) {
                    case INTERRUPT_PAUSE:
                        Serial.println("Task paused");
                        timerRunning = false; // Pause the timer
                        currentState = TASK_SCREEN; // Switch to task screen
                        break;
                    case INTERRUPT_DONE:
                        Serial.println("Task marked as done");
                        timerRunning = false; // Stop the timer
                        task_control_service.taskDoneEarly();
                        task_control_service.NewTrophy();
                        startNextTask(); // Start the next task
                        break;
                    case INTERRUPT_RESUME:
                        Serial.println("Resuming timer");
                        showCenteredMessage("Switching to timer running screen");
                        delay(1000); // Wait for 2 seconds
                        timerRunning = true; // Resume the timer
                        lastTimerUpdateTime = millis(); // Reset last timer update time
                        currentState = TIMER_RUNNING_SCREEN; // Switch to timer running state
                        break;
                }
                break;
        }
        longPressHandled = true; // Mark long press as handled
    }
}

void handleButtonRelease() {
    if (buttonState == HIGH && lastButtonState == LOW && !singlePressHandled) {
        releaseTime = currentTime;
        if (!longPressHandled) {
            Serial.println("Single press detected");
            handleSinglePress();
            singlePressHandled = true; // Mark single press as handled
          }
    }
}

void handleSinglePress() {
    static int pressCounter = 0;
    Serial.print("handleSinglePress() called #: ");
    Serial.println(++pressCounter);

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
                timerMinutes = task_control_service.tasks[task_control_service.getIndex()].timers[0]; // Reset timer to 25 minutes
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

        case (TASK_COMPLETE_QUERY_SCREEN):
            taskCompleteQuery = static_cast<TaskCompleteQuery>((taskCompleteQuery + 1) % NUM_TASK_COMPLETE_CHOICES);
            Serial.print("Task complete choice: ");
            Serial.println(taskCompleteQuery == TASK_COMPLETE ? "Complete" : "Extend");
            Serial.print("taskCompleteQuery numeric value: ");
            Serial.println(static_cast<int>(taskCompleteQuery));
            break;
            
        case INTERRUPT_QUERY_SCREEN:
            interruptChoice = static_cast<InterruptChoice>((interruptChoice + 1) % NUM_INTERRUPT_CHOICES);
            Serial.print("Interrupt choice: ");
            switch (interruptChoice) {
                case INTERRUPT_PAUSE:  Serial.println("Pause"); break;
                case INTERRUPT_DONE:   Serial.println("Done"); break;
                case INTERRUPT_RESUME: Serial.println("Resume"); break;
            }
            break;
    } 
}


void startNextTask() {
    if (task_control_service.getIndex() >= task_control_service.tasks.size() && task_control_service.TasksCompleted() == task_control_service.tasks.size()) {
        currentState = TASK_DONE;
        return;
    }
    // Reset scroll position and timer
    scrollResetX = -CHAR_WIDTH * task_control_service.tasks[task_control_service.getIndex()].taskName.length();
    scrollX = SCREEN_WIDTH;
    timerMinutes = task_control_service.tasks[task_control_service.getIndex()].timers[0];
    timerSeconds = 0;
    secSinceStart = 0;
    currentState = TASK_SCREEN;
}


void prepareTaskScreen() {
    if (task_control_service.getIndex() < task_control_service.tasks.size()) {
      int textWidth = CHAR_WIDTH * task_control_service.tasks[task_control_service.getIndex()].taskName.length();
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
                taskCompleteQuery = TASK_COMPLETE; // Reset task complete query
                currentState = TASK_COMPLETE_QUERY_SCREEN; // Switch back to task screen
                return;
            } else {
                timerMinutes--;
                timerSeconds = 59; // Reset seconds to 59
        } } else {
            timerSeconds--;
        }
        showTimerScreen(task_control_service.tasks[task_control_service.getIndex()].taskName, timerMinutes, timerSeconds); // Update the timer screen
    }
}

void updateScreenMode() {
    if (timerRunning && currentState != TASK_COMPLETE_QUERY_SCREEN && currentState != INTERRUPT_QUERY_SCREEN) 
        return;
    switch (currentState) {
        case MENU_SCREEN: // Show menu screen
            showMenuScreen();
            break;
        case TASK_SCREEN: // Show task screen
            task_control_service.taskPriority(15);
            updateScrollPosition(String(task_control_service.tasks[task_control_service.getIndex()].taskName.c_str()));
            showCurrentTask(String(task_control_service.tasks[task_control_service.getIndex()].taskName.c_str()));
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
        case TASK_COMPLETE_QUERY_SCREEN: // Show task complete query screen
            showTaskCompleteQueryScreen();
            break;
        case INTERRUPT_QUERY_SCREEN: // Show interrupt query screen
            showInterruptQueryScreen();
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
    display.println(task_control_service.getTrophys());

    // Printing the number of tasks completed today out of all daily tasks

    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Tasks completed today: ");
    display.setCursor(0, 56);
    display.print(task_control_service.TasksCompleted());
    display.print("/");
    display.print(task_control_service.NumberOfTasks());


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

void showTaskCompleteQueryScreen() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Is the task complete?");

    int y0 = 24;
    int y1 = 40;

    auto drawOption = [&](TaskCompleteQuery choice, const char* text, int y) {
        if (taskCompleteQuery == choice) {
            display.fillRect(0, y - 2, 128, 11, WHITE);
            display.setTextColor(BLACK);
            display.setCursor(5, y);
            display.print(text);
            display.setTextColor(WHITE);
        } else {
            display.setCursor(5, y);
            display.print(text);
        }
    };

    drawOption(TASK_COMPLETE, "Yes, task is done", y0);
    drawOption(TASK_EXTEND, "No, need more time", y1);
    Serial.print("Showing screen with query value: ");
    Serial.println(static_cast<int>(taskCompleteQuery));

    display.display();
}

void showInterruptQueryScreen() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Task interrupted. Choose:");

    // Define Y positions for each line
    int y0 = 20;
    int y1 = 32;
    int y2 = 44;

    // Helper to draw one line with optional highlight
    auto drawOption = [&](InterruptChoice choice, const char* text, int y) {
        if (interruptChoice == choice) {
            display.fillRect(0, y - 2, 128, 11, WHITE);
            display.setTextColor(BLACK);
            display.setCursor(5, y);
            display.print(text);
            display.setTextColor(WHITE);
        } else {
            display.setCursor(5, y);
            display.print(text);
        }
    };

    drawOption(INTERRUPT_PAUSE, "Pause Task", y0);
    drawOption(INTERRUPT_DONE, "Mark as Done", y1);
    drawOption(INTERRUPT_RESUME, "Resume Timer", y2);

    display.display();
}
