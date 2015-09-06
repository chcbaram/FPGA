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

int SCAN_ENTER = 0x5a; int SCAN_BREAK = 0xf0;
int breakActive = 0;
int clockPin = AL0; // Clock is only output. For BPW5006 Port 1
int dataPin = AL1; // The data pin is bi-directional For BPW5006 Port 1
//int clockPin = AL2; // Clock is only output. For BPW5006 Port 2
//int dataPin = AL3; // The data pin is bi-directional For BPW5006 Port 2
// But at the moment we are only interested in receiving   
int ledPin = AH7;  // When a SCAN_ENTER scancode is received the LED blink
int clockValue = 0; byte dataValue;
//byte scanCodes[10] = {0x45,0x16,0x1e,0x26,0x25,0x2e,0x36,0x3d,0x3e,0x46}; char characters[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
byte scanCodes[51] = {0x70, 0x69, 0x72, 0x7a, 0x6b, 0x73, 0x74, 0x6c, 0x75, 0x7d, 0x1c, 0x32, 0x21, 0x23, 0x24, 0x2b, 0x34, 0x33, 0x43, 0x3b, 0x42, 0x4b, 0x3a, 0x31, 0x44, 0x4d, 0x15, 0x2d, 0x1b, 0x2c, 0x3c, 0x2a, 0x1d, 0x22, 0x35, 0x1a, 0x16, 0x1e, 0x26, 0x25, 0x2e, 0x36, 0x3d, 0x3e, 0x46, 0x45, 0x29, 0x41, 0x49, 0x4a, 0X5A}; 
char characters[51] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' ', ',', '.', '/', 13};
int quantityCodes = 51;
char buffer[64] = {};	// This saves the characters (for now only numbers) 
int bufferPos = 0; 
int bufferLength = 64;


void setup() {
	pinMode(dataPin, INPUT);                                               
	pinMode(clockPin, INPUT);                                              
	pinMode(ledPin, OUTPUT);                                               
	Serial.begin(9600);                                                    
}

void loop() {
	dataValue = dataRead();                                                
	// If there is a break code, skip the next byte                        
	if (dataValue == SCAN_BREAK) {                                         
		breakActive = 1;                                                     
	}                                                                      
	// Translate the scan codes to numbers                                 
	// If there is a match, store it to the buffer                         
	for (int i = 0; i < quantityCodes; i++) {      	                       
		byte temp = scanCodes[i];                                            
		if(temp == dataValue){                                               
			if(!breakActive == 1){                                             
				buffer[bufferPos] = characters[i];                               
				bufferPos++;                                                     
			}                                                                  
		}                                                                    
	}                                                                      
	//Serial.print('*'); // Output an asterix for every byte               
	// Print the buffer if SCAN_ENTER is pressed.                          
	if(dataValue == SCAN_ENTER){                                           
		Serial.print("\nbuffer: ");                                          
		// Read the buffer                                                   
		int i=0;																		                         
		if (buffer[i] != 0) {                                                
			while(buffer[i] != 0) {                                            
				Serial.print( buffer[i] );                                       
				buffer[i] = 0;                                                   
				i++;                                                             
			}                                                                  
		}                                                                    
		Serial.println(" [Enter]");                                          
		bufferPos = 0;                                                       
		// Blink the LED	                                                   
		digitalWrite(ledPin, HIGH);                                          
		delay(300);                                                          
		digitalWrite(ledPin, LOW);                                           
	}                                                                      
	// Reset the SCAN_BREAK state if the byte was a normal one             
	if(dataValue != SCAN_BREAK){                                           
		breakActive = 0;                                                     
	}                                                                      
	dataValue = 0;                                                         
}

int dataRead() {
	byte val = 0;                                                          
	// Skip start state and start bit                                      
	while (digitalRead(clockPin));  // Wait for LOW.                       
	// clock is high when idle                                             
	while (!digitalRead(clockPin)); // Wait for HIGH.                      
	while (digitalRead(clockPin));  // Wait for LOW.                       
	for (int offset = 0; offset < 8; offset++) {                           
		while (digitalRead(clockPin));         // Wait for LOW               
		val |= digitalRead(dataPin) << offset; // Add to byte                
		while (!digitalRead(clockPin));        // Wait for HIGH              
	}                                                                      
// Skipping parity and stop bits down here.                            
	while (digitalRead(clockPin));           // Wait for LOW.              
	while (!digitalRead(clockPin));          // Wait for HIGH.             
	while (digitalRead(clockPin));           // Wait for LOW.              
	while (!digitalRead(clockPin));          // Wait for HIGH.             
	return val;                                                            
}
