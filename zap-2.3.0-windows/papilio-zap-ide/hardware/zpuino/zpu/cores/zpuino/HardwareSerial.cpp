#include "HardwareSerial.h"

HardwareSerial Serial(1); /* 1st instance/slot */

void HardwareSerial::begin_slow(const unsigned int baudrate) {
	 REGISTER(ioslot,1) = BAUDRATEGEN(baudrate)|BIT(UARTEN);
}

size_t HardwareSerial::write(unsigned char c) {
	while ((REGISTER(ioslot,1) & 2)==2);
	REGISTER(ioslot,0) = c;
        return 1;
}
