

#include "GLCD.h"
#include "font.h"
#include "qqp_arm.h"
//#include "SPI.h"
#include "delay.h"


#define SPIADC_8BIT  0
#define SPIADC_12BIT 1


#define GLCD_RST      12
#define GLCD_RS       13
#define GLCD_SCI      14
#define GLCD_SCL      15


int GlcdSlot = 6;


uint color[]={0xf800,0x07e0,0x001f,0xffe0,0x0000,0xffff,0x07ff,0xf81f};


u16 Hw_LCD_ImgBuf[320*240];
void Hw_LCD_DspStr(u8 *s, u8 x, u8 y);

void Hw_LCD_Init( void );
void Hw_LCD_Test();
void Hw_LCD_SetRS( u8 Data );
void Hw_LCD_SetCS( u8 Data );
void Hw_LCD_WriteSPI_Byte( u8 Data );
void Hw_LCD_WR_REG(uint index);
void Hw_LCD_WR_CMD(uint index,uint val);
void Hw_LCD_WR_Data(uint val);
void Hw_LCD_CfgInit( void );
void Hw_LCD_Delay_ms( u32 delay );
void Hw_LCD_DspImg( u16 *pBuf );
void Hw_LCD_Clear(uint p);
void Hw_LCD_DspChar(u8  casc, u8 postion_x, u8 postion_y);
void Hw_LCD_DspBuf( u16 x, u16 y, u16 width, u16 height, u16 *pBuf );
void Hw_LCD_DspGrayBuf( u16 x, u16 y, u16 width, u16 height, u16 *pBuf );

void Hw_LCD_PIN_RS_HIGH( void );
void Hw_LCD_PIN_RS_LOW( void );
void Hw_LCD_PIN_RST_HIGH( void );
void Hw_LCD_PIN_RST_LOW( void );


GLCD_Class::GLCD_Class(void)
{
	IsInit = false;
}


bool GLCD_Class::begin(void)
{
	bool ret = true;


	pinMode( GLCD_RST , OUTPUT );
	pinMode( GLCD_RS  , OUTPUT );

	digitalWrite( GLCD_RST , HIGH );
	digitalWrite( GLCD_RS  , HIGH );

	
	SPI.begin(GlcdSlot);
	SPI.begin( (MOSI)GLCD_SCI, (MISO)16, (SCK)GLCD_SCL );

	SPI.setDataMode     (SPI_MODE3);
	//SPI.setClockDivider (SPI_CLOCK_DIV1024);
	SPI.setClockDivider (SPI_CLOCK_DIV0);

	
	SPI.end_bit();

	position_x = 0;
	position_y = 0;

	Hw_LCD_Init();
	//Hw_LCD_DspImg(NULL);

	return ret;
}

void GLCD_Class::loop() 
{
	//Hw_LCD_Test();
	Hw_LCD_DspStr((u8 *)"Abcd", 0, 1 );
}

inline size_t GLCD_Class::write(uint8_t value) 
{
	bool line_feed = false;

	if( value == '\n' )
	{
		line_feed = true;
		position_x = 0;
	}
	else
	{
		Hw_LCD_DspChar(value, position_x, position_y);	

		position_x++;
		
		if( position_x >= GLCD_CHAR_WIDTH )
		{
			position_x = 0;
			line_feed = true;
		}	
	}
	


	if( line_feed == true )
	{
		position_y++;

		if( position_y >= GLCD_CHAR_HEIGHT )
		{
			position_y = 0;	
		}
	}

  return 1; // assume sucess
}


void GLCD_Class::clear(void) 
{
	Hw_LCD_Clear(0);
}


void GLCD_Class::setCursor(uint8_t col, uint8_t row)
{  
	position_x = col;
	position_y = row;
}


void GLCD_Class::drawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pBuf )
{  
	Hw_LCD_DspBuf( x, y, width, height, pBuf );
}


void GLCD_Class::drawGray(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pBuf )
{  
	Hw_LCD_DspGrayBuf( x, y, width, height, pBuf );
}


























u16 Xil_EndianSwap16(u16 Data)
{
	return (u16) (((Data & 0xFF00) >> 8) | ((Data & 0x00FF) << 8));
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_Init( void )
{


	Hw_LCD_PIN_RST_HIGH();
	Hw_LCD_PIN_RS_HIGH();

	Hw_LCD_PIN_RST_LOW();
	Hw_LCD_Delay_ms(20);
	Hw_LCD_PIN_RST_HIGH();
	Hw_LCD_Delay_ms(20);

	Hw_LCD_CfgInit();
	Hw_LCD_Clear(color[0]);
	Hw_LCD_Clear(color[1]);
	Hw_LCD_Clear(color[2]);
	Hw_LCD_Clear(0);
	//Hw_LCD_Test();

	Hw_LCD_DspStr((u8 *)"GLCD Init...", 0, 0);
	//Hw_LCD_DspImg(NULL);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_CfgInit
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_CfgInit()
{
	Hw_LCD_WR_CMD( 0x000, 0x0001 ); /* oschilliation start */
	Hw_LCD_Delay_ms( 10 );
	/* Power settings */
	Hw_LCD_WR_CMD( 0x100, 0x0000 ); /*power supply setup*/
	Hw_LCD_WR_CMD( 0x101, 0x0000 );
	Hw_LCD_WR_CMD( 0x102, 0x3110 );
	Hw_LCD_WR_CMD( 0x103, 0xe200 );
	Hw_LCD_WR_CMD( 0x110, 0x009d );
	Hw_LCD_WR_CMD( 0x111, 0x0022 );
	Hw_LCD_WR_CMD( 0x100, 0x0120 );
	Hw_LCD_Delay_ms( 20 );

	Hw_LCD_WR_CMD( 0x100, 0x3120 );
	Hw_LCD_Delay_ms( 80 );
	/* Display control */
	Hw_LCD_WR_CMD( 0x001, 0x0100 );
	Hw_LCD_WR_CMD( 0x002, 0x0000 );
	Hw_LCD_WR_CMD( 0x003, 0x1230 );	// 0x1230
	Hw_LCD_WR_CMD( 0x006, 0x0000 );
	Hw_LCD_WR_CMD( 0x007, 0x0101 );
	Hw_LCD_WR_CMD( 0x008, 0x0808 );
	Hw_LCD_WR_CMD( 0x009, 0x0000 );
	Hw_LCD_WR_CMD( 0x00b, 0x0000 );
	Hw_LCD_WR_CMD( 0x00c, 0x0000 );	// 0x0000
	Hw_LCD_WR_CMD( 0x00d, 0x0018 );
	/* LTPS control settings */
	Hw_LCD_WR_CMD( 0x012, 0x0000 );
	Hw_LCD_WR_CMD( 0x013, 0x0000 );
	Hw_LCD_WR_CMD( 0x018, 0x0000 );
	Hw_LCD_WR_CMD( 0x019, 0x0000 );

	Hw_LCD_WR_CMD( 0x203, 0x0000 );
	Hw_LCD_WR_CMD( 0x204, 0x0000 );

	Hw_LCD_WR_CMD( 0x210, 0x0000 );
	Hw_LCD_WR_CMD( 0x211, 0x00ef );
	Hw_LCD_WR_CMD( 0x212, 0x0000 );
	Hw_LCD_WR_CMD( 0x213, 0x013f );
	Hw_LCD_WR_CMD( 0x214, 0x0000 );
	Hw_LCD_WR_CMD( 0x215, 0x0000 );
	Hw_LCD_WR_CMD( 0x216, 0x0000 );
	Hw_LCD_WR_CMD( 0x217, 0x0000 );

	#define YT	1

	// Gray scale settings
	Hw_LCD_WR_CMD( 0x300, 0x5343);
	Hw_LCD_WR_CMD( 0x301, 0x1021);
	Hw_LCD_WR_CMD( 0x302, 0x0003);
	Hw_LCD_WR_CMD( 0x303, 0x0011);
	Hw_LCD_WR_CMD( 0x304, 0x050a);
	Hw_LCD_WR_CMD( 0x305, 0x4342);
	Hw_LCD_WR_CMD( 0x306, 0x1100);
	Hw_LCD_WR_CMD( 0x307, 0x0003);
	Hw_LCD_WR_CMD( 0x308, 0x1201);
	Hw_LCD_WR_CMD( 0x309, 0x050a);

	/* RAM access settings */
	Hw_LCD_WR_CMD( 0x400, 0x4027 );
	Hw_LCD_WR_CMD( 0x401, 0x0000 );
	Hw_LCD_WR_CMD( 0x402, 0x0000 );	/* First screen drive position (1) */
	Hw_LCD_WR_CMD( 0x403, 0x013f );	/* First screen drive position (2) */
	Hw_LCD_WR_CMD( 0x404, 0x0000 );

	Hw_LCD_WR_CMD( 0x200, 0x0000 );
	Hw_LCD_WR_CMD( 0x201, 0x0000 );

	Hw_LCD_WR_CMD( 0x100, 0x7120 );
	Hw_LCD_WR_CMD( 0x007, 0x0103 );
	Hw_LCD_Delay_ms( 10 );
	Hw_LCD_WR_CMD( 0x007, 0x0113 );
}



/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_SetRS
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_SetRS( u8 Data )
{
	if( Data == 1 )
	{
		Hw_LCD_PIN_RS_HIGH();		
	}
	else
	{
		Hw_LCD_PIN_RS_LOW();
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_SetCS
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_SetCS( u8 Data )
{
	
	if( Data == 1 )
	{
		//Hw_SPI_Sel(HW_SPI_CH1, 1);
		SPI.end_bit();
	}
	else
	{
		//Hw_SPI_Sel(HW_SPI_CH1, 0);		
		SPI.begin_bit();
	}
	
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WriteSPI_Byte
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WriteSPI_Byte( u8 Data )
{
	//Hw_SPI_Write8Bit( HW_SPI_CH1, Data );
	SPI.transfer(Data);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WriteSPI_U16
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WriteSPI_U16( u16 Data )
{
	SPI.transfer16(Data);
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WriteSPI
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WriteSPI( u8 *pData, u32 Length )
{
	u32 i;


	for( i=0; i<Length; i++ )
	{
		SPI.transfer(pData[i]);
	}

	//Hw_SPI_Write( HW_SPI_CH1, pData, Length, 100 );
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WR_REG
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WR_REG(uint index)
{
	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(0);

	Hw_LCD_WriteSPI_Byte( index>>8 );
	Hw_LCD_WriteSPI_Byte( index>>0 );

	Hw_LCD_SetCS(1);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WR_CMD
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WR_CMD(uint index,uint val)
{

	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(0);

	Hw_LCD_WriteSPI_Byte( index>>8 );
	Hw_LCD_WriteSPI_Byte( index>>0 );

	Hw_LCD_SetRS(1);

	Hw_LCD_WriteSPI_Byte( val>>8 );
	Hw_LCD_WriteSPI_Byte( val>>0 );

	Hw_LCD_SetCS(1);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WR_Data
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WR_Data(uint val)
{

	//Hw_LCD_WriteSPI_Byte( val>>8 );
	//Hw_LCD_WriteSPI_Byte( val>>0 );
	Hw_LCD_WriteSPI_U16(val);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_WR_Data
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_WR_Buf(u16 *pData, u32 Length)
{
	int i;
	u8 *pu8Data = (u8 *)pData;

	//Hw_LCD_WriteSPI( pu8Data, Length*2 );

	for( i=0; i<Length; i++ )
	{
		SPI.transfer16(pData[i]);
	}
}






/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_PIN_RST_HIGH
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_PIN_RST_HIGH( void )
{	
	digitalWrite( GLCD_RST , HIGH );
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_PIN_RST_LOW
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_PIN_RST_LOW( void )
{
	digitalWrite( GLCD_RST , LOW );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_PIN_RS_HIGH
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_PIN_RS_HIGH( void )
{
	digitalWrite( GLCD_RS  , HIGH );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_PIN_RS_HIGH
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_PIN_RS_LOW( void )
{
	digitalWrite( GLCD_RS  , LOW );	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_Delay_ms
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_Delay_ms( u32 _delay )
{
	delay(_delay);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_Test
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_Test()
{
	uint temp,num,i,j;
	u8 n;

	Hw_LCD_WR_CMD(0x210,0x00);
	Hw_LCD_WR_CMD(0x212,0x0000);
	Hw_LCD_WR_CMD(0x211,0xEF);
	Hw_LCD_WR_CMD(0x213,0x013F);

	Hw_LCD_WR_CMD(0x200,0x0000);
	Hw_LCD_WR_CMD(0x201,0x0000);

	Hw_LCD_WR_REG(0x202);

	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(1);

	/*
	for(n=0;n<8;n++)
	{
	    temp=color[n];
		for(num=40*240;num>0;num--)
		{
			Hw_LCD_WR_Data(temp);
		}
	}
	*/



	for(n=0;n<3;n++)
	{
	    temp=color[n];

		for( i=0; i<240; i++ )
		{
			for( j=0; j<320; j++ )
			{
				Hw_LCD_ImgBuf[i*320 + j] = Xil_EndianSwap16(temp);
			}
		}

		Hw_LCD_WR_CMD(0x210,0x00);
		Hw_LCD_WR_CMD(0x212,0x0000);
		Hw_LCD_WR_CMD(0x211,0xEF);
		Hw_LCD_WR_CMD(0x213,0x013F);

		Hw_LCD_WR_CMD(0x200,0x0000);
		Hw_LCD_WR_CMD(0x201,0x0000);

		Hw_LCD_WR_REG(0x202);

		Hw_LCD_SetCS(0);
		Hw_LCD_SetRS(1);

		Hw_LCD_WR_Buf( Hw_LCD_ImgBuf, 320*240 );

		for(i=0;i<240;i++)
		{
			for(num=0;num<320;num++)
			{
		  		//Hw_LCD_WR_Data(temp);
		  	}
		}

		Hw_LCD_Delay_ms(100);
	}

	Hw_LCD_SetCS(1);
}





void Hw_LCD_DspChar(u8  casc, u8 postion_x, u8 postion_y)
{
	u8 i,j,b;
	u8 *p;

	Hw_LCD_WR_CMD(0x210,postion_x*8); 	//x start point
	Hw_LCD_WR_CMD(0x212,postion_y*16); 	//y start point
	Hw_LCD_WR_CMD(0x211,postion_x*8+7);	//x end point
	Hw_LCD_WR_CMD(0x213,postion_y*16+15);	//y end point

	Hw_LCD_WR_CMD(0x200,postion_x*8);
	Hw_LCD_WR_CMD(0x201,postion_y*16);

	Hw_LCD_WR_REG(0x202);

	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(1);

	p=ascii;
	p+=casc*16;
	for(j=0;j<16;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if(b&0x80)
			{
				Hw_LCD_WR_Data(0xffff);
			}
			else
			{
				Hw_LCD_WR_Data(0x0000);
			}
			b=b<<1;

		}
	}
	Hw_LCD_SetCS(1);
}


void Hw_LCD_DspStr(u8 *s, u8 x, u8 y)
{
	while (*s)
	{
		Hw_LCD_DspChar(*s,x,y);
		if(++x>=30)
		{
			x=0;
			if(++y>=20)
			{
				y=0;
			}
		}
		s++;
    }
}





void Hw_LCD_DspImg( u16 *pBuf )
{
	u16 num;
	u16 *p;
	u8 c,g;
	u16 i,j;

	for(g=0;g<8;g++)
	{
		for(c=0;c<6;c++)
		{
			Hw_LCD_WR_CMD(0x210,c*40);
			Hw_LCD_WR_CMD(0x212,g*40);
			Hw_LCD_WR_CMD(0x211,c*40+39);
			Hw_LCD_WR_CMD(0x213,g*40+39);

			Hw_LCD_WR_CMD(0x200,c*40);
			Hw_LCD_WR_CMD(0x201,g*40);

			Hw_LCD_WR_REG(0x202);
			Hw_LCD_SetCS(0);
			Hw_LCD_SetRS(1);
			p = (u16*)gImage_qqp2;

			for( i=0; i<240; i++ )
			{
				for( j=0; j<320; j++ )
				{
					Hw_LCD_ImgBuf[i*320 + j] = Xil_EndianSwap16(p[i*320+j]);
				}
			}

			Hw_LCD_WR_Buf( Hw_LCD_ImgBuf, 1600 );


   			for(num=0;num<1600;num++)	//
			{
				//Hw_LCD_WR_Data(*p++);
			}



		}
  	}
	Hw_LCD_SetCS(1);
}


void Hw_LCD_DspBuf( u16 x, u16 y, u16 width, u16 height, u16 *pBuf )
{
	u16 num;
	u32 i;
	u32 length;

	Hw_LCD_WR_CMD(0x210,x);
	Hw_LCD_WR_CMD(0x212,y);
	Hw_LCD_WR_CMD(0x211,x+width-1);
	Hw_LCD_WR_CMD(0x213,y+height-1);

	Hw_LCD_WR_CMD(0x200,x);
	Hw_LCD_WR_CMD(0x201,y);

	Hw_LCD_WR_REG(0x202);
	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(1);


	length = width*height;

	for( i=0; i<length; i++ )
	{
		Hw_LCD_WR_Data( pBuf[i] );
	}

	Hw_LCD_SetCS(1);
}



void Hw_LCD_DspGrayBuf( u16 x, u16 y, u16 width, u16 height, u16 *pBuf )
{
	u16 num;
	u32 i;
	u32 length;
	u16 r,g,b;
	u16 pixel;


	Hw_LCD_WR_CMD(0x210,x);
	Hw_LCD_WR_CMD(0x212,y);
	Hw_LCD_WR_CMD(0x211,x+width-1);
	Hw_LCD_WR_CMD(0x213,y+height-1);

	Hw_LCD_WR_CMD(0x200,x);
	Hw_LCD_WR_CMD(0x201,y);

	Hw_LCD_WR_REG(0x202);
	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(1);


	length = width*height;



	for( i=0; i<length; i++ )
	{
		pixel = pBuf[i];

		r = pixel>>3;
		g = pixel>>2;
		b = pixel>>3;

		pixel = r<<11 | g<<5 | b<<0;

		Hw_LCD_WR_Data( pixel );
	}

	Hw_LCD_SetCS(1);
}



/*---------------------------------------------------------------------------
     TITLE   : Hw_LCD_Test
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_LCD_Clear(uint p)
{
	uint i,j;


	Hw_LCD_WR_CMD(0x210,0x00);
	Hw_LCD_WR_CMD(0x212,0x0000);
	Hw_LCD_WR_CMD(0x211,0xEF);
	Hw_LCD_WR_CMD(0x213,0x013F);

	Hw_LCD_WR_CMD(0x200,0x0000);
	Hw_LCD_WR_CMD(0x201,0x0000);

	Hw_LCD_WR_REG(0x202);

	Hw_LCD_SetCS(0);
	Hw_LCD_SetRS(1);

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			Hw_LCD_WR_Data(p);
		}
	}

	Hw_LCD_SetCS(1);
}
