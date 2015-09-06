/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef __PINS_PAPILIO_H__
#define __PINS_PAPILIO_H__

//****************** Special definitions for the Papilio Platform **************************
//Papilio Pin Swaps
#define PWM0_LOC *(byte *) 0x1000
#define PWM1A_LOC *(byte *) 0x1001
#define PWM1B_LOC *(byte *) 0x1002
#define PWM2_LOC *(byte *) 0x1003
#define SPI1_mosi_LOC *(byte *) 0x1004
#define SPI1_miso_LOC *(byte *) 0x1005
#define SPI1_sck_LOC *(byte *) 0x1006
#define SPI1_cs_LOC *(byte *) 0x1007

//Papilio Stepper Core definitions
#define SM1_LOC *(byte *) 0x2000
#define SM1CONTROL_L *(byte *) 0x2001
#define SM1CONTROL_H *(byte *) 0x2002
#define SM1TIMEBASE_L *(byte *) 0x2003
#define SM1TIMEBASE_H *(byte *) 0x2004
#define SM1PERIOD_L *(byte *) 0x2005
#define SM1PERIOD_H *(byte *) 0x2006
#define SM1STEPCNT_L *(byte *) 0x2007
#define SM1STEPCNT_H *(byte *) 0x2008
#define SM1STEPS_L *(byte *) 0x2009
#define SM1STEPS_H *(byte *) 0x2010

//Papilio Button/LED Core definitions
#define BL1_LOC *(byte *) 0x3000
#define BL1CONTROL_L *(byte *) 0x3001
#define BL1CONTROL_H *(byte *) 0x3002

//Papilio Wing Slots
#define NO_SLOT 0
#define AL 1
#define AH 2
#define BL 3
#define BH 4
#define CL 5
#define CH 6


//Papilio One Pin definitions
//Wing 1 Column A
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define A8 8
#define A9 9
#define A10 10
#define A11 11
#define A12 12
#define A13 13
#define A14 14
#define A15 15

//Wing 1 Column B
#define B0 16    //This doesn't work for some reason!
#define B1 17
#define B2 18
#define B3 19
#define B4 20
#define B5 21
#define B6 22
#define B7 23
#define B8 24
#define B9 25
#define B10 26
#define B11 27
#define B12 28
#define B13 29
#define B14 30
#define B15 31

//Wing 2 Column C
#define C0 32
#define C1 33
#define C2 34
#define C3 35
#define C4 36
#define C5 37
#define C6 38
#define C7 39
#define C8 40
#define C9 41
#define C10 42
#define C11 43
#define C12 44
#define C13 45
#define C14 46
#define C15 47

// ******** Define AH/AL syntax
//Wing 1 Column A
#define AL0 0
#define AL1 1
#define AL2 2
#define AL3 3
#define AL4 4
#define AL5 5
#define AL6 6
#define AL7 7
#define AH0 8
#define AH1 9
#define AH2 10
#define AH3 11
#define AH4 12
#define AH5 13
#define AH6 14
#define AH7 15

//Wing 1 Column B
#define BL0 16    //This doesn't work for some reason!
#define BL1 17
#define BL2 18
#define BL3 19
#define BL4 20
#define BL5 21
#define BL6 22
#define BL7 23
#define BH0 24
#define BH1 25
#define BH2 26
#define BH3 27
#define BH4 28
#define BH5 29
#define BH6 30
#define BH7 31

//Wing 2 Column C
#define CL0 32
#define CL1 33
#define CL2 34
#define CL3 35
#define CL4 36
#define CL5 37
#define CL6 38
#define CL7 39
#define CH0 40
#define CH1 41
#define CH2 42
#define CH3 43
#define CH4 44
#define CH5 45
#define CH6 46
#define CH7 47

//Wing 1 Column A
#define WA0 0
#define WA1 1
#define WA2 2
#define WA3 3
#define WA4 4
#define WA5 5
#define WA6 6
#define WA7 7
#define WA8 8
#define WA9 9
#define WA10 10
#define WA11 11
#define WA12 12
#define WA13 13
#define WA14 14
#define WA15 15

//Wing 1 Column B
#define WB0 16    //This doesn't work for some reason!
#define WB1 17
#define WB2 18
#define WB3 19
#define WB4 20
#define WB5 21
#define WB6 22
#define WB7 23
#define WB8 24
#define WB9 25
#define WB10 26
#define WB11 27
#define WB12 28
#define WB13 29
#define WB14 30
#define WB15 31

//Wing 2 Column C
#define WC0 32
#define WC1 33
#define WC2 34
#define WC3 35
#define WC4 36
#define WC5 37
#define WC6 38
#define WC7 39
#define WC8 40
#define WC9 41
#define WC10 42
#define WC11 43
#define WC12 44
#define WC13 45
#define WC14 46
#define WC15 47

// ******** Define AH/AL syntax
//Wing 1 Column A
#define WAL0 0
#define WAL1 1
#define WAL2 2
#define WAL3 3
#define WAL4 4
#define WAL5 5
#define WAL6 6
#define WAL7 7
#define WAH0 8
#define WAH1 9
#define WAH2 10
#define WAH3 11
#define WAH4 12
#define WAH5 13
#define WAH6 14
#define WAH7 15

//Wing 1 Column B
#define WBL0 16    //This doesn't work for some reason!
#define WBL1 17
#define WBL2 18
#define WBL3 19
#define WBL4 20
#define WBL5 21
#define WBL6 22
#define WBL7 23
#define WBH0 24
#define WBH1 25
#define WBH2 26
#define WBH3 27
#define WBH4 28
#define WBH5 29
#define WBH6 30
#define WBH7 31

//Wing 2 Column C
#define WCL0 32
#define WCL1 33
#define WCL2 34
#define WCL3 35
#define WCL4 36
#define WCL5 37
#define WCL6 38
#define WCL7 39
#define WCH0 40
#define WCH1 41
#define WCH2 42
#define WCH3 43
#define WCH4 44
#define WCH5 45
#define WCH6 46
#define WCH7 47

#endif
