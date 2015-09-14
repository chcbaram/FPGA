
#include "SPI.h"     
#include "SPIADCEX.h"


void setup()
{
  delay(2000);
  Serial.begin(115200);
  analogEx.begin(WISHBONESLOT(9),ADCBITS(SPIADC_8BIT));
  Serial.println("Channel 0\t1\t2\t3\t4\t5\t6\t7");
}
     

void loop() {
  Serial.print("\t");
  for ( int i = 0; i < 8; i++)
  {
    Serial.print(analogEx.read(i));
    Serial.print("\t");
  }
  Serial.println();
  delay(100);
}
