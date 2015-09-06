#include <Timer.h>
#include <register.h>
#include <zpuino.h>

void TimerClass::begin()
{
	TMR1CTL = BIT(TCTLENA)|BIT(TCTLCCM)|BIT(TCTLDIR);
}
/*
void TimerClass::setPWMFrequency(long int hz)
{
	const int dividers[8] = {
		1,2,4,8,16,64,256,1024
	};

	int i;

	for (i=0;i<8;i++) {
		long count = CLK_FREQ/dividers[i];
		count /= hz;
		if (count<65535) {
			TMR1CNT=0;
			TMR1CMP=count;
			TMR1OCR=count>>1;
			TMR1CTL &= ~(BIT(TCTLCP0)|BIT(TCTLCP1)|BIT(TCTLCP2));
			TMR1CTL |= (i<<TCTLCP0);
			return;
		}
	}
}
*/

void TimerClass::setPWMDuty(uint8_t val)
{
	long cmp = count;
	cmp *= val;
	cmp >>= 8;
//	TMR1OCR=cmp;
}

void TimerClass::setPWMFrequency(long int hz)
{
	count =  (CLK_FREQ / hz) -1 ;
	TMR1CNT=0;
	TMR1CMP=count;
  //  TMR1OCR=count>>1;
	TMR1CTL &= ~(BIT(TCTLCP0)|BIT(TCTLCP1)|BIT(TCTLCP2));
	//TMR1CTL |= (i<<TCTLCP0);
}

void TimerClass::setPWMOutputPin(int pin)
{
	pinMode(pin,OUTPUT);
	pinModePPS(pin,HIGH);
	outputPinForFunction( pin, IOPIN_TIMER1_OC);
}
