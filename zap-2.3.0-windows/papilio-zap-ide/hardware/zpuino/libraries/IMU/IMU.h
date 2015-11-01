
#ifndef _IMU_H_
#define _IMU_H_

#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <math.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter



class IMU_Class {
public:

	bool IsInit;

	int  TimeSensor;
	int  TimeFilter;

	float roll;
  	float pitch;
  	float yaw;


	float gyroXangle, gyroYangle; // Angle calculate using the gyro only
	float compAngleX, compAngleY; // Calculated angle using a complementary filter
	float kalAngleX, kalAngleY;   // Calculated angle using a Kalman filter  


	IMU_Class();

	bool begin(void);
    void loop(void);

    float GetAngleX(void) { return kalAngleX; };
    float GetAngleY(void) { return kalAngleY; };
    float GetAngleZ(void) { return 0; };


private:
	MPU6050 accelgyro;


	Kalman kalmanX; // Create the Kalman instances
	Kalman kalmanY;

	/* IMU Data */
	float accX, accY, accZ;
	float gyroX, gyroY, gyroZ;
	int16_t tempRaw;

	int16_t ax, ay, az;
	int16_t gx, gy, gz;

	uint32_t timer;
};

#endif