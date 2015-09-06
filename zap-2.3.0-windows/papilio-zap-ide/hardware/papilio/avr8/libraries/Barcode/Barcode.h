/*
  Barcode.h - Library for taking input from PS/2 Barcode scanners.
  Created by Benjamin Maus	( benjamin.maus <at> allesblinkt.com ), 2007.
  Ported to a library by Jack Gassett (jack.gassett@gadgetfactory.net), 2010
  Released into the public domain.
*/
#ifndef Barcode_h
#define Barcode_h

#include "WProgram.h"
#include <SdFat.h>
#include <SdFatUtil.h>
#define SCAN_ENTER 0x5a
#define SCAN_BREAK 0xf0
const byte scanCodes[51] = {0x70, 0x69, 0x72, 0x7a, 0x6b, 0x73, 0x74, 0x6c, 0x75, 0x7d, 0x1c, 0x32, 0x21, 0x23, 0x24, 0x2b, 0x34, 0x33, 0x43, 0x3b, 0x42, 0x4b, 0x3a, 0x31, 0x44, 0x4d, 0x15, 0x2d, 0x1b, 0x2c, 0x3c, 0x2a, 0x1d, 0x22, 0x35, 0x1a, 0x16, 0x1e, 0x26, 0x25, 0x2e, 0x36, 0x3d, 0x3e, 0x46, 0x45, 0x29, 0x41, 0x49, 0x4a};;
const char characters[51] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' ', ',', '.', '/'};;
	
class Barcode
{
  public:
    Barcode(int clockPin, int dataPin, int ledPin, int buttonPin);
	void scan();
	char * scannedCode;

  private:
	void dataRead();  
    int _clockPin;
	int _dataPin;
	int _ledPin;
	int _buttonPin;	
	int breakActive;
	int clockValue; 
	int quantityCodes;
	int bufferPos; 
	int bufferLength;	
	byte dataValue;
	int buttonState;
	char buffer[64];	// This saves the characters
};

#endif
