
#include "Wire.h"
#include "delay.h"
#include "NEncoder.h"




#define REG_ENCODER_A_STATUS		REGISTER(IO_SLOT(11), 0)
#define REG_ENCODER_A_CNT			REGISTER(IO_SLOT(11), 1)
#define REG_ENCODER_B_STATUS		REGISTER(IO_SLOT(11), 2)
#define REG_ENCODER_B_CNT			REGISTER(IO_SLOT(11), 3)






NEncoder_Class::NEncoder_Class(void)
{
	IsInit = false;
}


bool NEncoder_Class::begin( void )
{
	bool ret = true;



	IsInit = true;


	enc_offset[0] = REG_ENCODER_A_CNT;
	enc_offset[1] = REG_ENCODER_B_CNT;
	
	
	return ret;
}


int32_t NEncoder_Class::get_left_cnt( void )
{
	int32_t cnt;

	cnt = REG_ENCODER_A_CNT - enc_offset[0];

	return cnt;
}


int32_t NEncoder_Class::get_right_cnt( void )
{
	int32_t cnt;

	cnt = REG_ENCODER_B_CNT - enc_offset[1];

	return -cnt;
}


