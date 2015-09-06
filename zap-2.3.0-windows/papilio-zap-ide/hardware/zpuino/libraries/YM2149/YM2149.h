/*!
 *  @file		YM2149.h
 *  Project		YM2149 Library
 *	@brief		YM2149 Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */

#ifndef LIB_YM2149_H_
#define LIB_YM2149_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"


#define YM_ADDR_FREQ_A 0x00
#define YM_ADDR_FREQ_B 0x02
#define YM_ADDR_FREQ_C 0x04
#define YM_ADDR_NOISE 0x06
#define YM_ADDR_MIXER 0x07
#define YM_ADDR_LEVEL_A 0x08
#define YM_ADDR_LEVEL_B 0x09
#define YM_ADDR_LEVEL_C 0x0A
#define YM_ADDR_FREQ_E 0x0B
#define YM_ADDR_SHAPE_E 0x0D

// #define YM2149BASE IO_SLOT(12)
// #define YM2149REG(x) REGISTER(YM2149BASE,x)  

class YMVoice
{ 
  public:
    void setBase(int freqAddress, int volumeAddress,unsigned int wishboneSlot);
    void setNote(int note, boolean active);
    void setFreq(int freq);
    void setVolume(byte volume);
    void setEnvelope(boolean active);
    void setTone(boolean active);
    void setNoise(boolean active);
    void setNoiseFrequency(byte freq);    
    void setEnvelopeFrequency(int freq);
    void setEnvelopeFrequencyLo(byte freq);
    void setEnvelopeFrequencyHi(byte freq);    
    void setEnvelopeCONT(boolean active);
    void setEnvelopeATT(boolean active);
    void setEnvelopeALT(boolean active);
    void setEnvelopeHOLD(boolean active); 	
    void reset(); 
    void handleCC(byte number, byte value); 
    int getCurrentFreq(); 
    byte getVolume();   
  private:
	unsigned int ymbase;
    int baseAddress;
    int currentFreq;
    int YM_ADDR_FREQ;
    int YM_ADDR_LEVEL;
    struct YM_REG_LEVEL_STRUCT{
        unsigned int EMPTY : 3;
        unsigned int MODE : 1; 
        unsigned int LEVEL : 4;
    } ;  
    YM_REG_LEVEL_STRUCT YM_REG_LEVEL;     
};

class YM2149
{
  public:
    YMVoice V1;
    YMVoice V2;
    YMVoice V3;    
    YM2149();
	void setup(unsigned int wishboneSlot);
    void writeData(unsigned char address, unsigned char data);      
    void reset();    
    static const int MIDI2freq[129];  
  private:
	unsigned int ymbase;
};
#endif // LIB_YM2149_H_
