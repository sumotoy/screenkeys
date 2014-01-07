/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	screenkeys - A Library and Hardware (For Teensy3.x) to drive 1..64 LCD ScreenKeys buttons with 6..7 wires
---------------------------------------------------------------------------------------------------------------------
Version history:
0.3a3: alpha version, still in deep changes
---------------------------------------------------------------------------------------------------------------------
		Copyright (c) 2014, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
---------------------------------------------------------------------------------------------------------------------

    screenkeys Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    screenkeys Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

	Small code portions from Adafruit (Adafruit_GFX).
	https://github.com/adafruit/Adafruit-GFX-Library
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.3a3
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Attention! This library needs gpio_expander library
	https://github.com/sumotoy/gpio_expander
	-------------------------------------------------------------
	Hardware:
	Will coming soon...
*/

#ifndef _SCREENKEYS_XX_H_
#define _SCREENKEYS_XX_H_

#include <inttypes.h>
#include <Arduino.h>
#include "Print.h"

#define swap(a, b) { int8_t t = a; a = b; b = t; }

//#define _DBG

//Pixel engine write directly in a buffer, here you can choose
//the preferred method. Some font it's reversed or specular so the engine can
//switch method on the fly and come back to the default method.
//Do not touch these numbers
#define TL_ORIGIN		0
#define BR_ORIGIN		1
#define BL_ORIGIN 		2//normal operations
#define TR_ORIGIN		3

//here you can choose the preferred origin buffer write method
#define DEFAULT_BUFFER_ADDRESSING	BL_ORIGIN

#define BLACK 0
#define WHITE 1

class screenkeys : public Print {

public:
	screenkeys( );
	virtual void 	begin(bool protocolInitOverride=false) = 0;
	uint8_t 		getWidth(void);
	uint8_t 		getHeight(void);
	uint8_t 		getRotation(void);
	void 			setRotation(uint8_t val);
	void 			setTextWrap(boolean w);
	void 			setTextSize(uint8_t s);
	void 			setTextColor(uint8_t c);
	void 			setCursor(uint8_t x, uint8_t y);
	void 			drawPixel(uint8_t x, uint8_t y, bool color=BLACK);
	void 			drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,bool color=BLACK);
	void 			drawFastVLine(uint8_t x,uint8_t y,uint8_t h,bool color=BLACK);
	void 			drawFastHLine(uint8_t x,uint8_t y,uint8_t w,bool color=BLACK);
	void 			drawRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool color=BLACK);
	void 			fillRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool color=BLACK);
	
	void 			drawCircle(uint8_t x0, uint8_t y0, uint16_t r,bool color=BLACK);
	
	void 			fillCircle(uint8_t x0, uint8_t y0, uint16_t r,bool color=BLACK);
	void 			drawRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, bool color=BLACK);
	void 			fillRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, bool color=BLACK);
	void 			setFont(const unsigned char * fnt);
	virtual size_t  write(uint8_t);
	#if defined(_DBG)
	uint8_t			myDbg();
	#endif

protected:
	uint8_t 		_width;
	uint8_t 		_height;
	uint8_t 		_cursor_x;
	uint8_t 		_cursor_y;
	uint8_t 		_rotation;
	uint8_t 		_textsize;
	bool			_txtColour;
	boolean 		_wrap;
	uint8_t 		_XRES;
	uint8_t 		_YRES;
	uint8_t 		_buffer[64];
	uint8_t			_bufferAddressing;
	const unsigned char * _font;
	#if defined(_DBG)
	uint8_t			_dbg;
	#endif
private:
	void 			drawCircleHelper(uint8_t x0,uint8_t y0,uint8_t r,uint8_t cornername,bool color);
	void 			fillCircleHelper(uint8_t x0, uint8_t y0, uint16_t r, uint8_t cornername, int16_t delta, bool color);
	void 			drawChar(uint8_t x, uint8_t y, unsigned char c, uint8_t fontW, uint8_t fontH, bool color, uint8_t size);
	void			clearBuffer();
	#if defined(_DBG)
	void 			setDbg(uint8_t data);
	#endif
};

#endif