/*
  LiquidCrystal Library - Autoscroll
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.
 
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

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(AH6, AH5, AH4, AH3, AH2, AH1, AH0);		//Connect LCD Wing to AH. Change all instances of AH to your desired Wing Slot.
LiquidCrystal lcd(26, 25, 24, 23, 22, 21, 20);		//Uncomment this for the RetroCade MegaWing

void setup() {
//  pinMode(AH7, OUTPUT);     //Set contrast to GND for AH WingSlot
//  digitalWrite(AH7, LOW);   //Set contrast to GND for AH WingSlot
  pinMode(46, OUTPUT);     //Set contrast to GND for RetroCade MegaWing
  digitalWrite(46, LOW);   //Set contrast to GND for RetroCade MegaWing
  lcd.begin(16,2);
}

void loop() {
  // set the cursor to (0,0):
  lcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
   lcd.print(thisChar);
   delay(500);
  }

  // set the cursor to (16,1):
  lcd.setCursor(16,1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }
  // turn off automatic scrolling
  lcd.noAutoscroll();
  
  // clear screen for the next loop:
  lcd.clear();
}

