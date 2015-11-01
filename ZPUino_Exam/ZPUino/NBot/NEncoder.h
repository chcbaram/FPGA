
#ifndef _NENCODER_H_
#define _NENCODER_H_

#include <Arduino.h>
#include <inttypes.h>




class NEncoder_Class 
{
public:

	bool IsInit;




	NEncoder_Class();

	bool begin( void );

	int32_t get_left_cnt( void );
	int32_t get_right_cnt( void );

private:
	int32_t enc_offset[2];
	int32_t enc_cnt[2];
};

#endif

