#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// Indicators for button state
bool longPressHandled = false; // Indicates if a long press has been handled
bool singlePressPending = false; // Indicates if a single press is pending

// LED STATE AND MODE
bool ledState = true; // LED state (true = ON, false = OFF)
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
  
  showIntroScreen();
  digitalWrite(BUILTIN_LED, LOW); // Turn on the LED (active low)
  delay(2000);
}

// Main loop function
// It reads the button state, handles long press, single press, and updates LED mode


void loop() {
    char buffer[32];
    sprintf(buffer, "Now in mode: %d", mode);
    showTextScreen(buffer);
    updateButtonState();
    handleLongPress();
    handleButtonRelease();
    handleSinglePressTimeout();
    updateLEDMode();
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

        // Switch to slow blink mode if in normal mode
        // or return to manual LED mode if in blink mode
        if (mode == 1 || mode == 2) {
            mode = 0;
            Serial.println("Returning to manual LED mode");
        } else {
            mode = 2;
            Serial.println("Switching to slow blink mode");
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
            mode = 1;
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

            // Only toggle LED if in manual mode 
            if (mode == 0) {
              ledState = !ledState;
              digitalWrite(BUILTIN_LED, ledState ? LOW : HIGH);
              Serial.println(ledState ? "LED ON" : "LED OFF");
            }
            singlePressPending = false; // Reset single press pending
            lastClickTime = 0;
            mode = 0; // Switch to normal mode
        }
    }
}

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

