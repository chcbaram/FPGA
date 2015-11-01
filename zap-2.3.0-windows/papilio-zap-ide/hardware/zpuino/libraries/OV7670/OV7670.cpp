
#include "Wire.h"
#include "OV7670.h"
#include "delay.h"
#include "OV7670_Reg.h"


//#define _USE_VGA
//#define _USE_QVGA
#define _USE_QQVGA


#define IMAGE_WIDTH		160
#define IMAGE_HEIGHT 	120




OV7670_Class::OV7670_Class(void)
{
	IsInit = false;
}


bool OV7670_Class::begin( int sda, int scl, int reset, int pwdn )
{
	bool ret = false;


	pin_sda   = sda;
	pin_scl   = scl;
	pin_reset = reset;
	pin_pwdn  = pwdn;


	Wire.begin( (uint8_t)pin_sda, (uint8_t)pin_scl );


	pinMode( pin_reset, OUTPUT );
	pinMode( pin_pwdn,  OUTPUT );


	digitalWrite( pin_reset , HIGH );
	digitalWrite( pin_pwdn  , LOW );
	delay(50);
	digitalWrite( pin_reset , LOW );
	delay(50);
	digitalWrite( pin_reset , HIGH );
	delay(500);



	ret |= write_reg(0x12, 0x80);		//Reset the camera.
	delay(100);

	ret |= write_regs(ov7670_setup_regs);


	setRes(QQVGA);
	setColorSpace(RGB565);
	//setColorSpace(YUV422);


	IsInit = true;
	
	ret |= write_reg(REG_COM8, (1<<2) | (1<<1));	// AGC/AWB OFF

	// COLOR SETTING
	//
	
	write_reg(0x4f,0x80);
	write_reg(0x50,0x80);
	write_reg(0x51,0x00);
	write_reg(0x52,0x22);
	write_reg(0x53,0x5e);
	write_reg(0x54,0x80);
	write_reg(0x56,0x40);
	write_reg(0x58,0x9e);
	write_reg(0x59,0x88);
	write_reg(0x5a,0x88);
	write_reg(0x5b,0x44);
	write_reg(0x5c,0x67);
	write_reg(0x5d,0x49);
	write_reg(0x5e,0x0e);
	write_reg(0x69,0x00);
	write_reg(0x6a,0x40);
	write_reg(0x6b,0x0a);
	write_reg(0x6c,0x0a);
	write_reg(0x6d,0x55);
	write_reg(0x6e,0x11);
	write_reg(0x6f,0x9f);	
	write_reg(0xb0,0x84);
	
	return ret;
}


void OV7670_Class::setColorSpace(enum COLORSPACE color)
{
	switch(color){
		case YUV422:
			write_regs(yuv422_ov7670);
		break;
		case RGB565:
			write_regs(rgb565_ov7670);
			{uint8_t temp;
				read_reg(0x11, &temp);
			delay(1);
			write_reg(0x11,temp);}//according to the Linux kernel driver rgb565 PCLK needs rewriting
		break;
		case BAYER_RGB:
			write_regs(bayerRGB_ov7670);
		break;
	}
}


uint16_t OV7670_Class::read_pixel( uint32_t address )
{
	REGISTER(IO_SLOT(12), 0) = address;

	return (uint16_t)REGISTER(IO_SLOT(12), 0);	
}


void OV7670_Class::read_image( uint16_t *pImage, uint32_t length )
{
	uint32_t i;
	uint16_t pixel;
	uint16_t r,g,b;

	for( i=0; i<length; i++ )
	{		
		REGISTER(IO_SLOT(12), 0) = i;
		pixel = REGISTER(IO_SLOT(12), 0);
		
		//r = (pixel>>11) & 0x1F;
		//g = (pixel>>5)  & 0x2F;
		//b = (pixel>>0)  & 0x1F;

		//pImage[i] = r<<11 | g<<5 | b<<0;
		pImage[i] = pixel;
	}
}


void OV7670_Class::read_image( uint16_t *pImage, uint32_t length, uint8_t resize )
{
	uint32_t x,y;
	uint16_t pixel;
	uint16_t r,g,b;
	uint16_t width;
	uint16_t height;
	uint32_t i;

	i = 0;

	for( y=0; y<IMAGE_HEIGHT; y += resize )
	{
		for( x=0; x<IMAGE_WIDTH; x += resize )
		{		
			REGISTER(IO_SLOT(12), 0) = y*IMAGE_WIDTH + x;
			pixel = REGISTER(IO_SLOT(12), 0);		
			pImage[i++] = pixel;
		}
	}
}



void OV7670_Class::setRes(enum RESOLUTION res)
{
	switch(res){
		case VGA:
			write_reg(REG_COM3,0);	// REG_COM3
			write_regs(vga_ov7670);
		break;
		case QVGA:
			write_reg(REG_COM3,4);	// REG_COM3 enable scaling
			write_regs(qvga_ov7670);
		break;
		case QQVGA:
			write_reg(REG_COM3,4);	// REG_COM3 enable scaling
			write_regs(qqvga_ov7670);
		break;
	}
}

bool OV7670_Class::write_regs(const struct regval_list reglist[])
{
	bool ret = false;

	const struct regval_list *next = reglist;
	for(;;)
	{
		uint8_t reg_addr = next->reg_num;
		uint8_t reg_val = next->value;
		if((reg_addr==255)&&(reg_val==255))
			break;

		ret |= write_reg(reg_addr, reg_val);
		next++;
	}
}

bool OV7670_Class::write_reg(uint8_t reg,uint8_t dat)
{
    uint8_t status = 0;


	Wire.beginTransmission((uint8_t)OV7670_I2C_ADDRESS);
	Wire.write((uint8_t) reg); 
	Wire.write((uint8_t) dat);

	status = Wire.endTransmission();

    return status == 0;
}


int8_t OV7670_Class::read_reg(uint8_t reg, uint8_t *pdata)
{
    uint8_t status = 0;
    int length     = 1;
    int8_t count   = 0;
	uint16_t timeout = 100;
	

	uint32_t t1 = millis();


	Wire.beginTransmission((uint8_t)OV7670_I2C_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.beginTransmission((uint8_t)OV7670_I2C_ADDRESS);
	Wire.requestFrom((uint8_t)OV7670_I2C_ADDRESS, length);
        
	for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) 
	{
		pdata[0] = Wire.read();
	}       

	Wire.endTransmission();



    // check for timeout
    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1; // timeout


    return count;
}
