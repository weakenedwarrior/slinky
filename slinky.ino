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
#define BUTTONPRESSED        13
#define LEDCONTROLPIN         7
#define TOTALPIXELS          28
#define BUTTON_RESET_DELAY  100    // milliseconds

// Store timers here
unsigned long timer = 0;
unsigned long button1timer = 0;
unsigned long button2timer = 0;

// Store Bounce count
unsigned long bounce_count = 0;

// Button setup and ISRs
volatile bool button1pressed = false;
volatile bool button2pressed = false;

void setbutton1() {
  button1pressed = true;
}

void setbutton2() {
  button2pressed = true;
}

// Class for LED state
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTALPIXELS , LEDCONTROLPIN, NEO_RGB + NEO_KHZ800);

// Global list of light runs
LinkedList<Lightrun*> myLightRunsList = LinkedList<Lightrun*>();

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Slinky!" );

  // Setup Button ISRs
  attachInterrupt(digitalPinToInterrupt(BUTTON1PIN), setbutton1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2PIN), setbutton2, FALLING);

  // Required to setup LED strip controller
  strip.begin();
}

void loop() {
  // The first thing we do is clock out all the LED values and then show them
  strip.show();
  
  // Add light runs if buttons are pressed
  processButtonPushes();
  
  // Loop through all light runs
  processAll();
  
  // Wait for timer
  waitForTimer();
}

void processButtonPushes() {
  if (button1pressed && button2pressed) {
    addTrail();
  } else if (button1pressed) {
    //addLightRun();
    addJumpy();
  } else if (button2pressed) {
    addBounce();
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

  
}

void addLightRun() {
  // Create a LightRun

  int red = random(255);
  int green = random(255);
  int blue = random(255);
  
  uint32_t color = strip.Color(red, green, blue);
  
  Lightrun *lightrun = new Lightrun(&strip, color);
  myLightRunsList.add(lightrun);
}

void addBounce() {
  // Create a Bounce

  uint32_t nextcolor;
  byte modcount = bounce_count % 3;
  if (modcount == 0) {
    nextcolor = strip.Color(255, 0, 0);
  } else if (modcount == 1) {
    nextcolor = strip.Color(0, 255, 0);
  } else {
    nextcolor = strip.Color(0, 0, 255);
  }
  bounce_count += 1;
  
  Bounce *bounce = new Bounce(&strip, nextcolor);
  myLightRunsList.add(bounce);
}

void addTrail() {
  if (!activeTrailFound()) { 
    // Create a Trail
    uint32_t red = strip.Color(255, 0, 0);
    uint32_t darkblue = strip.Color(0, 0, 100);
    Trail *trail = new Trail(&strip, red, darkblue);
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
  uint32_t white = strip.Color(255, 255, 255);
  Jumpy *jumpy = new Jumpy(&strip, white);
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

    };
  }
  // Set the next timer
  timer = millis();
}

  

