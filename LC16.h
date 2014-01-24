#ifndef _LC16_H_
#define _LC16_H_

#include <inttypes.h>

#include "screenkeys.h"
#include <../SPI/SPI.h>//this chip needs SPI
#include <../gpio_expander/mcp23s17.h>

//----------------------------------------- Do NOT touch following
// colors for LC16.2
#define BL_NONE    0x00
#define BR_GREEN   0x33
#define BR_RED	   0xCC
#define BR_ORANGE  0xFF
#define DK_GREEN   0x03
#define DK_RED     0x0C
#define DK_ORANGE  0x0F
// LC16.2 specifics
#define LC16_XRES	32//in pixels
#define LC16_YRES   16//in pixels
#define	LC16_BFSIZE	64//(16*32)/8 = 64 bytes, this is the buffer for this screen!
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
	virtual void 			begin(bool protocolInitOverride=false);
	void					refresh(uint8_t key);
	void 					printImage(uint8_t key, unsigned char * data);
	void 					setColor(uint8_t key,byte color);
	void 					fill(uint8_t color);
	void 					clear();
	uint8_t					getError();
#ifndef EXTSWITCH
	uint8_t					keypressScan();
    void 					enableKeyInt(void (*isr)()) __attribute__((always_inline)) {
		attachInterrupt(INTused,*isr,FALLING);
    }
    void 					disableKeyInt() __attribute__((always_inline)) {
		detachInterrupt(INTused);
    }
	static void				keypress();
#endif
protected:
#ifndef EXTSWITCH
	static volatile  bool 	_keyPressed;
#endif
private:
    uint8_t 				_cs;
#ifndef __USEEXTCLK
	uint8_t 				_mainClock;
#endif
	uint8_t 				_programClock;
	uint8_t 				_clockEnable;
	uint8_t 				_adrs;
	uint8_t 				_units;
	uint8_t 				_error;
	void					_sendGpio(uint8_t key,uint16_t data);
	uint8_t					_witchGpio(uint8_t key);
	
// Instances for the esternal library....I know, it's messy!
	mcp23s17				mcp1;
#if SWITCHLIMIT <= 0
	#define		SWGPIOS		0
	#error 		You must specify a range between 1 to 64!
#elif SWITCHLIMIT > 0 && SWITCHLIMIT < 9 //1..8
	#define		SWGPIOS		1
	mcp23s17 _gpios_out[1] = {mcp1};
#elif SWITCHLIMIT > 8 && SWITCHLIMIT < 17 //9..16
	mcp23s17		mcp2;
	#define		SWGPIOS		2
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp2};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2};
	#endif
#elif SWITCHLIMIT > 16 && SWITCHLIMIT < 25//17..24
	mcp23s17		mcp2;
	mcp23s17		mcp3;
	mcp23s17		mcp4;
	#define		SWGPIOS		4//for semplicity, should be 3
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1,mcp2};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp3,mcp4};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2,mcp3,mcp4};
	#endif
#elif SWITCHLIMIT > 24 && SWITCHLIMIT < 33//25..32
	mcp23s17		mcp2;
	mcp23s17		mcp3;
	mcp23s17		mcp4;
	#define		SWGPIOS		4
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1,mcp2};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp3,mcp4};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2,mcp3,mcp4};
	#endif
#elif SWITCHLIMIT > 32 && SWITCHLIMIT < 41//33..40
	mcp23s17		mcp2;
	mcp23s17		mcp3;
	mcp23s17		mcp4;
	mcp23s17		mcp5;
	mcp23s17		mcp6;
	#define		SWGPIOS		6//for semplicity, should be 5
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1,mcp2,mcp3};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp4,mcp5,mcp6};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2,mcp3,mcp4,mcp5,mcp6};
	#endif
#elif SWITCHLIMIT > 40 && SWITCHLIMIT < 49//41..48
	mcp23s17		mcp2;
	mcp23s17		mcp3;
	mcp23s17		mcp4;
	mcp23s17		mcp5;
	mcp23s17		mcp6;
	#define		SWGPIOS		6
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1,mcp2,mcp3};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp4,mcp5,mcp6};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2,mcp3,mcp4,mcp5,mcp6};
	#endif
#elif SWITCHLIMIT > 48 && SWITCHLIMIT < 57//49..56
	mcp23s17		mcp2;
	mcp23s17		mcp3;
	mcp23s17		mcp4;
	mcp23s17		mcp5;
	mcp23s17		mcp6;
	mcp23s17		mcp7;
	mcp23s17		mcp8;
	#define		SWGPIOS		8//for semplicity, should be 7
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1,mcp2,mcp3,mcp4};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp5,mcp6,mcp7,mcp8};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2,mcp3,mcp4,mcp5,mcp6,mcp7,mcp8};
	#endif
#elif SWITCHLIMIT > 56 && SWITCHLIMIT < 65//57..64
	mcp23s17		mcp2;
	mcp23s17		mcp3;
	mcp23s17		mcp4;
	mcp23s17		mcp5;
	mcp23s17		mcp6;
	mcp23s17		mcp7;
	mcp23s17		mcp8;
	#define		SWGPIOS		8
	#ifndef __USEEXTCLK
	mcp23s17 _gpios_out[SWGPIOS/2] = {mcp1,mcp2,mcp3,mcp4};
	mcp23s17 _gpios_in [SWGPIOS/2] = {mcp5,mcp6,mcp7,mcp8};
	#else
	mcp23s17 _gpios_out[SWGPIOS] = {mcp1,mcp2,mcp3,mcp4,mcp5,mcp6,mcp7,mcp8};
	#endif
#else
	#define		SWGPIOS		0
	#error 		You must specify a range between 1 to 64!
#endif
	//void 				progEnable(boolean mode);
	void 				PClock(uint8_t cTicks);
	void 				sendData(uint8_t key,unsigned char cDataByte, unsigned char cParity);
	void 				sendByte(uint8_t key,byte reg,byte val);
	void 				sendWord(uint8_t key,byte reg,byte val1,byte val2);
	void 				init_lcdChip(uint8_t key);
};
#endif
