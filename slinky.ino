/*
  SLINKY - Script to control LEDs for Slinkypants Costume.
*/
#include <StandardCplusplus.h>
#include <vector>
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>
#include "lightrun.h"
#include "bounce.h"
#include "trail.h"
#include "jumpy.h"

#define CYCLEPERIOD          20    // milliseconds
#define BUTTON1PIN           18
#define BUTTON2PIN           19
#define BUTTON3PIN           20
#define BUTTONPRESSED        13
#define LEDCONTROLPIN1        8
#define LEDCONTROLPIN2        9
#define LEDCONTROLPIN3       10
#define LEDCONTROLPIN4       11
#define TOTALPIXELS          50
#define BUTTON_RESET_DELAY  100    // milliseconds

// Store timers here
unsigned long timer = 0;
unsigned long button1timer = 0;
unsigned long button2timer = 0;
unsigned long button3timer = 0;

// Store Bounce count
unsigned long bounce_count = 0;

// Button setup and ISRs
volatile bool button1pressed = false;
volatile bool button2pressed = false;
volatile bool button3pressed = false;

void setbutton1() {
  button1pressed = true;
}

void setbutton2() {
  button2pressed = true;
}

void setbutton3() {
  button3pressed = true;
}

// Classes for LED state
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(TOTALPIXELS , LEDCONTROLPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(TOTALPIXELS , LEDCONTROLPIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(TOTALPIXELS , LEDCONTROLPIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(TOTALPIXELS , LEDCONTROLPIN4, NEO_GRB + NEO_KHZ800);

// Global list of light runs
LinkedList<Lightrun*> myLightRunsList = LinkedList<Lightrun*>();

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Slinky!" );

  // Setup Button ISRs
  attachInterrupt(digitalPinToInterrupt(BUTTON1PIN), setbutton1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2PIN), setbutton2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3PIN), setbutton3, FALLING);

  // Required to setup LED strip controller
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
}

void loop() {
  // The first thing we do is clock out all the LED values and then show them
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  
  // Add light runs if buttons are pressed
  processButtonPushes();
  
  // Loop through all light runs
  processAll();
  
  // Wait for timer
  waitForTimer();
}

void processButtonPushes() {
  /*if (button1pressed && button2pressed) {
    addTrail();
  } else if (button1pressed) {
    //addLightRun();
    addJumpy();
  } else if (button2pressed) {
    addBounce();
  }*/

  if (button1pressed) {
    addLightRun1(strip1.Color(0, 255, 0));
    //addBounce();
  } 
  if (button2pressed) {
    addLightRun2(strip2.Color(255, 0, 0));
  } 
  if (button3pressed) {
    addLightRun3(strip3.Color(0, 0, 255));
  }
    
  // Reset  
  if (button1pressed) {
    button1timer = millis();
    button1pressed = false;
    detachInterrupt(digitalPinToInterrupt(BUTTON1PIN));
  }
  if (button2pressed) {
    button2timer = millis();
    button2pressed = false;
    detachInterrupt(digitalPinToInterrupt(BUTTON2PIN));
  }
  if (button3pressed) {
    button3timer = millis();
    button3pressed = false;
    detachInterrupt(digitalPinToInterrupt(BUTTON3PIN));
  }

  
}

void addLightRun1(uint32_t color) {
  Lightrun *lightrun = new Lightrun(&strip1, color);
  myLightRunsList.add(lightrun);
}

void addLightRun2(uint32_t color) {
  Lightrun *lightrun = new Lightrun(&strip2, color);
  myLightRunsList.add(lightrun);
}

void addLightRun3(uint32_t color) {
  Lightrun *lightrun = new Lightrun(&strip3, color);
  myLightRunsList.add(lightrun);
}

void addBounce() {
  // Create a Bounce

  uint32_t nextcolor;
  byte modcount = bounce_count % 3;
  if (modcount == 0) {
    nextcolor = strip1.Color(255, 0, 0);
  } else if (modcount == 1) {
    nextcolor = strip1.Color(0, 255, 0);
  } else {
    nextcolor = strip1.Color(0, 0, 255);
  }
  bounce_count += 1;
  
  Bounce *bounce = new Bounce(&strip1, nextcolor);
  myLightRunsList.add(bounce);
}

void addTrail() {
  if (!activeTrailFound()) { 
    // Create a Trail
    uint32_t red = strip1.Color(255, 0, 0);
    uint32_t darkblue = strip1.Color(0, 0, 100);
    Trail *trail = new Trail(&strip1, red, darkblue);
    myLightRunsList.add(trail);
  }    
}

bool activeTrailFound() {
  // Look for a "Trail" lightrun in myLightRunsList
 for(int i = 0; i < myLightRunsList.size(); i++) {
    Lightrun *lightrun = myLightRunsList.get(i);
    if (lightrun->getPattern() == TRAIL) {
      return true;
    }
  }
  return false;
}

void addJumpy() {
  uint32_t white = strip1.Color(255, 255, 255);
  Jumpy *jumpy = new Jumpy(&strip1, white);
  myLightRunsList.add(jumpy);
}

void processAll() {
  // Loop backwards through list, so that deletions don't mess up index
  // of remaining elements
  for(int i = myLightRunsList.size()-1; i >= 0; i--)
    processLightRun(i);
}

void processLightRun(int i) {
  // Get lightrun from list
  Lightrun *lightrun = myLightRunsList.get(i);

  // Get lightrun to do its thing, or delete itself if it's finished
  if (lightrun->isDone()) {
    removeLightRun(i);
  } else {
    lightrun->moveToNext();
  }
}

void removeLightRun(int i) {
  // Remove from linked list then delete
  Lightrun *lightrun = myLightRunsList.get(i);
  myLightRunsList.remove(i);
  delete lightrun;
}

void waitForTimer() {
  if (timer != 0) {
    // Wait until at least the scan period has elapsed
    while(millis() - timer < CYCLEPERIOD) {

      // Monitor button timers too
      if (button1timer != 0 && millis() - button1timer > BUTTON_RESET_DELAY) {
        attachInterrupt(digitalPinToInterrupt(BUTTON1PIN), setbutton1, FALLING);
        button1timer = 0;
        button1pressed = false;
      }
      if (button2timer != 0 && millis() - button2timer > BUTTON_RESET_DELAY) {
        attachInterrupt(digitalPinToInterrupt(BUTTON2PIN), setbutton2, FALLING);
        button2timer = 0;
        button2pressed = false;
      }
      if (button3timer != 0 && millis() - button3timer > BUTTON_RESET_DELAY) {
        attachInterrupt(digitalPinToInterrupt(BUTTON3PIN), setbutton3, FALLING);
        button3timer = 0;
        button3pressed = false;
      }

    };
  }
  // Set the next timer
  timer = millis();
}

  

