#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "Kalman.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;



// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO



#define LED_PIN 1
bool blinkState = false;

HardwareSerial Serial1(10); /* 1st instance/slot */

void setup() 
{
    Serial.begin(115200);
    Serial1.begin(115200);
    I2c.begin();
   

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values
    /*
    Serial.println("Updating internal sensor offsets...");
    // -76	-2359	1688	0	0	0
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}


void loop() 
{
  unsigned long startingTime;
  unsigned int startTag = 0xDEAD;
  
    // read raw accel/gyro measurements from device
    //startingTime = micros();
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //Serial.println(micros()-startingTime);

/*
    Serial.print("a/g:\t");
    Serial.print(ax/100); Serial.print("\t");
    Serial.print(ay/100); Serial.print("\t");
    Serial.print(az/100); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);
*/


  Serial1.write( (unsigned byte*)&startTag, 2);
  Serial1.write((unsigned byte*)&ax, 2);
  Serial1.write((unsigned byte*)&ay, 2);
  Serial1.write((unsigned byte*)&az, 2);
  Serial1.write((unsigned byte*)&gx, 2);
  Serial1.write((unsigned byte*)&gy, 2);
  Serial1.write((unsigned byte*)&gz, 2);
  

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);

    //Serial1.println("Bluetooth");    
    delay(100);
}

