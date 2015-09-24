/*
  SLINKY - Script to control LEDs for Slinkypants Costume.
*/

#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>
#include "lightrun.h"


#define CYCLEPERIOD   40    // milliseconds
#define BUTTON1PIN    18
#define BUTTON2PIN    19
#define BUTTONPRESSED 13
#define LEDCONTROLPIN  6
#define TOTALPIXELS   28

// Store timer here
unsigned long timer = 0;

// Buttons Data
volatile bool button1pressed = false;
volatile bool button2pressed = false;

// Class for LED state
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTALPIXELS , LEDCONTROLPIN, NEO_RGB + NEO_KHZ800);

// Global list of light runs
LinkedList<Lightrun*> myLightRunsList = LinkedList<Lightrun*>();


void setup() {
  Serial.begin(9600);
  Serial.println("Hello!" );

  // Setup Button ISRs
  attachInterrupt(digitalPinToInterrupt(BUTTON1PIN), setbutton1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2PIN), setbutton2, FALLING);
  
}

void loop() {
  
  // Add light runs if buttons are pressed
  processButtonPushes();

  Serial.print("There are ");
  Serial.print(myLightRunsList.size());
  Serial.print(" lightruns in the list");
  Serial.println(".");

  // Loop through light runs
  processLightRuns();

  // Wait for timer
  waitForTimer();

  //removeLightRun(0);

}


//Button ISRs
void setbutton1() {
  button1pressed = true;
}
void setbutton2() {
  button2pressed = true;
}

void processButtonPushes() {
  if (button1pressed) {
    addLightRun();
    button1pressed = false;
  }

  if (button2pressed) {
    removeLightRun(0);
    button2pressed = false;
  }
}


void waitForTimer() {
  if (timer != 0) {
    // Wait until at least the scan period has elapsed
    while(millis() - timer < CYCLEPERIOD) {};
  }
  // Set the next timer
  timer = millis();
}

void addLightRun() {
  // Create a LightRun
  Lightrun *lightrun = new Lightrun(&strip);
  myLightRunsList.add(lightrun);
}

void removeLightRun(int i) {
  // Remove from linked list then delete
  Lightrun *lightrun = myLightRunsList.get(i);
  myLightRunsList.remove(i);
  delete lightrun;
}

void processLightRuns() {
  Lightrun *lightrun;
  for(int i = 0; i < myLightRunsList.size(); i++){
    // Get lightrun from list
    lightrun = myLightRunsList.get(i);
    lightrun->flash();
  }
}
