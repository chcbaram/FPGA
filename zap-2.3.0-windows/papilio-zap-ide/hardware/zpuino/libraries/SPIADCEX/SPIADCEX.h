#ifndef SPIADCEX_H_INCLUDED
#define SPIADCEX_H_INCLUDED

#include <Arduino.h>
#include <inttypes.h>

#ifndef SPI_H_INCLUDED

#include <SPI.h>
#endif
#define SPIADC_8BIT  0
#define SPIADC_12BIT 1

typedef unsigned char byte ;


INTVAL(WISHBONESLOT);
INTVAL(ADCBITS);

class SPIADCEX {
  public    :

    void       begin     (WISHBONESLOT wishboneSlot, ADCBITS bits);
    uint16_t   read      (byte pin);
    SPIClass SPI;

  private   :
	int m_wishboneSlot;
  	bool adcBits ;
};

extern SPIADCEX analogEx;



#endif
