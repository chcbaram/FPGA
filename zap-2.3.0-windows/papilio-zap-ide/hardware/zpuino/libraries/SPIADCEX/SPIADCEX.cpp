#include "SPIADCEX.h"

void SPIADCEX::begin(WISHBONESLOT wishboneSlot, ADCBITS bits)
{

	adcBits = bits;

 	//SPI.begin(wishboneSlot);
	
	m_wishboneSlot = wishboneSlot;


	//SPI.setBitOrder     (MSBFIRST);
	SPI.setDataMode     (SPI_MODE3);
	SPI.setClockDivider (SPI_CLOCK_DIV1024);
}

uint16_t SPIADCEX::read (byte pin)
{
	uint16_t data ;


	// 0000 0XXX
	// 00XX X000
	SPI.begin(m_wishboneSlot);
	pin = pin << 4 ;
	SPI.transfer(pin);
	//reach 16 rising edge
	SPI.transfer(0);
	//last 8 bits
	
	data  = SPI.transfer(0);
	data  = data << 8;
	data |= SPI.transfer(0);

	SPI.end();

	if ( adcBits ) return data  ; // 12 bit
	else       return (data >> 4) ; //  8 bit

}


SPIADCEX analogEx ;

