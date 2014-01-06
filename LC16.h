#ifndef _LC16_H_
#define _LC16_H_

#include <inttypes.h>

#include "screenkeys.h"
#include <../SPI/SPI.h>//this chip needs SPI
#include <../gpio_expander/mcp23s17.h>

//witch pin for use internal clock generator
#define MAIN_CLK_PIN	4
//main clock frequency for the LCD's
#define CLK_FRQ    400000L

// colors for LC16.2
#define BL_NONE    0x00
#define BR_GREEN   0x33
#define BR_RED	   0xCC
#define BR_ORANGE  0xFF
#define DK_GREEN   0x03
#define DK_RED     0x0C
#define DK_ORANGE  0x0F
// LC16.2 resolution
#define XRES	   32
#define YRES       16
// LC16 registers
#define LC_FREQREG  0xEE
#define LC_MUXREG   0xEF
#define LC_COLREG   0xED
#define LC_PIXREG   0x80
#define LC_STARTRG  0x00
#define LC_STOPTRG  0xAA

class LC16 : public screenkeys
{

public:
	LC16(const uint8_t switches,const uint8_t csPin,const uint8_t startAdrs,const uint8_t prgClock,const uint8_t clkEnable);
	virtual void 	begin(bool protocolInitOverride=false);
	void			refresh(uint8_t key);
	void 			printImage(uint8_t key, unsigned char * data);
	void 			setColor(uint8_t key,byte color);
	void 			fill(uint8_t key,uint8_t color);
	void 			clear(uint8_t key);
	uint8_t			getError();
private:
    uint8_t 		_cs;
	uint8_t 		_mainClock;
	uint8_t 		_programClock;
	uint8_t 		_clockEnable;
	uint8_t 		_adrs;
	uint8_t 		_units;
	uint8_t 		_error;
	uint8_t			_chips;

	uint8_t 		_buffer[64] = { 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
	mcp23s17		mcp;
	void 			progEnable(boolean mode);
	void 			PClock(uint8_t cTicks);
	void 			sendData(uint8_t key,unsigned char cDataByte, unsigned char cParity);
	void 			start(uint8_t key);
	void 			stop(uint8_t key);
	void 			sendByte(uint8_t key,byte reg,byte val);
	void 			sendWord(uint8_t key,byte reg,byte val1,byte val2);
	void 			sk_initialize(uint8_t key);
};
#endif
