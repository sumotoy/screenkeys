


#ifndef _LC16_H_
#define _LC16_H_

#include <inttypes.h>

#include "screenkeys.h"
#include <../SPI/SPI.h>//this chip needs SPI
#include <../gpio_expander/mcp23s17.h>

#define MAIN_CLK_PIN	4

#define BL_NONE    0x00
#define BR_GREEN   0x33
#define BR_RED	   0xCC
#define BR_ORANGE  0xFF
#define DK_GREEN   0x03
#define DK_RED     0x0C
#define DK_ORANGE  0x0F
#define XRES	   32
#define YRES       16
#define CLK_FRQ    400000L


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
private:
    uint8_t 		_cs;
	uint8_t 		_mainClock;
	uint8_t 		_programClock;
	uint8_t 		_clockEnable;
	uint8_t 		_adrs;
	uint8_t 		_units;
	uint8_t 		_error;
	uint8_t			_chips;
	unsigned char 	_key_buff[64];
};
#endif
