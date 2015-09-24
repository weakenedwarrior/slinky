/*
  SLINKY - Script to control LEDs for Slinkypants Costume.
*/

#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>
#include "lightrun.h"


#define CYCLEPERIOD   20    // milliseconds
#define BUTTON1PIN    18
#define BUTTON2PIN    19
#define BUTTONPRESSED 13
#define LEDCONTROLPIN  6
#define TOTALPIXELS   28

// Store timer here
unsigned long timer = 0;

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

  // Space out serial output
  //if (myLightRunsList.size() > 0) {
  //  Serial.println("");
  //}

  // Wait for timer
  waitForTimer();

}

void processButtonPushes() {
  if (button1pressed) {
    addLightRun();
    button1pressed = false;
  }
  if (button2pressed) {
    addLightRun();
    button2pressed = false;
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
    while(millis() - timer < CYCLEPERIOD) {};
  }
  // Set the next timer
  timer = millis();
}

  

