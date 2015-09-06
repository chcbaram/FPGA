/*
  Gadget Factory
  Button/LED Wing Example
  BPW5007
 
 Pressing any of the 4 pushbuttons on the Button/LED Wing will light the corresponding LED. 
 
 BPW5007 Button/LED Wing Reference:
 http://www.gadgetfactory.net/gf/project/bpw5007-butnled/
 
 Hardware:
 * Connect a Button/LED Wing to any available Wing Slot

 created 2010
 by Jack Gassett from existing Arduino code snippets
 http://www.gadgetfactory.net
 
 This example code is in the public domain.
 */

int ledPins[] = { 
  0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46  };       // an array of pin numbers to which LEDs are attached
int ledCount = 24;           // the number of pins (i.e. the length of the array)

int buttonPins[] = { 
  1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47 };       // an array of pin numbers to which Buttons are attached
int buttonCount = 24;           // the number of pins (i.e. the length of the array)

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int thisPin;

void setup() {
  // initialize the LED pins as an output:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);      
  }
  
  // initialize the pushbutton pin as an input:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(buttonPins[thisPin], INPUT);      
  }  
}

void loop(){
  for (int thisPin = 0; thisPin < buttonCount; thisPin++)  {
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPins[thisPin]);
  
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState == HIGH) {     
      // turn LED on:    
      digitalWrite(ledPins[thisPin], HIGH);  
    } 
    else {
      // turn LED off:
      digitalWrite(ledPins[thisPin], LOW); 
    }
  }
}