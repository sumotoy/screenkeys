/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	screenkeys - A Library and Hardware (For Teensy3.x) to drive 1..64 LCD ScreenKeys buttons with 6..7 wires
---------------------------------------------------------------------------------------------------------------------
Version history:
0.3a1: alpha version, still in deep changes
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
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.3a1
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

#define swap(a, b) { int8_t t = a; a = b; b = t; }

/*
#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
*/

//#define TL_ORIGIN
//#define BR_ORIGIN
#define BL_ORIGIN //choose this !!!!
//#define TR_ORIGIN


class screenkeys {

public:
	screenkeys( );
	virtual void 	begin(bool protocolInitOverride=false) = 0;
	uint8_t 		getWidth(void);
	uint8_t 		getHeight(void);
	uint8_t 		getRotation(void);
	void 			setRotation(uint8_t val);
	void 			drawPixel(uint8_t x, uint8_t y, uint8_t color);
	void 			drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color);
	void 			drawFastVLine(uint8_t x,uint8_t y,uint8_t h,uint8_t color);
	void 			drawFastHLine(uint8_t x,uint8_t y,uint8_t w,uint8_t color);
	void 			drawRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t color);
	void 			fillRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t color);
	
	void 			drawCircle(uint8_t x0, uint8_t y0, uint16_t r,uint8_t color);
	
	void 			fillCircle(uint8_t x0, uint8_t y0, uint16_t r,uint8_t color);
	void 			drawRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, uint8_t color);
	void 			fillRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, uint8_t color);
	void 			setFont(const unsigned char * f);
	
	void 			printChar(uint8_t x, uint8_t y, unsigned char c,uint8_t colour,uint8_t sze);
protected:
	uint8_t 		_width;
	uint8_t 		_height;
	uint8_t 		_cursor_x;
	uint8_t 		_cursor_y;
	uint8_t 		_rotation;
	uint8_t 		_textsize;
	boolean 		_wrap;
	uint8_t 		_XRES;
	uint8_t 		_YRES;
	uint8_t 		_buffer[];
	const unsigned char * _font;
private:
	void 			drawCircleHelper(uint8_t x0,uint8_t y0,uint8_t r,uint8_t cornername,uint8_t color);
	void 			fillCircleHelper(uint8_t x0, uint8_t y0, uint16_t r, uint8_t cornername, int16_t delta, uint8_t color);
	void 			writeBitmap(uint8_t x,uint8_t y,const unsigned char* bmp,uint16_t i,uint8_t wi,uint8_t he,uint8_t colour,uint8_t sze);
};

#endif