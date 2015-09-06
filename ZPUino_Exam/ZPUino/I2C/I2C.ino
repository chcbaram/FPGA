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
 
/* Define PPS signal lines.
*/
#define I2C_SCL 6
#define I2C_SDA 7
 
/* Define FPGA pins
*/
#define I2C_SCL_O WING_C_7
#define I2C_SCL_I WING_C_6
#define I2C_SDA_O WING_C_5
#define I2C_SDA_I WING_C_4
 
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
 
/* I2C core register definitions.
*/
#define REG_CLKOE 0x09
#define REG_DIV1 0x0C
#define REG_XDRV 0x12
#define REG_CAPLOAD 0x13
#define REG_PBHI 0x40
#define REG_PBLO 0x41
#define REG_QCNT 0x42
#define REG_MATRIX1 0x44
#define REG_MATRIX2 0x45
#define REG_MATRIX3 0x46
#define REG_DIV2 0x47
 
/* I2C core read/write flags.
*/
#define I2C_M_WR 0x00
#define I2C_M_RD 0x01
 
/* Define I2C devivce address.
*/
#define I2C_DEVICE_ADDRESS 0x68
void setup()
{
  /*
   pinMode(I2C_SCL_O, OUTPUT);
   pinModePPS(I2C_SCL_O, HIGH);
   outputPinForFunction(I2C_SCL_O, I2C_SCL);
 
   pinMode(I2C_SCL_I, INPUT );
   inputPinForFunction (I2C_SCL_I, I2C_SCL);
 
   pinMode(I2C_SDA_O, OUTPUT);
   pinModePPS(I2C_SDA_O, HIGH);
   outputPinForFunction(I2C_SDA_O, I2C_SDA);
 
   pinMode(I2C_SDA_I, INPUT );
   inputPinForFunction (I2C_SDA_I, I2C_SDA);
 */
   Serial.begin(9600);
   
   I2C_CTR = CTR_EN;
}
 
int count = 0;
void loop()
{
   Serial.print("Calculating tuned frequency...\r\n");
   while (!Serial.available());
   while (Serial.available())
      Serial.read();
 
   // Write the value to the device register.
   int lengthTransferred = 0;
 
   // Write the device address.
   I2C_PRERlo = 50;
   I2C_PRERhi = 0;

   I2C_CR = CR_STA;
   while ((I2C_SR & SR_TIP) != 0)
   {
     //Serial.println(I2C_SR);
   }
     
   I2C_TXR = I2C_DEVICE_ADDRESS << 1;
   I2C_CR = CR_WR | CR_STA;
   while ((I2C_SR & SR_TIP) != 0)
   {
     //Serial.println(I2C_SR);
   }
 
   // Get the acknowledgement.
   lengthTransferred = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;
   
   Serial.println(I2C_SR, HEX);
   if (lengthTransferred == 1)
   {
      // Write the register.
      I2C_TXR = REG_DIV1;
      I2C_CR = CR_WR;
      while ((I2C_SR & SR_TIP) != 0);
 
      // Get the acknowledgement.
      lengthTransferred = ((I2C_SR & SR_RxACK) == 0) ? 2 : 0;
   }
 
   if (lengthTransferred == 2)
   {
      // Write the value.
      I2C_TXR = count;
      I2C_CR = CR_WR | CR_STO;
      while ((I2C_SR & SR_TIP) != 0);
 
      // Get the acknowledgement.
      lengthTransferred = ((I2C_SR & SR_RxACK) == 0) ? 3 : 0;
   }
 
   Serial.print("Length transferred during write = ");
   Serial.print(lengthTransferred);
   Serial.print("\r\n");
 
   // Read the value back from the device.
   lengthTransferred = 0;
 
   // Write the device address.
   I2C_TXR = (I2C_DEVICE_ADDRESS << 1);
   I2C_CR = CR_WR | CR_STA;
   while ((I2C_SR & SR_TIP) != 0);
 
   // Get the acknowledgement.
   lengthTransferred = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;
   Serial.println(I2C_SR, HEX);
 
   // Write the register.
   if (lengthTransferred == 1)
   {
      // Write the register.
      I2C_TXR = REG_DIV1;
      I2C_CR = CR_WR;
      while ((I2C_SR & SR_TIP) != 0);
 
      // Get the acknowledgement.
      lengthTransferred = ((I2C_SR & SR_RxACK) == 0) ? 2 : 0;
   }
 
   // Write the read address.
   if (lengthTransferred == 2)
   {
      // Write the read address...
      I2C_TXR = (I2C_DEVICE_ADDRESS << 1) | 0x01;
      I2C_CR = CR_WR | CR_STA;
      while ((I2C_SR & SR_TIP) != 0);
 
      // Get the acknowledgement.
      lengthTransferred = ((I2C_SR & SR_RxACK) == 0) ? 3 : 0;
   }
 
   // Read the value.
   int value = 0;
   if (lengthTransferred = 3)
   {
      // Read the value.
      I2C_CR = CR_RD | CR_STO | CR_ACK;
      while ((I2C_SR & SR_TIP) != 0);
      value = I2C_RXR;
      lengthTransferred = 4;
   }
   else
   {
      I2C_CR = CR_STO;
      while ((I2C_SR & SR_TIP) != 0);
   }
 
   Serial.print("Length transferred during read = ");
   Serial.print(lengthTransferred);
   Serial.print("\r\n");
   Serial.print("Value read = ");
   Serial.print(value);
   Serial.print("\r\n");
   count++;
}

