/*
  I2C.cpp - I2C library
  Copyright (c) 2011-2012 Wayne Truchsess.  All right reserved.
  Rev 5.0 - January 24th, 2012
          - Removed the use of interrupts completely from the library
            so TWI state changes are now polled. 
          - Added calls to lockup() function in most functions 
            to combat arbitration problems 
          - Fixed scan() procedure which left timeouts enabled 
            and set to 80msec after exiting procedure
          - Changed scan() address range back to 0 - 0x7F
          - Removed all Wire legacy functions from library
          - A big thanks to Richard Baldwin for all the testing
            and feedback with debugging bus lockups!
  Rev 4.0 - January 14th, 2012
          - Updated to make compatible with 8MHz clock frequency
  Rev 3.0 - January 9th, 2012
          - Modified library to be compatible with Arduino 1.0
          - Changed argument type from boolean to uint8_t in pullUp(), 
            setSpeed() and receiveByte() functions for 1.0 compatability
          - Modified return values for timeout feature to report
            back where in the transmission the timeout occured.
          - added function scan() to perform a bus scan to find devices
            attached to the I2C bus.  Similar to work done by Todbot
            and Nick Gammon
  Rev 2.0 - September 19th, 2011
          - Added support for timeout function to prevent 
            and recover from bus lockup (thanks to PaulS
            and CrossRoads on the Arduino forum)
          - Changed return type for stop() from void to
            uint8_t to handle timeOut function 
  Rev 1.0 - August 8th, 2011
  
  This is a modified version of the Arduino Wire/TWI 
  library.  Functions were rewritten to provide more functionality
  and also the use of Repeated Start.  Some I2C devices will not
  function correctly without the use of a Repeated Start.  The 
  initial version of this library only supports the Master.


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#if(ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <inttypes.h>
#include "I2C.h"


#define I2C_SLOT 8
 
/* Define I2C registers.
*/
#define I2C_BASE   IO_SLOT(I2C_SLOT)
#define I2C_PRERlo REGISTER(I2C_BASE, 3)
#define I2C_PRERhi REGISTER(I2C_BASE, 4)
#define I2C_CTR    REGISTER(I2C_BASE, 0)
#define I2C_TXR    REGISTER(I2C_BASE, 1)
#define I2C_RXR    REGISTER(I2C_BASE, 1)
#define I2C_CR     REGISTER(I2C_BASE, 2)
#define I2C_SR     REGISTER(I2C_BASE, 2)

/* I2C core control register bit definitions.
*/
#define CTR_EN 0x80 // i2c core enable bit.
// When set to ���� the core is enabled.
// When set to ���� the core is disabled.
 
#define CTR_IEN 0x40 // i2c core interrupt enable bit.
// When set to ���� interrupt is enabled.
// When set to ���� interrupt is disabled.
 
/* I2C core command register bit definitions.
*/
#define CR_STA  0x80 // generate (repeated) start condition
#define CR_STO  0x40 // generate stop condition
#define CR_RD   0x20 // read from slave
#define CR_WR   0x10 // write to slave
#define CR_ACK  0x08 // when a receiver, send ACK (ACK = ���� or NACK (ACK = ����
#define CR_IACK 0x01 // interrupt acknowledge. When set, clears a pending interrupt.
 
/* I2C status register bit definitions.
*/
#define SR_RxACK 0x80 // received acknowledge from slave.
// This flag represents acknowledge from the addressed slave.
// ����= No acknowledge received
// ����= Acknowledge received
 
#define SR_BUSY 0x40 // i2c bus busy
// ����after START signal detected
// ����after STOP signal detected
 
#define SR_AL 0x20 // arbitration lost
// This bit is set when the core lost arbitration.
// Arbitration is lost when:
// a STOP signal is detected, but non requested
// The master drives SDA high, but SDA is low.
 
#define SR_TIP 0x02 // transfer in progress.
// ����when transferring data
// ����when transfer complete
 
#define SR_IF 0x01 // interrupt Flag. This bit is set when an
// interrupt is pending, which will cause a
// processor interrupt request if the IEN bit is set.
// The Interrupt Flag is set when:
// one byte transfer has been completed
// arbitration is lost
 
 
/* I2C core read/write flags.
*/
#define I2C_M_WR 0x00
#define I2C_M_RD 0x01
 
/* Define I2C devivce address.
*/
#define I2C_DEVICE_ADDRESS 0x68




uint8_t I2C::bytesAvailable = 0;
uint8_t I2C::bufferIndex = 0;
uint8_t I2C::totalBytes = 0;
uint16_t I2C::timeOutDelay = 0;

I2C::I2C()
{
}


////////////// Public Methods ////////////////////////////////////////



void I2C::begin()
{
    I2C_CTR = CTR_EN;
    I2C_PRERlo = 50;
    I2C_PRERhi = 0;  

    timeOut(80);

    stop();
}

void I2C::end()
{

}

void I2C::timeOut(uint16_t _timeOut)
{
  timeOutDelay = _timeOut;
}

void I2C::setSpeed(uint8_t _fast)
{
}
  
void I2C::pullup(uint8_t activate)
{
}

void I2C::scan()
{
  uint16_t tempTime = timeOutDelay;
  timeOut(80);
  uint8_t totalDevicesFound = 0;
  Serial.println("Scanning for devices...please wait");
  Serial.println();
  for(uint8_t s = 0x00; s <= 0x7F; s++)
  {
    returnStatus = 0;
    returnStatus = start();
    //Serial.println(returnStatus, HEX);

    if(!returnStatus)
    { 
      //Serial.println("start..");
      returnStatus = sendAddress(SLA_W(s));

      //Serial.println(returnStatus, HEX);
    }
    else
    {
     //Serial.println(returnStatus, HEX); 
    }
    if(returnStatus)
    {
      if(returnStatus == 1)
      {
        Serial.println("There is a problem with the bus, could not complete scan");
        timeOutDelay = tempTime;
        return;
      }
    }
    else
    {
      Serial.print("Found device at address - ");
      Serial.print(" 0x");
      Serial.println(s,HEX);
      totalDevicesFound++;
    }
    stop();
  }
  if(!totalDevicesFound){Serial.println("No devices found");}
  timeOutDelay = tempTime;
}


uint8_t I2C::available()
{
  return(bytesAvailable);
}

uint8_t I2C::receive()
{
  bufferIndex = totalBytes - bytesAvailable;
  if(!bytesAvailable)
  {
    bufferIndex = 0;
    return(0);
  }
  bytesAvailable--;
  return(data[bufferIndex]);
}

  
/*return values for new functions that use the timeOut feature 
  will now return at what point in the transmission the timeout
  occurred. Looking at a full communication sequence between a 
  master and slave (transmit data and then readback data) there
  a total of 7 points in the sequence where a timeout can occur.
  These are listed below and correspond to the returned value:
  1 - Waiting for successful completion of a Start bit
  2 - Waiting for ACK/NACK while addressing slave in transmit mode (MT)
  3 - Waiting for ACK/NACK while sending data to the slave
  4 - Waiting for successful completion of a Repeated Start
  5 - Waiting for ACK/NACK while addressing slave in receiver mode (MR)
  6 - Waiting for ACK/NACK while receiving data from the slave
  7 - Waiting for successful completion of the Stop bit

  All possible return values:
  0           Function executed with no errors
  1 - 7       Timeout occurred, see above list
  8 - 0xFF    See datasheet for exact meaning */ 


/////////////////////////////////////////////////////

uint8_t I2C::write(uint8_t address, uint8_t registerAddress)
{
  returnStatus = 0;

#if 1  
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  
#else  
  I2C_TXR = SLA_W(address);
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {     
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 2;    
  }

  I2C_TXR = registerAddress;
  I2C_CR = CR_WR | CR_STO;
  while ((I2C_SR & SR_TIP) != 0)
  {
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 7;
  }
#endif

  return(returnStatus);
}

uint8_t I2C::write(int address, int registerAddress)
{
  return(write((uint8_t) address, (uint8_t) registerAddress));
}

uint8_t I2C::write(uint8_t address, uint8_t registerAddress, uint8_t data)
{
  returnStatus = 0;

#if 1
  returnStatus = start(); 
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = sendByte(data);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
#else
  I2C_TXR = SLA_W(address);
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {     
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 2;    
  }

  I2C_TXR = registerAddress;
  I2C_CR = CR_WR;
  while ((I2C_SR & SR_TIP) != 0)
  {
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 3;
  }

  I2C_TXR = data;
  I2C_CR = CR_WR | CR_STO;
  while ((I2C_SR & SR_TIP) != 0)
  {
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 7;
  }
#endif
  return(returnStatus);
}

uint8_t I2C::write(int address, int registerAddress, int data)
{
  return(write((uint8_t) address, (uint8_t) registerAddress, (uint8_t) data));
}

uint8_t I2C::write(uint8_t address, uint8_t registerAddress, char *data)
{
  uint8_t bufferLength = strlen(data);
  returnStatus = 0;
  returnStatus = write(address, registerAddress, (uint8_t*)data, bufferLength);
  return(returnStatus);
}

uint8_t I2C::write(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t numberBytes)
{
  returnStatus = 0;

#if 1
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  for (uint8_t i = 0; i < numberBytes; i++)
  {
    returnStatus = sendByte(data[i]);
    if(returnStatus)
      {
        if(returnStatus == 1){return(3);}
        return(returnStatus);
      }
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
#else  
  I2C_TXR = SLA_W(address);
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {     
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 2;    
  }

  I2C_TXR = registerAddress;
  I2C_CR = CR_WR;
  while ((I2C_SR & SR_TIP) != 0)
  {
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 3;
  }

  for (uint8_t i = 0; i < numberBytes; i++)
  {
    I2C_TXR = data[i];

    if( i == (numberBytes-1) )    
      I2C_CR = CR_WR | CR_STO;
    else
      I2C_CR = CR_WR;
    
    while ((I2C_SR & SR_TIP) != 0)
    {
    }
    if((I2C_SR & SR_RxACK) != 0)
    {
      return 7;
    }
  }
#endif

  return(returnStatus);
}

uint8_t I2C::read(int address, int numberBytes)
{
  return(read((uint8_t) address, (uint8_t) numberBytes));
}

uint8_t I2C::read(uint8_t address, uint8_t numberBytes)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;

#if 1
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}

      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    data[i] = I2C_RXR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
#else

  I2C_TXR = SLA_R(address);
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {     
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 2;    
  }

  for(uint8_t i = 0; i < numberBytes; i++)
  {

    if( i == nack )
    {
      I2C_CR = CR_RD;
      while ((I2C_SR & SR_TIP) != 0);
      if((I2C_SR & SR_RxACK) == 0) return 6;
    }
    else
    {
      I2C_CR = CR_RD | CR_ACK;
      while ((I2C_SR & SR_TIP) != 0);
      if((I2C_SR & SR_RxACK) != 0) return 6;
    }
    data[i] = I2C_RXR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }

  I2C_CR = CR_STO;
  while ((I2C_SR & SR_TIP) != 0)
  {
  }
#endif

  return(returnStatus);
}

uint8_t I2C::read(int address, int registerAddress, int numberBytes)
{
  return(read((uint8_t) address, (uint8_t) registerAddress, (uint8_t) numberBytes));
}

uint8_t I2C::read(uint8_t address, uint8_t registerAddress, uint8_t numberBytes)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;


#if 1
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = start();
  if(returnStatus)
  {
    if(returnStatus == 1){return(4);}
    return(returnStatus);
  }
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    data[i] = I2C_RXR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
#else
  I2C_TXR = SLA_W(address);
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {     
  }
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 1;    
  }

  I2C_TXR = registerAddress;
  I2C_CR = CR_WR;
  while ((I2C_SR & SR_TIP) != 0);
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 2;    
  }

  I2C_TXR = SLA_R(address);
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0);
  if((I2C_SR & SR_RxACK) != 0)
  {
    return 3;    
  }


  for(uint8_t i = 0; i < numberBytes; i++)
  {

    if( i == nack )
    {
      I2C_CR = CR_RD;
      while ((I2C_SR & SR_TIP) != 0);
      if((I2C_SR & SR_RxACK) == 0) return 6;
    }
    else
    {
      I2C_CR = CR_RD | CR_ACK;
      while ((I2C_SR & SR_TIP) != 0);
      if((I2C_SR & SR_RxACK) != 0) return 6;
    }
    data[i] = I2C_RXR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }

  I2C_CR = CR_STO;
  while ((I2C_SR & SR_TIP) != 0)
  {
  }  
#endif
  return(returnStatus);
}

uint8_t I2C::read(uint8_t address, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    dataBuffer[i] = I2C_RXR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}

uint8_t I2C::read(uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(2);}
    return(returnStatus);
  }
  returnStatus = sendByte(registerAddress);
  if(returnStatus)
  {
    if(returnStatus == 1){return(3);}
    return(returnStatus);
  }
  returnStatus = start();
  if(returnStatus)
  {
    if(returnStatus == 1){return(4);}
    return(returnStatus);
  }
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus == 1){return(6);}
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    dataBuffer[i] = I2C_RXR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  returnStatus = stop();
  if(returnStatus)
  {
    if(returnStatus == 1){return(7);}
    return(returnStatus);
  }
  return(returnStatus);
}


/////////////// Private Methods ////////////////////////////////////////


uint8_t I2C::start()
{
  unsigned long startingTime = millis();

/*  
  I2C_CR = CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
*/
  return(0);
}

uint8_t I2C::sendAddress(uint8_t i2cAddress)
{
  I2C_TXR = i2cAddress;
  unsigned long startingTime = millis();
  
  I2C_CR = CR_WR | CR_STA;
  while ((I2C_SR & SR_TIP) != 0)
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((I2C_SR & SR_RxACK) == 0)
  {
    return(0);
  }
  else
  {
    stop();
    return(I2C_SR);
  }
}

uint8_t I2C::sendByte(uint8_t i2cData)
{
  I2C_TXR = i2cData;
  unsigned long startingTime = millis();

  I2C_CR = CR_WR;
  while ((I2C_SR & SR_TIP) != 0)
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((I2C_SR & SR_RxACK) == 0)
  {
    return(0);
  }
  else
  {
    stop();
    return(I2C_SR);
  }
}

uint8_t I2C::receiveByte(uint8_t ack)
{
  unsigned long startingTime = millis();
  if(ack)
  {
    I2C_CR = CR_RD;
  }
  else
  {
    I2C_CR = CR_RD  | CR_ACK;
  }
  while ((I2C_SR & SR_TIP) != 0)
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
  }
  
  if((I2C_SR & SR_RxACK) == 0)
  {
    return MR_DATA_ACK;
  }
  else
  {
    return MR_DATA_NACK;
  }
}

uint8_t I2C::stop()
{
  unsigned long startingTime = millis();


  I2C_CR = CR_STO;

  while ((I2C_SR & SR_TIP) != 0)
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  return(0);
}

void I2C::lockUp()
{
}

I2C I2c = I2C();

