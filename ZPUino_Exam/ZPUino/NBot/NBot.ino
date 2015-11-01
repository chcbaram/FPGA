
#include <I2C.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "IMU.h"
#include "SPI.h"
#include "GLCD.h"
#include "NEncoder.h"
#include "NMotor.h"

#include <math.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter


#define LED_PIN 1


#define ISR_SPEED	100 	// 100ms




int32_t ENC_CNT_A;
int32_t ENC_CNT_B;
int32_t TMR_CNT = 0;


int32_t backToSpot = 1;
static const uint16_t zoneA = 8000 * 4;
static const uint16_t zoneB = 4000 * 4;
static const uint16_t zoneC = 1000 * 4;
static const float positionScaleA = 600.0f * 4.0f; // One resolution is 1856 pulses per encoder
static const float positionScaleB = 800.0f * 4.0f;
static const float positionScaleC = 1000.0f * 4.0f;
static const float positionScaleD = 500.0f * 4.0f;
static const float velocityScaleMove = 70.0f * 4.0f;
static const float velocityScaleStop = 60.0f * 4.0f;
static const float velocityScaleTurning = 70.0f * 4.0f;

static int32_t lastWheelPosition; 		// Used to calculate the wheel velocity
static int32_t wheelVelocity;     		// Wheel velocity based on encoder readings
static int32_t targetPosition = 0;    	// The encoder position the robot should be at

static bool steerStop = true; 			// Stop by default
static bool stopped; 					// This is used to set a new target position after braking

static bool layingDown = true; 			// Use to indicate if the robot is laying down

static float targetOffset = 0.0f; 		// Offset for going forward and backward
static float turningOffset = 0.0f; 		// Offset for turning left and right

float P = 6.0f;
float I = 0.0f;
float D = 0.0f;
float targetAngle = 0; // Resting angle of the robot

static float lastError; // Store last angle error
static float iTerm; // Store iTerm
static float lastRestAngle; // Used to limit the new restAngle if it's much larger than the previous one



IMU_Class  			IMU;
GLCD_Class 			GLCD;
NEncoder_Class		ENCODER;
C_NMotor			MOTOR;


HardwareSerial Serial1(10); 



//-- 내부 함수 
//
void setup_timer_isr( void );
int32_t getWheelsPosition( void );
void stopAndReset();
void updatePID(float restAngle, float offset, float turning, float dt);


void setup() 
{
	Serial.begin(115200);
	Serial1.begin(115200);


	IMU.begin();
	GLCD.begin();
	ENCODER.begin();
	MOTOR.begin();

	GLCD.setCursor(0,0); GLCD.print("NBot Robot 1.0");

	pinMode(LED_PIN, OUTPUT);

	stopAndReset();
	setup_timer_isr();
}



void loop() 
{
	unsigned long startingTime;
	int  ImuTime;
	int  FilterTime;
	int  SendTime;
	char str[100];
	int  timer;
	static int  pidTimer;


	startingTime = micros();
	IMU.loop();
	FilterTime = micros() - startingTime;

	startingTime = micros();


	SendTime = micros() - startingTime;


  	GLCD.setCursor(0,2); GLCD.print("Roll        : "); GLCD.print(IMU.kalAngleX);
  	GLCD.setCursor(0,3); GLCD.print("Pitch       : "); GLCD.print(IMU.kalAngleY);

  	GLCD.setCursor(0,5); GLCD.print("FiltterTime : "); GLCD.print(FilterTime/1000.); GLCD.print(" ms");

  	ENC_CNT_A = ENCODER.get_left_cnt();
  	ENC_CNT_B = ENCODER.get_right_cnt();

  	GLCD.setCursor(0,8); GLCD.print("ENC L : "); GLCD.print(getWheelsPosition()); GLCD.print("     ");
	GLCD.setCursor(0,9); GLCD.print("ENC R : "); GLCD.print(ENC_CNT_B); GLCD.print("     ");

	

	timer = micros();
	updatePID( targetAngle, targetOffset, turningOffset, (float)(timer - pidTimer) / 1000000.0f);
	pidTimer = timer;

	//MOTOR.Set_Pwm( 50, 50 );

}




/*---------------------------------------------------------------------------
     TITLE   : _zpu_interrupt
     WORK    : 
     		   1ms interrupt
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void _zpu_interrupt( void )
{
	static uint32_t cnt = 0;
	

	if( TMR0CTL & (1<<TCTLIF) )
	{
		//GLCD.setCursor(0,12); GLCD.print(cnt);

		if( cnt%1000 == 0 )
		{
			TMR_CNT++;
		}

		if( cnt%100 == 0 )
		{
			int32_t wheelPosition = getWheelsPosition();
			wheelVelocity = wheelPosition - lastWheelPosition;
			lastWheelPosition = wheelPosition;

			if (abs(wheelVelocity) <= 40 && !stopped) 
			{ 	// Set new targetPosition if braking
				targetPosition = wheelPosition;
				stopped = true;
    		}
		}

		cnt++;

		TMR0CTL &= ~(1<<TCTLIF);	
	}
}



/*---------------------------------------------------------------------------
     TITLE   : setup_timer_isr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void setup_timer_isr( void )
{
	uint32_t freq = 1000;

	TMR0CNT = 0;
	TMR0CMP = (CLK_FREQ / (freq*4)) - 1;
	TMR0CTL = (1<<TCTLENA) | (1<<TCTLCCM) | (1<<TCTLDIR) | (1<<TCTLIEN)
			| (0<<TCTLCP2) | (1<<TCTLCP1) | (0<<TCTLCP0); // 96M/4 

	INTRMASK = (1<<INTRLINE_TIMER0);
	INTRCTL  = (1<<0);
}



int32_t getWheelsPosition( void )
{
	return (ENCODER.get_left_cnt()+ENCODER.get_right_cnt());
}


void stopAndReset() 
{
	MOTOR.Set_Pwm( 0, 0 );
	lastError = 0;
  	iTerm = 0;
	targetPosition = getWheelsPosition();
	lastRestAngle = targetAngle;
}


void updatePID(float restAngle, float offset, float turning, float dt) 
{

	/* Brake */
	if (steerStop) 
	{
	    int32_t wheelPosition = getWheelsPosition();
	    int32_t positionError = wheelPosition - targetPosition;

	    if (backToSpot) 
	    {
			if (abs(positionError) > zoneA) 	// Inside zone A
				restAngle -= (float)positionError / positionScaleA;
			else if (abs(positionError) > zoneB) // Inside zone B
				restAngle -= (float)positionError / positionScaleB;
			else if (abs(positionError) > zoneC) // Inside zone C
				restAngle -= (float)positionError / positionScaleC;
			else // Inside zone D
				restAngle -= (float)positionError / positionScaleD;
	    } else 
	    {
	      if (abs(positionError) < zoneC)
	        restAngle -= (float)positionError / positionScaleD;
	      else
	        targetPosition = wheelPosition;
	    }
	    restAngle -= (float)wheelVelocity / velocityScaleStop;

	    restAngle = constrain(restAngle, targetAngle - 10, targetAngle + 10); // Limit rest Angle
	}
	/* Drive forward and backward */
  	else 
  	{
		if ((offset > 0 && wheelVelocity < 0) || (offset < 0 && wheelVelocity > 0) || offset == 0) // Scale down offset at high speed - wheel velocity is negative when driving forward and positive when driving backward
		{
			offset += (float)wheelVelocity / velocityScaleMove; // We will always compensate if the offset is 0, but steerStop is not set
		}
		restAngle -= offset;
	}

	restAngle = constrain(restAngle, lastRestAngle - 1, lastRestAngle + 1); // Don't change restAngle with more than 1 degree in each loop
	lastRestAngle = restAngle;


	restAngle = 0;

	/* Update PID values */
  	float error = -(restAngle - (-IMU.kalAngleX));
	float pTerm = P * error;
	iTerm += I * 100.0f * error * dt; // Multiplication with Ki is done before integration limit, to make it independent from integration limit value
	iTerm = constrain(iTerm, -100.0f, 100.0f); // Limit the integrated error - prevents windup
	float dTerm = (D / 100.0f) * (error - lastError) / dt;
	lastError = error;
	float PIDValue = pTerm + iTerm + dTerm;

	/* Steer robot sideways */
	if (turning < 0) 
	{ 	// Left
		turning += abs((float)wheelVelocity / velocityScaleTurning); // Scale down at high speed
		if (turning > 0)
		{
			turning = 0;
		}
	}
	else if (turning > 0) 
	{ 	// Right
		turning -= abs((float)wheelVelocity / velocityScaleTurning); // Scale down at high speed
		if (turning < 0)
		{
			turning = 0;
		}
	}

	float PIDLeft  = PIDValue + turning;
	float PIDRight = PIDValue - turning;



	if( PIDLeft  > 100 ) PIDLeft  = 100;
	if( PIDRight > 100 ) PIDRight = 100;


	GLCD.setCursor(0,11); GLCD.print("restAngle : "); GLCD.print(restAngle); GLCD.print("     ");
	GLCD.setCursor(0,12); GLCD.print("PIDValue  : "); GLCD.print(PIDValue); GLCD.print("     ");


	MOTOR.Set_Pwm( PIDLeft*255/100, PIDRight*255/100 );

}

