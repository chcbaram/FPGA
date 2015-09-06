/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 3;

// the setup routine runs once when you press reset:
void setup() {                
  int i;
  
  // initialize the digital pin as an output.
  for( i=0; i<8; i++ )
    pinMode(i, OUTPUT);     
}

  int Ch = 0;
  
// the loop routine runs over and over again forever:
void loop() {
  int i;
  
  
  for( i=0; i<8; i++ )
  {
    if( i == Ch )
    {
      digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else
    {
      digitalWrite(i, LOW);    // turn the LED off by making the voltage LOW
    }
  }
  
  Ch++;
  Ch %= 8;
  
  delay(100);
}
