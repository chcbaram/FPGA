#include "SPI.h"
#include "Wire.h"
#include "GLCD.h"
#include "OV7670.h"


//#define USE_COLOR	
#define USE_GRAY	



OV7670_Class 	OV7670;
GLCD_Class 		GLCD;


int led = 1;





uint16_t  ImageBuf[320*240];
uint16_t  DspBuf[320*240];
uint16_t  ResultBuf[320*240];

// the setup routine runs once when you press reset:
void setup() 
{
	bool ret;
	u32 i;

	Serial.begin(115200);

	// initialize the digital pin as an output.
	pinMode(led, OUTPUT);     

	ret = OV7670.begin( 17, 16, 18, 19 );
	Serial.println(ret);

	#ifdef USE_GRAY
	OV7670.setColorSpace(YUV422);
	#endif

	GLCD.begin();

	GLCD.setCursor(0,0); GLCD.print("OV7670 Test");

	for( i=0; i<320*240; i++ )
	{
		ImageBuf[i] = 0xFF;
	}

	GLCD.drawImage( 0, 60, 160, 120, ImageBuf );

}

// the loop routine runs over and over again forever:
void loop() 
{
	//display_color();
	display_gray();

}





void display_gray() 
{
	u32 i;
	u16 r,g,b;
	u16 h,l;
	int FrameTime;
	uint32_t *pImage;
	unsigned long startingTime;
	uint16_t pixel;


	static u16 Cnt = 0;


	startingTime = micros();


	
	OV7670.read_image( ImageBuf, 80*60, 2 );


	for( i=0; i<80*60; i++ )
	{
		pixel = (ImageBuf[i]>>8);
		ImageBuf[i] = pixel;
	}

	GLCD.drawGray( 0, 60, 80, 60, ImageBuf );

	for( i=0; i<80*60; i++ )
	{
		if( ImageBuf[i] < 30 ) ResultBuf[i] = 255;
		else                    ResultBuf[i] = 0;
	}

	/*
	for( i=0; i<80*60; i++ )
	{
		pixel = ResultBuf[i];

		r = pixel>>3;
		g = pixel>>2;
		b = pixel>>3;

		DspBuf[i] = r<<11 | g<<5 | b<<0;
	}
	*/

	GLCD.drawGray( 0+80, 60, 80, 60, ResultBuf );

	FrameTime = micros() - startingTime;	

	GLCD.setCursor(0,1); GLCD.print(1000/(FrameTime/1000)); GLCD.print(" frame/sec");

}




void display_color() 
{
	u32 i;
	u16 r,g,b;
	u16 h,l;
	int FrameTime;
	uint32_t *pImage;
	unsigned long startingTime;
	uint16_t pixel;

	static u16 Cnt = 0;


	startingTime = micros();

	OV7670.read_image( ImageBuf, 160*120 );

	GLCD.drawImage( 0, 60, 160, 120, ImageBuf );

	FrameTime = micros() - startingTime;	

	GLCD.setCursor(0,1); GLCD.print(1000/(FrameTime/1000)); GLCD.print(" frame/sec");

}