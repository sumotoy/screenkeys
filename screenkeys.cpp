#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <Arduino.h>
#include "screenkeys.h"

#include "glcdfont.c"

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

screenkeys::screenkeys() 
{
	_width = 0;//mod by rotation
	_height = 0;//mod by rotation
	_cursor_x = 0;
	_cursor_y = 0;
	_rotation = 0;
	_textsize = 1;
	_txtColour = 0;//black
	_wrap = true;
	clearBuffer();
	_bufferAddressing = DEFAULT_BUFFER_ADDRESSING;
	
	setFont(font5x7);
	#if defined(_DBG)
	_dbg = 1;
	#endif
}

void screenkeys::clearBuffer() {
	for (uint8_t i=0;i<64;i++){
		_buffer[i] = 0x00;
	}	
}

uint8_t screenkeys::getWidth(void) {
  return _width;
}

uint8_t screenkeys::getHeight(void) {
  return _height;
}

uint8_t screenkeys::getRotation(void) {
  return _rotation;
}

void screenkeys::setTextWrap(boolean w) {
  _wrap = w;
}

void screenkeys::setTextSize(uint8_t s) {
  _textsize = (s > 0) ? s : 1;
}

void screenkeys::setTextColor(uint8_t c) {
	if (c > 1) c = 1;
	_txtColour = c;
}

void screenkeys::setRotation(uint8_t val) {
  _rotation = (val & 3);
  switch(_rotation) {
  case 0:
  case 2:
    _width  = _XRES;
    _height = _YRES;
    break;
  case 1:
  case 3:
    _width  = _XRES;
    _height = _YRES;
    break;
  }
}

void screenkeys::drawPixel(uint8_t x, uint8_t y, bool color) {
	if ((x >= getWidth()) || (y >= getHeight())) return;
	uint8_t xbyte;
	switch (getRotation()) {
	case 1:
		swap(x, y);
		x = _XRES - x - 1;
		break;
	case 2:
		x = _XRES - x - 1;
		y = _YRES - y - 1;
		break;
	case 3:
		swap(x, y);
		y = _YRES - y - 1;
		break;
  }  
  //pixel engine can be referenced from any angle of the LCD
	if (_bufferAddressing == TL_ORIGIN){
  //0,0 on the Top Left
		if (x < 8) {
			xbyte = 0;
		} else if (x < 16) {
			xbyte = 1;
		} else if (x < 24) {
			xbyte = 2;
		} else {
			xbyte = 3;
		}
		bitWrite(_buffer[xbyte+(y*4)],x-(xbyte*8),!color);
	} else if (_bufferAddressing == BR_ORIGIN){
  //0,0 on Bottom Right
		if (x < 8) {
			xbyte = 63;
		} else if (x < 16) {
			xbyte = 62;
		} else if (x < 24) {
			xbyte = 61;
		} else {
			xbyte = 60;
		}
		bitWrite(_buffer[xbyte-(y*4)],(7-x)-((xbyte-63)*8),!color);
	} else if (_bufferAddressing == BL_ORIGIN){
  //0,0 on Bottom Left
		if (x < 8) {
			xbyte = 60;
		} else if (x < 16) {
			xbyte = 61;
		} else if (x < 24) {
			xbyte = 62;
		} else {
			xbyte = 63;
		}
		bitWrite(_buffer[xbyte-(y*4)],x-((xbyte-60)*8),!color);
	} else {//TR_ORIGIN
  //0,0 on Top Right
		if (x < 8) {
			xbyte = 3;
		} else if (x < 16) {
			xbyte = 2;
		} else if (x < 24) {
			xbyte = 1;
		} else {
			xbyte = 0;
		}
		bitWrite(_buffer[xbyte+(y*4)],(7-x)-((xbyte-3)*8),!color);
	}
}

void screenkeys::drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,bool color) {
	if ((x0 >= getWidth()) || (y0 >= getHeight()) || (x1 >= getWidth()) || (y1 >= getHeight())) return;
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);
	uint8_t err = dx / 2;
	uint8_t ystep;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	for (; x0<=x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void screenkeys::drawFastVLine(uint8_t x,uint8_t y,uint8_t h,bool color) {
	drawLine(x,y,x,y+h-1,color);
}

void screenkeys::drawFastHLine(uint8_t x,uint8_t y,uint8_t w,bool color) {
	drawLine(x,y,x+w-1,y,color);
}

void screenkeys::drawRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h-1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x+w-1, y, h, color);
}

void screenkeys::fillRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool color) {
	uint8_t i;
	for (i=x; i<x+w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

void screenkeys::drawCircleHelper(uint8_t x0,uint8_t y0,uint8_t r,uint8_t cornername,bool color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		} 
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void screenkeys::drawCircle(uint8_t x0, uint8_t y0, uint16_t r,bool color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0  , y0+r, color);
	drawPixel(x0  , y0-r, color);
	drawPixel(x0+r, y0  , color);
	drawPixel(x0-r, y0  , color);
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void screenkeys::fillCircleHelper(uint8_t x0, uint8_t y0, uint16_t r, uint8_t cornername, int16_t delta, bool color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x1) {
			drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
			drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
			drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}

void screenkeys::fillCircle(uint8_t x0, uint8_t y0, uint16_t r,bool color) {
	drawFastVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}


void screenkeys::drawRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, bool color) {
  // smarter version
	drawFastHLine(x+r  , y    , w-2*r, color); // Top
	drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
	drawFastVLine(x    , y+r  , h-2*r, color); // Left
	drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1, color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}


void screenkeys::fillRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, bool color) {
  // smarter version
	fillRect(x+r, y, w-2*r, h, color);
  // draw four corners
	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}



void screenkeys::setCursor(uint8_t x, uint8_t y) {
	_cursor_x = x;
	_cursor_y = y;
}

void screenkeys::setFont(const unsigned char * fnt) {
	_font = fnt;
}

#if defined(_DBG)
void screenkeys::setDbg(uint8_t data){
	_dbg = data;
}

uint8_t screenkeys::myDbg(){
	//setDbg(pgm_read_byte(font+1));
	return _dbg;
}
#endif

void screenkeys::drawChar(uint8_t x, uint8_t y, unsigned char c, uint8_t fontW, uint8_t fontH, bool color, uint8_t size) {
  // Clip right // Clip bottom  // Clip left  // Clip top
  if ((x >= _width) || (y >= _height) || (((x + (fontW+1)) * (size - 1)) < 0) || (((y + (fontH+1)) * (size - 1)) < 0)) return;
  uint8_t i,j;
  _bufferAddressing = pgm_read_byte(_font+2);;//How to write this font? This will instruct the pixel engine.
  for (i = 0;i <= fontW;i++) {
		uint8_t line;
		if (i == fontW) {
			line = 0x0;
		} else {
			line = pgm_read_byte((_font+(c*fontW)+i)+3);
		}
		for (j = 0; j < 8; j++) {
			if (line & 0x1) {
				if (size == 1) {
					drawPixel(x+i, y+j, color);
				} else {  // big size
					fillRect(x+(i*size), y+(j*size), size, size, color);
				} 
			}
			line >>= 1;
		}
	}
	_bufferAddressing = DEFAULT_BUFFER_ADDRESSING;//go back to choosed addressing
}

size_t screenkeys::write(uint8_t c) {
	uint8_t fontW = pgm_read_byte(_font);//5
	uint8_t fontH = pgm_read_byte(_font+1);//7
	if (c == '\n') {
		_cursor_y += _textsize * (fontH + 1);
		_cursor_x  = 0;
	} else if (c == '\r') {
    // skip em
	} else {
		if (_cursor_x < fontW) _cursor_x = fontW + _cursor_x;
		if (_cursor_y < fontH) _cursor_y = fontH + _cursor_y;
		drawChar(_cursor_x, _cursor_y, c, fontW,fontH,_txtColour, _textsize);
		_cursor_x += _textsize * (fontW + 1);
		if (_wrap && (_cursor_x > (_width - (_textsize * (fontW + 1))))) {
			_cursor_y += _textsize * (fontH + 1);
			_cursor_x = 0;
		}
	}
	return 1;
}
