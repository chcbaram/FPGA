#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <inttypes.h>
#include <zpuino.h>
#include <delay.h>
#include <HardwareSerial.h>
#include <binary.h>

#ifndef  boolean
#define boolean bool
#endif

#ifndef _BV
#define _BV(x) (1<<(x))
#endif


#define PI          3.1415926535897932384626433832795
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg)            ((deg)*DEG_TO_RAD)
#define degrees(rad)            ((rad)*RAD_TO_DEG)
#define sq(x)                   ((x)*(x))

/* undefine stdlib's abs if encountered */
#ifdef abs
#undef abs
#endif
#define abs(x) (((x) > 0) ? (x) : -(x))


#define A0	0
#define A1	1
#define A2	2
#define A3	3
#define A4	4
#define A5	5
#define A6	6
#define A7	7

int analogRead(uint8_t pin);


#define u8      uint8_t
#define u16     uint16_t
#define u32     uint32_t

#define s8      int8_t
#define s16     int16_t
#define s32     int32_t



#endif
