/*
  Gadget Factory
  Papilio One QuickStart Example
 
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
  0, 1, 2, 3, 4, 5, 6, 7  };       // an array of pin numbers to which LEDs are attached
int ledCount = 8;           // the number of pins (i.e. the length of the array)

int buttonPins[] = { 
  8 };       // an array of pin numbers to which Buttons are attached
int buttonCount = 1;           // the number of pins (i.e. the length of the array)



// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int thisPin;
int ledState = LOW;

// first visible ASCIIcharacter '!' is number 33:
int thisByte = 33; 
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!'; 

void setup() {
  // initialize the LED pins as an output:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);  
  }
  
  // initialize the pushbutton pin as an input:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(buttonPins[thisPin], INPUT);      
  }  
  
  //Delay for 3 seconds to prevent detection as a serial mouse.
  delay(3000);
  
  //Setup Serial port and send out Title
  Serial.begin(9600); 

  // prints title with ending line break 
  Serial.println("ASCII Table ~ Character Map"); 
}

void loop(){
  //This sends the ASCII table to the serial port.
  
  // prints value unaltered, i.e. the raw binary version of the 
  // byte. The serial monitor interprets all bytes as 
  // ASCII, so 33, the first number,  will show up as '!' 
  //Serial.print(thisByte, BYTE);    

  Serial.print("dec: "); 
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);      
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:

  // Serial.print(thisByte, DEC);  


  Serial.print(", hex: "); 
  // prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);     

  Serial.print(", oct: "); 
  // prints value as string in octal (base 8);
  Serial.print(thisByte, OCT);     

  Serial.print(", bin: "); 
  // prints value as string in binary (base 2) 
  // also prints ending line break:
  Serial.println(thisByte, BIN);   
 
  
  //This section blinks the LED's and keeps them solid if a button is pressed. 
  delay(1000);                  // wait for a second 

  for (int thisPin = 0; thisPin < ledCount; thisPin++)  
  {
    if (thisByte & (1<<thisPin)) 
    {     
      // turn LED on:    
      digitalWrite(ledPins[thisPin], LOW);  
    } 
    else 
    {
      digitalWrite(ledPins[thisPin], HIGH); 
    }
  }
  
  // if printed last visible character '~' or 126, stop: 
  if(thisByte == 126) {     // you could also use if (thisByte == '~') {
    thisByte = 33;
  } 
  // go on to the next character
  thisByte++; 
 
  /*
  ledState = !ledState;  
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
      // toggle LED:
      digitalWrite(ledPins[thisPin], ledState); 
    }
  }
  */
}
