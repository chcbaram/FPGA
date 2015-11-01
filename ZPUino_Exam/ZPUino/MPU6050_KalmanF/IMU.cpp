

#include "IMU.h"


#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf








IMU_Class::IMU_Class(void)
{
    IsInit = false;
}


bool IMU_Class::begin() 
{
    bool ret = true;


    I2c.begin();
   

	accelgyro.initialize();

    if( accelgyro.testConnection() == false) 
    {
        return false;
    }


	delay(100); // Wait for sensor to stabilize

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	accX = ax;
	accY = ay;
	accZ = az;

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
    roll  = atan2f(accY, accZ) * RAD_TO_DEG;
  	pitch = atanf(-accX / sqrtf(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  	roll  = atanf(accY / sqrtf(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  	pitch = atan2f(-accX, accZ) * RAD_TO_DEG;
#endif

  	kalmanX.setAngle(roll); // Set starting angle
  	kalmanY.setAngle(pitch);
  	gyroXangle = roll;
  	gyroYangle = pitch;
  	compAngleX = roll;
  	compAngleY = pitch;
	
	timer = micros();
}

void IMU_Class::loop() 
{
	unsigned long startingTime;


	startingTime = micros();

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	TimeSensor = micros() - startingTime;
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
    roll  = atan2f(accY, accZ) * RAD_TO_DEG;
    pitch = atanf(-accX / sqrtf(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
    roll  = atanf(accY / sqrtf(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    pitch = atan2f(-accX, accZ) * RAD_TO_DEG;
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


    

	TimeFilter = micros() - startingTime;
	startingTime = micros();
}
