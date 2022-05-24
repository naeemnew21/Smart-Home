#include <Arduino_JSON.h>


//********************* Variables declaration **********************

JSONVar relay_status; // the current status of relay1

JSONVar switch_status; // current status of switch

JSONVar last_status; // Last status of switch

unsigned long lastDebounceTime[6]; // the last time the output pin was toggled - 6: the feedback numbers

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers - 25 = 50, 30 = 60...


//-------------------------------------
int pin;
int State;

int last_pin;
int last_state;

byte Device[13];

void refresh(){
  for (int i=0; i<6; i++){
    Device[i] = (int)relay_status[String(i+2)];
  }
  for (int i=6; i<12; i++){
    Device[i] = digitalRead(i+2);
  }
}

void setup(){
 Serial.begin(9600);
 delay(2000);
 for (int i=2; i<14; i++){
  pinMode(i, OUTPUT);
  relay_status[String(i)] = 0; 
 }
 for (int i=14; i<20; i++){
  pinMode(i, INPUT_PULLUP);
 }
 Device[12] = 3; // to check if send receiving array correct

 //make first readings as refrence for first run
 for(int i= 14; i<20 ; i++){
   int reading = digitalRead(i);
   switch_status[String(i)] = reading ;
 }
  

}

void loop()
{
    int rx;
    while (Serial.available()) {
      rx = Serial.read();
      if (rx > 1 && rx < 14){ // rx limited by pins used for relay to protect pins used for feedback
        pin = rx;
      }
      if (rx == 0 || rx == 1){
        State = rx;
      }
    }

    if (pin != last_pin || State != last_state){
        digitalWrite(pin, State);
        relay_status[String(pin)] = State;
        last_pin = pin;
        last_state = State;
        refresh();
        Serial.write(Device , 13);
    }

  for (int i =14; i<20 ; i++){
    int reading = digitalRead(i);
    if (reading != (int)last_status[String(i)]){
       lastDebounceTime[i-14] = millis(); 
    }
    if ((millis() - lastDebounceTime[i-14]) > debounceDelay) 
    {
       if (reading != (int)switch_status[String(i)]){
           switch_status[String(i)] = reading;
           if (reading == HIGH){
              if ((int)relay_status[String(i-12)] == 0){
                relay_status[String(i-12)] = 1;
              }else {
                relay_status[String(i-12)] = 0;
              }
              refresh();
              Serial.write(Device , 13);
           }
       }
    }
    digitalWrite(i-12, (int)relay_status[String(i-12)]);
    last_status[String(i)] = reading;
  }

  
}
