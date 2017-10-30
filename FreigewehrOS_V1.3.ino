#include "Config.h"

volatile int triggerPullCount = 0;
volatile int pusherState = 0;
int totalWait = 0;


void setup() {
  pinMode(PUSHER_RELAY_PIN, OUTPUT);
  pinMode(FLYWHEEL_RELAY_PIN, OUTPUT);
  pinMode(TRIGGER_SWITCH_PIN, INPUT_PULLUP);
  pinMode(PUSHER_SWITCH_PIN, INPUT_PULLUP);
 
  digitalWrite(PUSHER_RELAY_PIN, HIGH);
  digitalWrite(FLYWHEEL_RELAY_PIN, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(TRIGGER_SWITCH_PIN), triggerPulled, FALLING);
  //attachInterrupt(digitalPinToInterrupt(PUSHER_SWITCH_PIN), pusherReturned, FALLING);
}

void loop() {
  totalWait = 0;
  pusherState = 0;
 
  // Firing Loop  
  if(triggerPullCount > 0){
    triggerPullCount =0;
      delay(PUSHER_LAG_MS);
    
    digitalWrite(PUSHER_RELAY_PIN, LOW); // Turns on the pusher
    /*
     * PUSHER STATES:
     * 0 - Entry State, assumed retracted
     * 1 - Pusher has been extended
     * 2 - Pusher has been retracted after an extension
     * 3 - Too much time has passed, and the pusher is assumed to be jammed
     */
    while(!digitalRead(PUSHER_SWITCH_PIN)){
      delay(1);
    }
    delay(275);
    digitalWrite(PUSHER_RELAY_PIN, HIGH);

  }else{
    delay(10);
  }
}

void triggerPulled(){
  static unsigned long lastTriggerPullTime = 0;
  unsigned long triggerPullTime = millis();
  if(triggerPullTime - lastTriggerPullTime > DEBOUNCE_MS){
    triggerPullCount++;
  }
  lastTriggerPullTime = triggerPullTime;
}

// Interrupt for a pusher switch falling-edge (pusher-returned)
// Debounces via a waited-time threshold
/*
void pusherReturned(){
  static unsigned long lastPusherPressTime = 0;
  unsigned long pusherPressTime = millis();
  if(pusherPressTime - lastPusherPressTime > DEBOUNCE_MS && pusherState == 1){
    pusherState = 2;
  }
  lastPusherPressTime = pusherPressTime;
}
*/

