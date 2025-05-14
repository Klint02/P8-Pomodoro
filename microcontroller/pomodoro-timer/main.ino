#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

ButtonHandler button(D5, 3000); // Button handler for pin D5 with a long press threshold of 3 seconds


class Screen {
    public:
        virtual void draw(Adafruit_SSD1306& display) = 0;
        virtual void handleInput(bool singlePress, bool longPress) = 0;
        virtual ~Screen() = default;
};

class ButtonHandler {
    public:
        ButtonHandler(int pin, unsigned long longPressThreshold = 3000) :
            pin(pin), longPressThreshold(longPressThreshold) {}
    
    void update() {
            bool reading = digitalRead(pin);
            unsigned long currentTime = millis();

    // Check for button state change
    if (reading != lastState) {
        lastdebounceTime = currentTime;
    }

    // If the button state has been stable for the debounce time
    if ((currentTime - lastdebounceTime) > 50) {
        // Update the button state
        if (reading != currentState) {
            currentState = reading;

            // If the button is pressed
            if (currentState == LOW) {
                lastPressTime = currentTime;
            else {
                // Check for long press
                unsigned long pressDuration = currentTime - lastPressTime;

                // If the button was pressed long enough
                if (pressDuration >= longPressThreshold) {
                    longPressDetected = true;
                } else {
                    singlePressDetected = true;
                }
            }
        }
    }
    lastState = reading;
    }
    // Reset the flags
    singlePress = singlePressDetected;
    longPress = longPressDetected;
    singlePressDetected = false;
    longPressDetected = false;
    };

    bool isSinglePress() const { 
        bool detected = singlePressDetected;
        singlePressDetected = false; // Reset after reading
        return detected;
    }
    bool isLongPress() const { 
        bool detected = longPressDetected;
        longPressDetected = false; // Reset after reading
        return detected;
    }

    private:
        int pin;
        unsigned long longPressThreshold;

        bool currentState = HIGH;
        bool lastState = HIGH;

        unsigned long lastdebounceTime = 0;
        unsigned long lastPressTime = 0;

        bool singlePressDetected = false;
        bool longPressDetected = false;
};



void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Booting...");

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
        for(;;);
    }
    
}

void loop() {
    button.update();

}