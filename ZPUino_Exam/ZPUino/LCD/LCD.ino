#include "SPI.h"
#include "GLCD.h"


GLCD_Class GLCD;


int led = 1;

// the setup routine runs once when you press reset:
void setup() 
{
	// initialize the digital pin as an output.
	pinMode(led, OUTPUT);     

	GLCD.begin();
	GLCD.loop();
}

// the loop routine runs over and over again forever:
void loop() 
{
	//digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
	//delay(500);               // wait for a second
	//digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
	//delay(500);               // wait for a second

	GLCD.clear();
	GLCD.print(0,0,"aasdfasdfasdf");

}