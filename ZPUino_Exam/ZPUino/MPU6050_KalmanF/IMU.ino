
#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <math.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter



#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf


MPU6050 accelgyro;

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;



/* IMU Data */
float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
int16_t tempRaw;

int16_t ax, ay, az;
int16_t gx, gy, gz;


float gyroXangle, gyroYangle; // Angle calculate using the gyro only
float compAngleX, compAngleY; // Calculated angle using a complementary filter
float kalAngleX , kalAngleY;  // Calculated angle using a Kalman filter




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


	delay(100); // Wait for sensor to stabilize

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	accX = ax;
	accY = ay;
	accZ = az;

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
    float roll  = atan2f(accY, accZ) * RAD_TO_DEG;
  	float pitch = atanf(-accX / sqrtf(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  	float roll  = atanf(accY / sqrtf(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  	float pitch = atan2f(-accX, accZ) * RAD_TO_DEG;
#endif

  	kalmanX.setAngle(roll); // Set starting angle
  	kalmanY.setAngle(pitch);
  	gyroXangle = roll;
  	gyroYangle = pitch;
  	compAngleX = roll;
  	compAngleY = pitch;

	pinMode(LED_PIN, OUTPUT);
	
	timer = micros();
}


void loop() 
{
	unsigned long startingTime;
	int  ImuTime;
	int  FilterTime;
	int  SendTime;


	startingTime = micros();

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	ImuTime = micros() - startingTime;
	startingTime = micros();

	accX = ax;
  	accY = ay;
  	accZ = az;
  	
  	tempRaw = accelgyro.getTemperature();
  	
  	gyroX = gx;
  	gyroY = gy;
  	gyroZ = gz;

	float dt = (float)(micros() - timer) / 1000000; // Calculate delta time
	timer = micros();

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  float roll  = atan2f(accY, accZ) * RAD_TO_DEG;
  float pitch = atanf(-accX / sqrtf(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  float roll  = atanf(accY / sqrtf(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  float pitch = atan2f(-accX, accZ) * RAD_TO_DEG;
#endif



  float gyroXrate = gyroX / 131.0; // Convert to deg/s
  float gyroYrate = gyroY / 131.0; // Convert to deg/s


#ifdef RESTRICT_PITCH
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } 
  else
  {
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
  }
  if (abs(kalAngleX) > 90)
  {
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
  }
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
#else
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } 
  else
  {
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
  }
  if (abs(kalAngleY) > 90)
  {
    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
  }
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
#endif


  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
  //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;



	FilterTime = micros() - startingTime;
	startingTime = micros();



	SendTime = micros() - startingTime;
}
