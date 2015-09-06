/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <stdio.h>
#ifdef __AVR__
#include <Arduino.h>
#include <avr/pgmspace.h>
#endif

#ifdef __AVR__

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

#endif

#ifdef ZPU

#include <zpuino-types.h>
#include <zpuino.h>
#define SPI_CLOCK_DIV0 0
#define SPI_CLOCK_DIV2 SPICP0
#define SPI_CLOCK_DIV4 SPICP1
#define SPI_CLOCK_DIV8 (SPICP1|SPICP0)
#define SPI_CLOCK_DIV16 SPICP2
#define SPI_CLOCK_DIV64 (SPICP2|SPICP0)
#define SPI_CLOCK_DIV256 (SPICP2|SPICP1)
#define SPI_CLOCK_DIV1024 (SPICP2|SPICP1|SPICP0)

/* Note - these modes are not 100% accurate yet */

#define SPI_MODE0 0
#define SPI_MODE1 SPISRE
#define SPI_MODE2 SPICPOL
#define SPI_MODE3 (SPICPOL|SPISRE)

#define SPI_CLOCK_MASK (SPICP0|SPICP1|SPICP2)
#define SPI_MODE_MASK (SPICPOL|SPISRE)

#endif /* defined ZPU */


class SPIClass {
public:
  inline static byte transfer(byte _data);

  // SPI Configuration methods

  inline static void attachInterrupt();
  inline static void detachInterrupt(); // Default
#ifndef ZPU
  static void begin(); // Default
#else
  static void begin(int mosi, int miso, int sck);
#endif
  static void end();

  static void setBitOrder(uint8_t);
  static void setDataMode(uint8_t);
  static void setClockDivider(uint8_t);
};

extern SPIClass SPI;

#ifdef __AVR__

byte SPIClass::transfer(byte _data) {
  SPDR = _data;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}

void SPIClass::attachInterrupt() {
  SPCR |= _BV(SPIE);
}

void SPIClass::detachInterrupt() {
  SPCR &= ~_BV(SPIE);
}

#endif /* defined __AVR__ */

#ifdef ZPU

byte SPIClass::transfer(byte _data) {
  USPIDATA = _data;
  return USPIDATA;
}

void SPIClass::attachInterrupt() {
  /* No interrupt support */
}

void SPIClass::detachInterrupt() {
  /* No interrupt support */
}

#endif /* defined __AVR__ */


#endif
