#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1 // Reset pin not used
#define SCREEN_ADDRESS 0x3C // Most common I2C address for 128x64 OLED

#define SDA_PIN D3  
#define SCL_PIN D4 
#define BTN 14

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


String tasks[] = {"Buy birthday present", "Cleaning","Hejsa med digsa","super duper","intet"};//, "Pick up kids", "Groceries"};
int taskNum = 0;
String question = "Click the button to start task";

int currentBTNstate = HIGH;
int lastBTNstate = HIGH;
unsigned long currentMillis;
unsigned long startmillis;
unsigned long t1 = 0;
unsigned long t2 = 0;
signed long timepressed = 0;
int minutes = 25;
int sec = 0;
bool running = false;
int minX;
int x = 0;
signed long secSinceStart =0.0; 
bool timerbool=true;

//Hourglass bitmap when timer is running.
const unsigned char myBitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x03, 0x7f, 0xff, 0xff, 0xfb, 0x7f, 0xff, 0xff, 0xf3, 0x1f, 0xff, 0xff, 0xe3,
	0x0f, 0xff, 0xff, 0x83, 0x0c, 0x00, 0x01, 0x83, 0x0c, 0x00, 0x00, 0x83, 0x0c, 0x00, 0x00, 0x83,
	0x0c, 0x00, 0x00, 0x83, 0x0c, 0x00, 0x00, 0x83, 0x0c, 0x00, 0x00, 0x83, 0x0d, 0xff, 0xfc, 0x83,
	0x0d, 0xff, 0xfc, 0x83, 0x06, 0x7f, 0xf9, 0x83, 0x03, 0x3f, 0xf7, 0x03, 0x01, 0x9f, 0xe6, 0x03,
	0x00, 0xcf, 0x9c, 0x03, 0x00, 0x67, 0x38, 0x03, 0x00, 0x38, 0x60, 0x03, 0x00, 0x18, 0x40, 0x03,
	0x00, 0x18, 0x40, 0x03, 0x00, 0x30, 0x70, 0x03, 0x00, 0x60, 0x38, 0x03, 0x01, 0xc0, 0x0c, 0x03,
	0x03, 0x80, 0x06, 0x03, 0x06, 0x00, 0x03, 0x03, 0x0c, 0x03, 0x01, 0x83, 0x0c, 0x07, 0x80, 0x83,
	0x0c, 0x0f, 0xc0, 0x83, 0x0c, 0x1f, 0xe0, 0x83, 0x0c, 0x3f, 0xf0, 0x83, 0x0c, 0xff, 0xf8, 0x83,
	0x0d, 0xff, 0xfc, 0x83, 0x0c, 0x00, 0x00, 0x83, 0x0f, 0xff, 0xff, 0x83, 0x1f, 0xff, 0xff, 0xc3,
	0x3f, 0xff, 0xff, 0xe3, 0x7f, 0xff, 0xff, 0xf3, 0x7f, 0xff, 0xff, 0xfb, 0x00, 0x00, 0x00, 0x03
};
// Functions
void displayCurrentTask();
void readButtonState();
void timer();
void pressRead();
int calcBarPct();
void doneFunc();








void setup() {
  Serial.begin(115200);
  
  Wire.begin(SDA_PIN, SCL_PIN);  // <-- tell Wire which pins to use

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Stay stuck here
  }
   display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  pinMode(BTN, INPUT_PULLUP);
  startmillis = millis();
  minX = -12 * (tasks[taskNum].length());
}

bool lastButtonState = HIGH; // start assuming unpressed

void loop()
{
  //Checks if running = true else just display current task.
  if(!running && taskNum < sizeof(tasks)/sizeof(tasks[0])){
    displayCurrentTask();
    pressRead();
  }else if(taskNum >= sizeof(tasks)/sizeof(tasks[0])){
    // screen for when all tasks are complete
    doneFunc();
  }else {
    //runs timer function
    timer();
    pressRead();
  }

  //Decide whether the task is long enough for the screen or not.
  if(x < minX ) {
    x = display.width();
  }else if((12*tasks[taskNum].length())> 124){
    x--;
  }else{
    x=0;
  }
}
void timer()
{

  currentMillis = millis();
  
  if (currentMillis - startmillis >= 1000)
  {
    startmillis = currentMillis;
    if (sec <= 0)
    {
      if (minutes > 0 && timerbool)
      {
        minutes--;
        sec = 59;
      }
      else
      { // End of timer
        running = false;
        taskNum++;
         if (taskNum < sizeof(tasks)/sizeof(tasks[0])) {
        minX = -12 * tasks[taskNum].length();
        }
        secSinceStart = 0.0;
        return;
      }
    }
    else
    {
      sec--;
    }
    secSinceStart++;

    // display the task at the top.
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(tasks[taskNum]);

    // displays the timer in the center. 
    //SetCursor x coordinat skal være (11,32) hvis det er bitmap og (32,22) hvis loadbar bruges.
    display.setCursor(11, 32);
    display.setTextSize(2);
    display.print(String(minutes) + ":" + String(sec));

     display.drawBitmap(82, 20, myBitmap, 30, 40, WHITE);
    // display.drawRect(0,50,124,10,WHITE);
    // display.fillRect(0,50,calcBarPct(),10,WHITE);
    
    display.setTextSize(1);
    display.display();
  }
}

void displayCurrentTask()
{

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Next task: ");
  display.setTextSize(2);

  display.setCursor(x, 24);
  display.print(tasks[taskNum]);
  display.setTextSize(1);

  display.setCursor(0, 56);
  display.print("Long press to start.");

  display.display();
}

// Checks for short and long press
void pressRead()
{
  currentBTNstate = digitalRead(BTN);
  t1 = millis();
  
  while (currentBTNstate == LOW)
  {
    x=0;
    displayCurrentTask();
    delay(50);
    currentBTNstate = digitalRead(BTN);

    t2 = millis();
    timepressed = t2 - t1;

    if (timepressed >= 3000 && currentBTNstate == LOW)
    {
      Serial.println("Long press");
      // change the flag
      running = false;
      break;
    }
    else if (timepressed < 3000 && currentBTNstate == HIGH && !running)
    {
      Serial.println("Short press");
      running = true;

      // resets the time.
      break;
    }else if (timepressed < 3000 && currentBTNstate == HIGH && running) {
      delay(100);
      running = false;
      minutes = 25;
      sec = 0;
      taskNum++;
      if (taskNum < sizeof(tasks)/sizeof(tasks[0])) {
      minX = -12 * tasks[taskNum].length();
      }
      secSinceStart = 0.0;
    }
  }
}
//Load-bar for other display feature.
int calcBarPct(){
  return round((float)secSinceStart/1500*124);
}
// screen after all tasks are complete.
void doneFunc(){
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

