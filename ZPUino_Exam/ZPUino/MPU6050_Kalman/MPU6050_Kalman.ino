
#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <math.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter


#define LED_PIN 1


#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf


MPU6050 accelgyro;


Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

int16_t ax, ay, az;
int16_t gx, gy, gz;


double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;


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


	delay(100); // Wait for sensor to stabilize

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	accX = ax;
	accY = ay;
	accZ = az;

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
	double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  	double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  	double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  	double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
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
	int  KalmanTime;


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

	double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
	timer = micros();

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif



  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s


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

  /* Print Data */
#if 0 // Set to 1 to activate
  Serial.print(accX); Serial.print("\t");
  Serial.print(accY); Serial.print("\t");
  Serial.print(accZ); Serial.print("\t");

  Serial.print(gyroX); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ); Serial.print("\t");

  Serial.print("\t");
#endif

	KalmanTime = micros() - startingTime;

	Serial1.print(roll);		 Serial1.print("\t");
  	Serial1.print(gyroXangle); Serial1.print("\t");
  	Serial1.print(compAngleX); Serial1.print("\t");
  	Serial1.print(kalAngleX);  Serial1.print("\t");

  	Serial1.print("\t");

  	Serial1.print(pitch); 	 Serial1.print("\t");
  	Serial1.print(gyroYangle); Serial1.print("\t");
  	Serial1.print(compAngleY); Serial1.print("\t");
  	Serial1.print(kalAngleY);  Serial1.print("\t");

#if 0 // Set to 1 to print the temperature
  Serial.print("\t");

  double temperature = (double)tempRaw / 340.0 + 36.53;
  Serial.print(temperature); Serial.print("\t");
#endif

  	Serial1.print("\r\n");

  	Serial.print("ImuTime    : "); Serial.println(ImuTime);
  	Serial.print("KalmanTime : "); Serial.println(KalmanTime);
  	delay(2);
}
