#ifndef __TIMER_H__
#define __TIMER_H__

#include <zpuino-types.h>


class TimerClass
{
public:
	void begin();
	void setPWMFrequency(long int hz);
	void setPWMOutputPin(int);
	void setPWMDuty(uint8_t val);

protected:
	long count;
};

#endif
