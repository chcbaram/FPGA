
#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <math.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter
#include "IMU.h"

#define LED_PIN 1



IMU_Class IMU;



HardwareSerial Serial1(10); /* 1st instance/slot */


void setup() 
{
	Serial.begin(115200);
	Serial1.begin(115200);


	IMU.begin();


	pinMode(LED_PIN, OUTPUT);
}

void loop() 
{
	unsigned long startingTime;
	int  ImuTime;
	int  FilterTime;
	int  SendTime;


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
}
