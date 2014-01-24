/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	screenkeys - A Library & Hardware (For Teensy3.x) for drive 1..64 LCD ScreenKeys buttons with few wires
---------------------------------------------------------------------------------------------------------------------
Version history:
0.6b1
---------------------------------------------------------------------------------------------------------------------
		Copyright (c) 2014, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
---------------------------------------------------------------------------------------------------------------------

    screenkeys Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version. You cannot use for commercial producs
	or military purposes!

    screenkeys Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

	Small code portions from Adafruit (Adafruit_GFX).
	https://github.com/adafruit/Adafruit-GFX-Library
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.6b1
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Attention! This library needs sumotoy gpio_expander library
	https://github.com/sumotoy/gpio_expander
	-------------------------------------------------------------
	Hardware needed:
	schematics coming soon...
	-------------------------------------------------------------
	Pins used:
	MOSI,MISO,SCK, a common CS pin for all GPIO's,an INT and a selectable pin for CLK
	So 3 can be shared with other stuff, only 3 are really necessary!!!
*/

#ifndef _SCREENKEYS_XX_H_
#define _SCREENKEYS_XX_H_

#include <inttypes.h>
#include <Arduino.h>
#include "Print.h"
//#include "membuf.h"

#define swap(a, b) { int8_t t = a; a = b; b = t; }

#if defined(__AVR__) || (defined(ARDUINO) && ARDUINO > 152) || defined(CORE_TEENSY)
	#include <avr/pgmspace.h>
#else // ARM
// Stub out some other common PROGMEM functions
	#define PROGMEM
	#define memcpy_P memcpy
	static inline uint8_t pgm_read_byte(const void *addr) { return *((uint8_t *)addr); }
#endif
#define nop asm volatile ("nop\n\t")

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
........................... GPIO CONNECTIONS .............................
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
//---------------------------- 1 GPIO mode (8 switches)
//all switches data to GPIO I/O Bank A
//all switches SWA tied to GND, a pullup resistor of 10K to each pin of GPIO I/O Bank B and SWB tied to GPIO I/O Bank B
//connect INT B pin from GPIO to a pullup resistor (10k) then connect to choosed Teensy3.1 INT pin (or use a level converter for Teensy3.0)
//---------------------------- Multiple GPIO mode (>8 to 64 switches)
//if 4 GPIO connected, 2 GPIO dedicated to DATA e 2 to Switches (32 switches total)
//First 2 GPIO are for DATA
//3rd and 4rth are for switches scan, connect a 10K resistor pullup on each GPIO pin to avoid noise influence.
//connect INT A pin of GPIO 3 and GPIO 4 together and a pullup resistor (10K), then connect to choosed Teensy3.1 INT pin (or use a level converter for Teensy3.0)
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
........................... SETUP .............................
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
//------------------ External Clock Enable -----------------
//if you are using an external clock and you don't want generate internally.

#define	__USEEXTCLK//if defined you force library to don't generate it(1 pin less)

//witch pin for use internal clock generator (if you are not using an external generator)
#if defined(__USEEXTCLK)
	#define MAIN_CLK_PIN	4
//main clock frequency for the LCD's
	#define CLK_FRQ    		400000L//400Khz
#endif
//------------------ Set clock divisor in relation of the choosed scan frequency -----------------
//You need to set the following accordly the clock frequency (even if clock it's external!)
//This is needed always!
#define		SCRKEY_CLK		0x60//For 400Khz! Read datasheet if you change the clock speed
//------------------ Debug Routines ------------------------
//this is used only for debug purposes

//#define 	_DBG

//----------------- Text Options ---------------------------
//there's a small routine that clear before write any char, useful for mainly large screens

//#define _PRECLN

//----------------- Switches Engine ---------------------------
//You MUST specify here the max number of switches your design it's using!
//Set 0 if you plan to use an external way to check switches

#define	SWITCHLIMIT			8 // 0:disable switches, 1...64 range

//----------------- Switches Handle ---------------------------
//Here you can define if you plan to use an external way to handle the switches.
//In this way the library handle only LCD and you provide a custom way to handle switches

//#define	EXTSWITCH	//When defined it will enable an external handle of switch
//------------------ INT PIN and INT --------------------------
//scan engine uses an INT and PIN to detect a keypress, here you choose
#ifndef EXTSWITCH
	#define INTused 		2  // witch int you plan to use
	#define INTpin 			2  // the pin associated to the int
#endif
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
........................... END SETUP .............................
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
// ---------------- Pixel Engine ---------------------------
//Pixel engine write directly in a buffer, here you can choose
//the preferred method. Some font it's reversed or specular so the engine can
//switch method on the fly and come back to the default method.
//Do not touch following numbers

//#define TL_ORIGIN//		0
//#define BR_ORIGIN//		1
#define BL_ORIGIN// 		2//normal operations
//#define TR_ORIGIN//		3
//-----------------------------------------------------------

#define BLACK 0
#define WHITE 1

class screenkeys : public Print {

public:
	screenkeys( );
	virtual void 			begin(bool protocolInitOverride=false) = 0;
	uint8_t 				getWidth(void);
	uint8_t 				getHeight(void);
	uint8_t 				getRotation(void);
	void 					setRotation(uint8_t val);
	void 					setTextWrap(boolean w);
	void 					setTextSize(uint8_t s);
	void 					setTextColor(uint8_t c);
	void 					setCursor(uint8_t x, uint8_t y);
	void 					drawPixel(uint8_t x, uint8_t y, bool color=BLACK);
	void 					drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,bool color=BLACK);
	void 					drawFastVLine(uint8_t x,uint8_t y,uint8_t h,bool color=BLACK);
	void 					drawFastHLine(uint8_t x,uint8_t y,uint8_t w,bool color=BLACK);
	void 					drawRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool color=BLACK);
	void 					fillRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool color=BLACK);
	void 					drawCircle(uint8_t x0, uint8_t y0, uint16_t r,bool color=BLACK);
	void 					fillCircle(uint8_t x0, uint8_t y0, uint16_t r,bool color=BLACK);
	void 					drawRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, bool color=BLACK);
	void 					fillRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, bool color=BLACK);
	void 					setFont(const unsigned char * fnt);
	virtual size_t  		write(uint8_t);
#if defined(_DBG)
	uint8_t					myDbg();
#endif
	void 					invert();
protected:
	uint8_t 				_width;//this can be different if rotation is set
	uint8_t 				_height;//this can be different if rotation is set
	uint8_t 				_cursor_x;
	uint8_t 				_cursor_y;
	uint8_t 				_rotation;
	uint8_t 				_textsize;
	bool					_foreground;
	bool					_background;
	boolean 				_wrap;
	uint8_t 				_XRES;//This is specific of the Screenkey used. It's the X resolution
	uint8_t 				_YRES;//This is specific of the Screenkey used. It's the Y resolution
	//volatile uint8_t 		_buffer[64];
	static volatile uint8_t _buffer[];
	uint8_t					_bufferAddressing;
	const unsigned char * 	_font;
	uint8_t					_switches;
#if defined(_DBG)
	uint8_t					_dbg;
#endif
private:
	void 					drawCircleHelper(uint8_t x0,uint8_t y0,uint8_t r,uint8_t cornername,bool color);
	void 					fillCircleHelper(uint8_t x0, uint8_t y0, uint16_t r, uint8_t cornername, int16_t delta, bool color);
	uint8_t 				drawChar(uint8_t x,uint8_t y, unsigned char c,bool colour, bool background=WHITE);
	uint8_t 				charWidth(unsigned char c);
	//void					clearBuffer();
	
#if defined(_DBG)
	void 					setDbg(uint8_t data);
#endif
	struct FontHeader {
		uint16_t 			size;			//0
		uint8_t 			fixedWidth;		//1
		uint8_t 			height;			//2
		uint8_t 			firstChar;		//3
		uint8_t 			charCount;		//4
	};
};

#endif