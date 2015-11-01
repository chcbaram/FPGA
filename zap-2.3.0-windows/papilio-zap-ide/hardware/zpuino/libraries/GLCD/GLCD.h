
#ifndef _GLCD_H_
#define _GLCD_H_

#include <Arduino.h>
#include <inttypes.h>
#include "Print.h"

#ifndef SPI_H_INCLUDED

#include <SPI.h>
#endif


#define GLCD_CHAR_WIDTH		(240/8)
#define GLCD_CHAR_HEIGHT	(320/16)


class GLCD_Class : public Print {
public:

	bool IsInit;




	GLCD_Class();

	bool begin(void);
    void loop(void);

	void clear(void);
	void setCursor(uint8_t col, uint8_t row);
	void drawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pBuf );
	void drawGray(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pBuf );

	virtual size_t write(uint8_t);

private:
	u8 position_x;
	u8 position_y;
};

#endif

