/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
  *
 * Modified for the Papilio Platform 9/28/2010 by Jack Gassett
 * Documentation for the BPW5008 Infrared Wing is located at (http://gadgetforge.gadgetfactory.net/gf/project/bpw5008-ir/)
 */

#include <IRremote.h>

IRsend irsend;

void setup()
{
  //Move the PWM2 output to the appropriate pin for the Papilio Platforms BPW5008 Infrared Wing
  //By default BPW5008 Infrared Wing is connected to AL
  PWM2_LOC=AL7;	//AL7 for Wing Slot AL, AH7 for Wing Slot AH, BL7 for Wing Slot BL, BH7 for Wing Slot BH, CL7 for Wing Slot CL, CH7 for Wing Slot CH.
  Serial.begin(9600);
}

void loop() {
  if (Serial.read() != -1) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 12); // Sony TV power code
      delay(100);
    }
  }
}

