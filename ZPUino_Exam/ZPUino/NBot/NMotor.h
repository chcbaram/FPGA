//----------------------------------------------------------------------------
//    프로그램명 	: Motor
//
//    만든이     	: Cho Han Cheol 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:	 
//
//    파일명     	: NMotor.h
//----------------------------------------------------------------------------
#ifndef _NMOTOR_H_
#define _NMOTOR_H_



#include <Arduino.h>
#include <inttypes.h>



class C_NMotor
{
public:
	C_NMotor( void );


	void begin( void );

	void Set_Pwm( int16_t PwmLeft, int16_t PwmRight );



private:
	int16_t  Pwm_Left;
	int16_t  Pwm_Right;

	void Set_PwmLeftOut( uint16_t PwmData  );
	void Set_PwmRightOut( uint16_t PwmData  );

	void Set_DirLeftOut( uint8_t DirData  );
	void Set_DirRightOut( uint8_t DirData  );

};


#endif
