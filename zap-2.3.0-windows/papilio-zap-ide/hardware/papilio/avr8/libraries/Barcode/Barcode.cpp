/*
  Barcode.h - Library for taking input from PS/2 Barcode scanners.
  Created by Benjamin Maus	( benjamin.maus <at> allesblinkt.com ), 2007.
  Ported to a library by Jack Gassett (jack.gassett@gadgetfactory.net), 2010
  Released into the public domain.
*/

#include "WProgram.h"
#include "Barcode.h"

Barcode::Barcode(int clockPin, int dataPin, int ledPin, int buttonPin)
{
  pinMode(clockPin, INPUT);
  pinMode(dataPin, INPUT);
  pinMode(ledPin, OUTPUT);
  _clockPin = clockPin;
  _dataPin = dataPin;
  _ledPin = ledPin;
  _buttonPin = buttonPin;
  this->breakActive = 0;
  this->clockValue = 0; 
  //this->scanCodes[51] = {0x70, 0x69, 0x72, 0x7a, 0x6b, 0x73, 0x74, 0x6c, 0x75, 0x7d, 0x1c, 0x32, 0x21, 0x23, 0x24, 0x2b, 0x34, 0x33, 0x43, 0x3b, 0x42, 0x4b, 0x3a, 0x31, 0x44, 0x4d, 0x15, 0x2d, 0x1b, 0x2c, 0x3c, 0x2a, 0x1d, 0x22, 0x35, 0x1a, 0x16, 0x1e, 0x26, 0x25, 0x2e, 0x36, 0x3d, 0x3e, 0x46, 0x45, 0x29, 0x41, 0x49, 0x4a, 0X5A}; 
  //this->characters[51] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' ', ',', '.', '/', 13};
  this->quantityCodes = 51;
  this->bufferPos = 0; 
  this->bufferLength = 64;	
  scannedCode = buffer;
  buttonState = 0;
}

void Barcode::dataRead()
{
	byte val = 0;                                                          
	// Skip start state and start bit                                      
	while (digitalRead(_clockPin));  // Wait for LOW.                       
	// clock is high when idle                                             
	while (!digitalRead(_clockPin)); // Wait for HIGH.                      
	while (digitalRead(_clockPin));  // Wait for LOW.                       
	for (int offset = 0; offset < 8; offset++) {                           
		while (digitalRead(_clockPin));         // Wait for LOW               
		val |= digitalRead(_dataPin) << offset; // Add to byte                
		while (!digitalRead(_clockPin));        // Wait for HIGH              
	}                                                                      
	// Skipping parity and stop bits down here.                            
	while (digitalRead(_clockPin));           // Wait for LOW.              
	while (!digitalRead(_clockPin));          // Wait for HIGH.             
	while (digitalRead(_clockPin));           // Wait for LOW.              
	while (!digitalRead(_clockPin));          // Wait for HIGH.             
	this->dataValue = val;  
}

void Barcode::scan()
{
	while (1)
	{
		dataRead();
		// If there is a break code, skip the next byte                        
		if (this->dataValue == SCAN_BREAK) {                                         
			breakActive = 1;                                                     
		}                                                                      
		// Translate the scan codes to numbers                                 
		// If there is a match, store it to the buffer                         
		for (int i = 0; i < quantityCodes; i++) {      	                       
			byte temp = scanCodes[i];                                            
			if(temp == this->dataValue){                                               
				if(!breakActive == 1){                                             
					buffer[bufferPos] = characters[i];                               
					bufferPos++;   
				}                                                                  
			}                                                                    
		}                                                                      
		if(this->dataValue == SCAN_ENTER){
			//Serial.print("\nbuffer: ");                                          
			//Read the buffer                                                   
			// int i=0;																		                         
			// if (buffer[i] != 0) {                                                
				// while(buffer[i] != 0) {                                            
					// Serial.print(buffer[i] );                                       
					// buffer[i] = 0;                                                   
					// i++;                                                             
				// }                                                                  
			// }       
			//Serial.print(buffer);
			//file->write(buffer,bufferPos);
			//file->println(",");
			//Serial.println(" [Enter]");                                          
			bufferPos = 0;                                                       
			// Blink the LED	                                                   
			digitalWrite(_ledPin, HIGH);                                          
			delay(100);                                                          
			digitalWrite(_ledPin, LOW);                                           
			break;
		}                                                                      
		// Reset the SCAN_BREAK state if the byte was a normal one             
		if(this->dataValue != SCAN_BREAK){                                           
			breakActive = 0;                                                     
		}                                                                      
		dataValue = 0; 
	}
}

