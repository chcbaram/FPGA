/*
  Gadget Factory
  Papilio One VGA LCD Example
  
 VGA Wing Introduction:
 http://papilio.cc/index.php?n=Papilio.VGAWingIntroduction
 
 Hardware:
 If using an Arcade MegaWing or a LogicStart MegaWing then select the appropriate "Hyperion" board type in the ZAP IDE.
 If using a VGA Wing or some other device then use the "Hyperion" pinselect board type in the ZAP IDE and be sure to uncomment "setup_pin_select" in setup.
 
 ZPUino Variant:
 Hyperion "The Lord of Light" with HQVGA adapter is needed for this example sketch

 created 2012
 by Jack Gassett
 http://www.gadgetfactory.net
 
 This example code is in the public domain.
 */
#include <VGA.h>
#include <VGALiquidCrystal.h>

VGALiquidCrystal lcd(1,1,1,1,1,1);

#define HSync WING_A_1
#define VSync WING_A_0
#define Red0 WING_A_7
#define Red1 WING_A_5
#define Green0 WING_A_6
#define Green1 WING_A_4
#define Blue0 WING_A_2
#define Blue1 WING_A_3

void setup_pin_select() {
  pinMode(HSync,OUTPUT);
  digitalWrite(HSync,HIGH);
  outputPinForFunction(HSync, 15);
  pinModePPS(HSync, HIGH);

  pinMode(VSync,OUTPUT);
  digitalWrite(VSync,HIGH);
  outputPinForFunction(VSync, 14);
  pinModePPS(VSync, HIGH);   
  
  pinMode(Red0,OUTPUT);
  digitalWrite(Red0,HIGH);
  outputPinForFunction(Red0, 9);
  pinModePPS(Red0, HIGH);   

  pinMode(Red1,OUTPUT);
  digitalWrite(Red1,HIGH);
  outputPinForFunction(Red1, 8);
  pinModePPS(Red1, HIGH); 
  
  pinMode(Green0,OUTPUT);
  digitalWrite(Green0,HIGH);
  outputPinForFunction(Green0, 11);
  pinModePPS(Green0, HIGH); 

  pinMode(Green1,OUTPUT);
  digitalWrite(Green1,HIGH);
  outputPinForFunction(Green1, 10);
  pinModePPS(Green1, HIGH); 

  pinMode(Blue0,OUTPUT);
  digitalWrite(Blue0,HIGH);
  outputPinForFunction(Blue0, 13);
  pinModePPS(Blue0, HIGH); 

  pinMode(Blue1,OUTPUT);
  digitalWrite(Blue1,HIGH);
  outputPinForFunction(Blue1, 12);
  pinModePPS(Blue1, HIGH);     
}

void setup() {
  
  //Uncomment this if you are using the pinselect variant
  setup_pin_select();
  VGA.clear();

 // set up the LCD's number of columns and rows:
 lcd.begin(16,2);
 // clear the LCD screen:
 lcd.clear();
 
 lcd.setCursor(0,0);
 lcd.print("PAPILIO LCD DEMO");
 lcd.setCursor(1,1);
 lcd.print("ZPUINO INSIDE!");
 delay(3000); 
}

void loop() {
 lcd.scrollDisplayLeft();
 delay(100);
}
