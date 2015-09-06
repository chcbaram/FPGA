#include <Barcode.h>

/*
Barcode Scanner                                                        
	This code reads the input from a ps/2 keyboard or keyboard-like        
	device (e.g. a barcode scanner), translates the scan-codes into        
	numbers (only numbers from 0 to 9 can be used at the moment)           
	It is nowhere near a complete implementation of the ps/2 protocol,     
	but it should give you a starting point.                               
	mys .// Benjamin Maus	( benjamin.maus <at> allesblinkt.com )          
	2007    

Modified for BPW5006 PS/2 Wing by Jack Gassett 10/1/2010
Connect PS/2 Wing to Wing Slot AL and a Button/LED Wing to Wing Slot AH
To change Wing Slots just replace AL with the desired Wing Slot such as AH, BL, BH, CL, or CH.	
*/

int clockPin = CH0; // Clock is only output. For BPW5006 Port 1
int dataPin = CH1; // The data pin is bi-directional For BPW5006 Port 1
int ledPin = CL6;  // When a SCAN_ENTER scancode is received the LED blink


Barcode barcode(clockPin, dataPin, ledPin);

void setup() {
                                              
	Serial.begin(9600);                                                    
}

void loop() {
	barcode.scan();                                                
                                                      
}
