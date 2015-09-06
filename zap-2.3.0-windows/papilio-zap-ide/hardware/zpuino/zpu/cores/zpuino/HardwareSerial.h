#ifndef __HardwareSerial_h__
#define __HardwareSerial_h__

#include <zpuino.h>
#include <zpuino-types.h>
#include "Stream.h"
#include "Print.h"

class HardwareSerial: public Stream
{
private:
public:
	HardwareSerial(unsigned int b): ioslot(IO_SLOT(b)) {}

	__attribute__((always_inline)) inline void begin(const unsigned int baudrate) {
		if (__builtin_constant_p(baudrate)) {
			REGISTER(ioslot,1) = BAUDRATEGEN(baudrate) | BIT(UARTEN);
		} else {
			begin_slow(baudrate);
		}
	}
	void begin_slow(const unsigned int baudrate);

	int available(void) {
		return (REGISTER(ioslot,1) & 1);
	}

	virtual int read(void) {
		return REGISTER(ioslot,0);
	}

	virtual void flush(void) {};

	size_t write(uint8_t c);

	virtual int peek() { return -1; }

	using Print::write; // pull in write(str) and write(buf, size) from Print
private:
	unsigned int ioslot;
};

extern void serialEventRun(void) __attribute__((weak));

extern HardwareSerial Serial; /* 1st slot */

#endif
