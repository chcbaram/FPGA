//----------------------------------------------------------------------------
//    프로그램명 	: BiRover Motor
//
//    만든이     	: Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: BiRover_Motor.cpp
//----------------------------------------------------------------------------



#include "NMotor.h"




#define LEFT_PIN_DIR_0			25
#define LEFT_PIN_DIR_1			26
#define LEFT_PIN_PWM			24


#define RIGHT_PIN_DIR_0			28
#define RIGHT_PIN_DIR_1			29
#define RIGHT_PIN_PWM			30


#define STBY_PIN				27


#define MOTOR_FW				0
#define MOTOR_BW				1
#define MOTOR_STOP				2






/*---------------------------------------------------------------------------
     TITLE   : C_NMotor
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
C_NMotor::C_NMotor( void )
{
	Pwm_Left  = 0;
	Pwm_Right = 0;
}





/*---------------------------------------------------------------------------
     TITLE   : setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void C_NMotor::begin( void )
{
	uint32_t freq = 100000;


	//-- PWM 초기화 
	//
	TMR1CNT = 0;
	//TMR1CMP	= (CLK_FREQ / freq) - 1;
	TMR1CMP	= 255;
	TMR1CTL =   (1<<TCTLENA) | (1<<TCTLDIR) | (1<<TCTLCCM) 
	          | (0<<TCTLCP2) | (1<<TCTLCP1) | (0<<TCTLCP0); // 96M/4/256 = 

	TMR1PWMCTL(0) = 1;
	TMR1PWMCTL(1) = 1;


	//-- I/O 초기화  
	//
	pinMode( LEFT_PIN_DIR_0, OUTPUT );
	pinMode( LEFT_PIN_DIR_1, OUTPUT );
	pinMode( LEFT_PIN_PWM, OUTPUT );
	
	pinModePPS( LEFT_PIN_PWM, HIGH );
    outputPinForFunction( LEFT_PIN_PWM, 1 );



	pinMode( RIGHT_PIN_DIR_0, OUTPUT );
	pinMode( RIGHT_PIN_DIR_1, OUTPUT );
	pinMode( RIGHT_PIN_PWM, OUTPUT );

	
	pinModePPS( RIGHT_PIN_PWM, HIGH );
    outputPinForFunction( RIGHT_PIN_PWM, 2 );



	pinMode( STBY_PIN, OUTPUT );


	digitalWrite( STBY_PIN, HIGH );


	Set_DirLeftOut ( MOTOR_STOP );
	Set_DirRightOut( MOTOR_STOP );

	Set_PwmLeftOut ( 0 );
	Set_PwmRightOut( 0 );
}





/*---------------------------------------------------------------------------
     TITLE   : Set_PwmLeftOut
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void C_NMotor::Set_Pwm( int16_t PwmLeft, int16_t PwmRight )
{
	if( PwmLeft >  255 ) PwmLeft =  255;
	if( PwmLeft < -255 ) PwmLeft = -255;

	if( PwmRight >  255 ) PwmRight =  255;
	if( PwmRight < -255 ) PwmRight = -255;


	if( PwmLeft == 0 )
	{
		Set_DirLeftOut ( MOTOR_STOP );
		Set_PwmLeftOut ( 0 );		
	}
	else
	if( PwmLeft > 0 )	
	{
		Set_DirLeftOut ( MOTOR_FW );
		Set_PwmLeftOut ( PwmLeft );		
	}
	else
	{
		Set_DirLeftOut ( MOTOR_BW );
		Set_PwmLeftOut ( -PwmLeft );		
	}


	if( PwmRight == 0 )
	{
		Set_DirRightOut ( MOTOR_STOP );
		Set_PwmRightOut ( 0 );		
	}
	else
	if( PwmRight > 0 )	
	{
		Set_DirRightOut ( MOTOR_FW );
		Set_PwmRightOut ( PwmRight );		
	}
	else
	{
		Set_DirRightOut ( MOTOR_BW );
		Set_PwmRightOut ( -PwmRight );		
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Set_PwmLeftOut
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void C_NMotor::Set_PwmLeftOut( uint16_t PwmData  )
{
	if( PwmData > 255 ) PwmData = 255;

	Pwm_Left = PwmData;

	TMR1PWMLOW(0) = 0;
	TMR1PWMHIGH(0) = Pwm_Left;
}





/*---------------------------------------------------------------------------
     TITLE   : Set_PwmRighttOut
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void C_NMotor::Set_PwmRightOut( uint16_t PwmData  )
{
	if( PwmData > 255 ) PwmData = 255;

	Pwm_Right = PwmData;

	TMR1PWMLOW(1)  = 0;
	TMR1PWMHIGH(1) = Pwm_Right;
}





/*---------------------------------------------------------------------------
     TITLE   : Set_DirLeftOut
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void C_NMotor::Set_DirLeftOut( uint8_t DirData  )
{
	switch( DirData )
	{
		case MOTOR_FW:
			digitalWrite( LEFT_PIN_DIR_0, HIGH );
			digitalWrite( LEFT_PIN_DIR_1, LOW );		
			break;

		case MOTOR_BW:
			digitalWrite( LEFT_PIN_DIR_0, LOW );
			digitalWrite( LEFT_PIN_DIR_1, HIGH );		
			break;

		case MOTOR_STOP:
			digitalWrite( LEFT_PIN_DIR_0, LOW );
			digitalWrite( LEFT_PIN_DIR_1, LOW );		
			break;

		default:
			digitalWrite( LEFT_PIN_DIR_0, LOW );
			digitalWrite( LEFT_PIN_DIR_1, LOW );		
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Set_DirRightOut
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void C_NMotor::Set_DirRightOut( uint8_t DirData  )
{
	switch( DirData )
	{
		case MOTOR_FW:
			digitalWrite( RIGHT_PIN_DIR_0, HIGH );
			digitalWrite( RIGHT_PIN_DIR_1, LOW );		
			break;

		case MOTOR_BW:
			digitalWrite( RIGHT_PIN_DIR_0, LOW );
			digitalWrite( RIGHT_PIN_DIR_1, HIGH );		
			break;

		case MOTOR_STOP:
			digitalWrite( RIGHT_PIN_DIR_0, LOW );
			digitalWrite( RIGHT_PIN_DIR_1, LOW );		
			break;

		default:
			digitalWrite( RIGHT_PIN_DIR_0, LOW );
			digitalWrite( RIGHT_PIN_DIR_1, LOW );		
			break;
	}
}
