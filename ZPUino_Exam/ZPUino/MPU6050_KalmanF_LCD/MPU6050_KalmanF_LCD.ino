
#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "IMU.h"
#include "SPI.h"
#include "GLCD.h"


#include <math.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter


#define LED_PIN 1



IMU_Class  IMU;
GLCD_Class GLCD;


HardwareSerial Serial1(10); /* 1st instance/slot */


void setup() 
{
	Serial.begin(115200);
	Serial1.begin(115200);


	IMU.begin();
	GLCD.begin();

	GLCD.setCursor(0,0); GLCD.print("MPU6050 & Kalman Filter");

	pinMode(LED_PIN, OUTPUT);
}

void loop() 
{
	unsigned long startingTime;
	int  ImuTime;
	int  FilterTime;
	int  SendTime;
	char str[100];


	startingTime = micros();
	IMU.loop();
	FilterTime = micros() - startingTime;

	startingTime = micros();

	//Serial1.print(IMU.roll);	   Serial1.print("\t");
  	//Serial1.print(IMU.gyroXangle); Serial1.print("\t");
  	//Serial1.print(IMU.compAngleX); Serial1.print("\t");
  	Serial1.print(IMU.kalAngleX);  Serial1.print("\t");

  	Serial1.print("\t");

  	//Serial1.print(IMU.pitch); 	   Serial1.print("\t");
  	//Serial1.print(IMU.gyroYangle); Serial1.print("\t");
  	//Serial1.print(IMU.compAngleY); Serial1.print("\t");
  	Serial1.print(IMU.kalAngleY);  Serial1.print("\t");
  	Serial1.print("\r\n");

	SendTime = micros() - startingTime;

  	Serial.print("ImuTime     : "); Serial.println(ImuTime);
  	Serial.print("FiltterTime : "); Serial.println(FilterTime);
  	Serial.print("SendTime    : "); Serial.println(SendTime);
  	//delay(2);
	//sprintf(str,"Roll : %d", IMU.kalAngleX);
  	//GLCD.print(0,1,)
  	GLCD.setCursor(0,2); GLCD.print("Roll        : "); GLCD.print(IMU.kalAngleX);
  	GLCD.setCursor(0,3); GLCD.print("Pitch       : "); GLCD.print(IMU.kalAngleY);

  	GLCD.setCursor(0,5); GLCD.print("FiltterTime : "); GLCD.print(FilterTime/1000.); GLCD.print(" ms");

}
