  /*
  LiquidCrystal Library - TextDirection
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch demonstrates how to use leftToRight() and rightToLeft()
 to move the cursor.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 25 July 2009
 by David A. Mellis
 modified 9/30/2010 - For Papilio Platform BPW5003 Wing
 by Jack Gassett (http://www.GadgetFactory.net) 
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 
 */

// include the library code:
#include <LiquidCrystal.h>
int thisChar = 'a';

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(AH6, AH5, AH4, AH3, AH2, AH1, AH0);		//Connect LCD Wing to AH. Change all instances of AH to your desired Wing Slot.
LiquidCrystal lcd(26, 25, 24, 23, 22, 21, 20);		//Uncomment this for the RetroCade MegaWing

void setup() {
//  pinMode(AH7, OUTPUT);     //Set contrast to GND for AH WingSlot
//  digitalWrite(AH7, LOW);   //Set contrast to GND for AH WingSlot
  pinMode(46, OUTPUT);     //Set contrast to GND for RetroCade MegaWing
  digitalWrite(46, LOW);   //Set contrast to GND for RetroCade MegaWing
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // turn on the cursor:
  lcd.cursor();
  Serial.begin(9600);
}

void loop() {
  // reverse directions at 'm':
  if (thisChar == 'm') {
    // go right for the next letter
    lcd.rightToLeft(); 
  }
  // reverse again at 's':
  if (thisChar == 's') {
    // go left for the next letter
    lcd.leftToRight(); 
  }
  // reset at 'z':
  if (thisChar > 'z') {
    // go to (0,0):
    lcd.home(); 
    // start again at 0
    thisChar = 'a';
  }
  // print the character
  lcd.print(thisChar, BYTE);
  // wait a second:
  delay(1000);
  // increment the letter:
  thisChar++;
}








