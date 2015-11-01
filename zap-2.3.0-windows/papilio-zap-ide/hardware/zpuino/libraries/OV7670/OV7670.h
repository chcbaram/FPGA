
#ifndef _OV7670_H_
#define _OV7670_H_

#include <Arduino.h>
#include <inttypes.h>


#include "OV7670_Reg.h"



class OV7670_Class {
public:

	bool IsInit;




	OV7670_Class();

	bool begin( int sda, int scl, int reset, int pwdn );

	uint16_t read_pixel( uint32_t address );
	void     read_image( uint16_t *pImage, uint32_t length );
	void     read_image( uint16_t *pImage, uint32_t length, uint8_t resize );

	bool   write_reg(uint8_t reg,uint8_t dat);
	bool   write_regs(const struct regval_list reglist[]);	
	int8_t read_reg(uint8_t reg, uint8_t *pdata);

	void  setColorSpace(enum COLORSPACE color);
	void  setRes(enum RESOLUTION res);

private:
	int pin_sda;
	int pin_scl;
	int pin_reset;
	int pin_pwdn;
};

#endif

