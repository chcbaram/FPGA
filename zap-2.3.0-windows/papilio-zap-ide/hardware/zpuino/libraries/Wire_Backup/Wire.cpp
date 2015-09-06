/******************************************************************************
 * The MIT License
 *
 *****************************************************************************/

/**
 *  @brief Wire library, ported from Arduino orginally by Leaf Labs. Provides a simplistic
 *  interface to i2c.
 *  Substantially rewritten after many hours with a logic analyzer by Martin Mason for the CM-900
 *  SDA and SCL must be tied high with ~2K resitors as SDA and SCL are tri-state.
 */

#include "Wire.h"
//#include "wirish.h"


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






/* low level conventions:
 * - SDA/SCL idle high (expected high)
 * - always start with i2c_delay rather than end
 */
uint32 i2c_delay = 1;

void i2c_start(Port port) {
    
/*
    I2C_DELAY;
    digitalWrite(port.sda,LOW);
    I2C_DELAY;
    digitalWrite(port.scl,LOW);
	I2C_DELAY;
*/
    I2C_CR = CR_STA;
}

void i2c_stop(Port port) {
    /*
	digitalWrite(port.sda,LOW);		//stm32, bug on generating stop bit
    I2C_DELAY;
    digitalWrite(port.scl,HIGH);
	I2C_DELAY;
    digitalWrite(port.sda,HIGH);
    */
    I2C_CR = CR_STO;
}

boolean i2c_get_ack(Port port) {

/*
    digitalWrite(port.scl,LOW);
    digitalWrite(port.sda,HIGH);
    I2C_DELAY;
//    pinMode(sda,INPUT);			//stm32
//    pinMode(sda, OUTPUT_OPEN_DRAIN);	//stm32
    digitalWrite(port.scl,HIGH);

    if (!digitalRead(port.sda)) {
      I2C_DELAY;
      digitalWrite(port.scl,LOW);
	  I2C_DELAY;
	  digitalWrite(port.sda,HIGH);		//stm32, bug!
//	  digitalWrite(port.sda,LOW);		//stm32
      return true;
    } else {
      I2C_DELAY;
      digitalWrite(port.scl,LOW);
	  I2C_DELAY;
	  digitalWrite(port.sda,HIGH);
      return false;
    }
*/
    boolean Ret;

    Ret = ((I2C_SR & SR_RxACK) == 0) ? 1 : 0;

    return Ret;
}


void i2c_write_ack(Port port) {
    /*
    I2C_DELAY;
    digitalWrite(port.scl,HIGH);
    I2C_DELAY;
	digitalWrite(port.scl,LOW);
    digitalWrite(port.sda,HIGH);
    I2C_DELAY;
    digitalWrite(port.sda,LOW);
    */
}

void i2c_write_nack(Port port) {	
/*
	digitalWrite(port.sda,LOW); //LOW
	I2C_DELAY;
	digitalWrite(port.scl,HIGH);
    I2C_DELAY;
	digitalWrite(port.scl,LOW);
	digitalWrite(port.sda,HIGH);
*/    
}

uint8 i2c_shift_in(Port port) {
    uint8 data = 0;
    int i;

/*
    for (i=0;i<8;i++) {
        I2C_DELAY;
        digitalWrite(port.scl,HIGH);
        I2C_DELAY;
        data += digitalRead(port.sda) << (7-i);
        digitalWrite(port.scl,LOW);
    }
*/

    return data;
}

void i2c_shift_out(Port port, uint8 val) {
    int i;

    for (i=0;i<8;i++) {
        I2C_DELAY;
        digitalWrite(port.sda, !!(val & (1 << (7 - i))));
        I2C_DELAY;
        digitalWrite(port.scl, HIGH);
        I2C_DELAY;
        digitalWrite(port.scl, LOW);
    }
}

TwoWire::TwoWire() {
    i2c_delay = 0;
    rx_buf_idx = 0;
    rx_buf_len = 0;
    tx_addr = 0;
    tx_buf_idx = 0;
    tx_buf_overflow = false;
}

/*
 * Sets pins SDA and SCL to OUPTUT_OPEN_DRAIN, joining I2C bus as
 * master.  If you want them to be some other pins, use begin(uint8,
 * uint8);
 */
void TwoWire::begin() {
    begin(SDA, SCL);    
}

/*
 * Joins I2C bus as master on given SDA and SCL pins.
 */
void TwoWire::begin(uint8 sda, uint8 scl) {
    port.sda = sda;
    port.scl = scl;
    /*
    pinMode(scl, OUTPUT_OPEN_DRAIN);
    pinMode(sda, OUTPUT_OPEN_DRAIN);
    digitalWrite(scl, HIGH);
    digitalWrite(sda, HIGH);
    */

    I2C_CTR = CTR_EN;
    I2C_PRERlo = 50;
    I2C_PRERhi = 0;    
}

void TwoWire::beginTransmission(uint8 slave_address) {
    tx_addr = slave_address;
    tx_buf_idx = 0;
    tx_buf_overflow = false;
    rx_buf_idx = 0;
    rx_buf_len = 0;
}

void TwoWire::beginTransmission(int slave_address) {
    beginTransmission((uint8)slave_address);
}

uint8 TwoWire::endTransmission(void) {
    if (tx_buf_overflow) return EDATA;

/*
    i2c_start(port);

    i2c_shift_out(port, (tx_addr << 1) | I2C_WRITE); 
    if (!i2c_get_ack(port)) 
	{
	i2c_stop(port);
	return ENACKADDR;
	}
	
    // shift out the address we're transmitting to
    for (uint8 i = 0; i < tx_buf_idx; i++) {
        uint8 ret = writeOneByte(tx_buf[i]);
        if (ret) return ret;    // SUCCESS is 0
    }

    i2c_stop(port);

    tx_buf_idx = 0;
    tx_buf_overflow = false;
*/
    // Write the device address.
    I2C_TXR = (tx_addr << 1) | I2C_WRITE;
    I2C_CR = CR_WR | CR_STA;
    while ((I2C_SR & SR_TIP) != 0);


    if ( (I2C_SR & SR_RxACK) == 0 )
    {
        for (uint8 i = 0; i < tx_buf_idx; i++) 
        {
            uint8 ret = writeOneByte(tx_buf[i]);
            if (ret) return ret;    // SUCCESS is 0
        }         
    }
    else
    {
        I2C_CR = CR_STO;
        return ENACKADDR;
    }

    I2C_CR = CR_STO;
    while ((I2C_SR & SR_TIP) != 0);

    tx_buf_idx = 0;
    tx_buf_overflow = false;


    return SUCCESS;
}

uint8 TwoWire::requestFrom(uint8 address, int num_bytes) {
    if (num_bytes > WIRE_BUFSIZ) num_bytes = WIRE_BUFSIZ;
    rx_buf_idx = 0;
    rx_buf_len = 0;

    /*
	i2c_start(port);
    //Send the address
    i2c_shift_out(port, (address << 1) | I2C_READ);
	//Check the ack on the address
    if (!i2c_get_ack(port)) 
	{
	i2c_stop(port);
	return ENACKADDR;
	}    //Start reading data one byte at a time.  
	while (rx_buf_len < (num_bytes-1)) {
	*(rx_buf + rx_buf_len) = i2c_shift_in(port);
	//Write_nac generates a propr ack response from the device (9th clock pulse)
	i2c_write_nack(port);
	rx_buf_len++;
	}
	
	*(rx_buf + rx_buf_len++) = i2c_shift_in(port);	//STM32 bug rx_buf_len to rx_buf_len++
	//get_nac checks for a propr ack response from the device (9th clock pulse)
	i2c_get_ack(port);


	i2c_stop(port);
    */

    I2C_TXR = (address << 1) | I2C_READ;
    I2C_CR = CR_WR | CR_STA;
    while ((I2C_SR & SR_TIP) != 0);


    if ( (I2C_SR & SR_RxACK) == 0 )
    {
        while (rx_buf_len < (num_bytes)) 
        {
            I2C_CR = CR_RD;
            while ((I2C_SR & SR_TIP) != 0);

            *(rx_buf + rx_buf_len) = I2C_RXR;
            rx_buf_len++;
        }
    }
    else
    {
        I2C_CR = CR_STO;
        return ENACKADDR;
    }

    I2C_CR = CR_STO + CR_ACK;
    while ((I2C_SR & SR_TIP) != 0);

    return rx_buf_len;
}

uint8 TwoWire::requestFrom(int address, int numBytes) {
    return TwoWire::requestFrom((uint8)address, (uint8) numBytes);
}

void TwoWire::write(uint8 value) {
    if (tx_buf_idx == WIRE_BUFSIZ) {
        tx_buf_overflow = true;
        return;
    }

    tx_buf[tx_buf_idx++] = value;
}

void TwoWire::write(uint8* buf, int len) {
    for (uint8 i = 0; i < len; i++) write(buf[i]);
}

void TwoWire::write(int value) {
    write((uint8)value);
}

void TwoWire::write(int* buf, int len) {
    write((uint8*)buf, (uint8)len);
}

void TwoWire::write(char* buf) {
    uint8 *ptr = (uint8*)buf;
    while(*ptr) {
        write(*ptr);
        ptr++;
    }
}

uint8 TwoWire::available() {
    return rx_buf_len - rx_buf_idx;
}

uint8 TwoWire::read() {
    if (rx_buf_idx == rx_buf_len) return 0;
    return rx_buf[rx_buf_idx++];
}

// private methods

uint8 TwoWire::writeOneByte(uint8 byte) {
    
    // Write the register.
    I2C_TXR = byte;
    I2C_CR = CR_WR;
    while ((I2C_SR & SR_TIP) != 0);

    if( (I2C_SR & SR_RxACK) != 0 )
    {
        I2C_CR = CR_STO;
        return ENACKTRNS;
    }

    return SUCCESS;
}

uint8 TwoWire::readOneByte(uint8 address, uint8 *byte) {

/*
    //Shift in toggles the clock line 8 times leaving it low checking the 
	//Date line on each clock cycle
    *byte = i2c_shift_in(port);
	//Write_nac generates a propr ack response from the device (9th clock pulse)
	i2c_write_nack(port);
	//i2c_stop(port);
*/
    // Read the value.
    I2C_CR = CR_RD;
    while ((I2C_SR & SR_TIP) != 0);
    
    *byte = I2C_RXR;

    return SUCCESS;      // no real way of knowing, but be optimistic!
}

// Declare the instance that the users of the library can use
TwoWire Wire;

