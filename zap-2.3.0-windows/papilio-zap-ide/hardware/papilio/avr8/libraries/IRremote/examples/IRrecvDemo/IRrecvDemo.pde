/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * Modified for the Papilio Platform 9/28/2010 by Jack Gassett
 * Documentation for the BPW5008 Infrared Wing is located at (http://gadgetforge.gadgetfactory.net/gf/project/bpw5008-ir/)
  */

#include <IRremote.h>

//By default BPW5008 Infrared Wing is connected to AL
int RECV_PIN = AL4;			//Use AL4, AH4, BL4, BH4, CL4, or CH4 depending on which Wing Slot you want the Papilio Platforms BPW5008 Infrared Wing connected to. Connects BPW5008-TX1 to AL by default.


IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
