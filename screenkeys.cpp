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

void screenkeys::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
	if ((x >= getWidth()) || (y >= getHeight())) return;
	uint8_t xbyte;
	// check rotation, move pixel around if necessary
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
	if (color == 0) {
		bitSet(_buffer[xbyte+(y*4)],x-(xbyte*8));
	} else {
		bitClear(_buffer[xbyte+(y*4)],x-(xbyte*8));
	}
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
	if (color == 0) {
		bitSet(_buffer[xbyte-(y*4)],(7-x)-((xbyte-63)*8));
	} else {
		bitClear(_buffer[xbyte-(y*4)],(7-x)-((xbyte-63)*8));
	}
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
	if (color == 0) {
		bitSet(_buffer[xbyte-(y*4)],x-((xbyte-60)*8));
	} else {
		bitClear(_buffer[xbyte-(y*4)],x-((xbyte-60)*8));
	}
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
	if (color == 0) {
		bitSet(_buffer[xbyte+(y*4)],(7-x)-((xbyte-3)*8));
	} else {
		bitClear(_buffer[xbyte+(y*4)],(7-x)-((xbyte-3)*8));
	}
	}
}

void screenkeys::drawLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color) {
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

void screenkeys::drawFastVLine(uint8_t x,uint8_t y,uint8_t h,uint8_t color) {
	drawLine(x,y,x,y+h-1,color);
}

void screenkeys::drawFastHLine(uint8_t x,uint8_t y,uint8_t w,uint8_t color) {
	drawLine(x,y,x+w-1,y,color);
}

void screenkeys::drawRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h-1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x+w-1, y, h, color);
}

void screenkeys::fillRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t color) {
	uint8_t i;
	for (i=x; i<x+w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

void screenkeys::drawCircleHelper(uint8_t x0,uint8_t y0,uint8_t r,uint8_t cornername,uint8_t color) {
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

void screenkeys::drawCircle(uint8_t x0, uint8_t y0, uint16_t r,uint8_t color) {
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

void screenkeys::fillCircleHelper(uint8_t x0, uint8_t y0, uint16_t r, uint8_t cornername, int16_t delta, uint8_t color) {
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

void screenkeys::fillCircle(uint8_t x0, uint8_t y0, uint16_t r,uint8_t color) {
	drawFastVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}


void screenkeys::drawRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, uint8_t color) {
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


void screenkeys::fillRoundRect(uint8_t x, uint8_t y, uint8_t w,uint8_t h, uint16_t r, uint8_t color) {
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


void screenkeys::drawChar(uint8_t x, uint8_t y, unsigned char c, uint8_t color, uint8_t size) {
	// Clip right // Clip bottom  // Clip left  // Clip top
  if ((x >= _width) || (y >= _height) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0)) return;
  _bufferAddressing = TL_ORIGIN;//adafruit glcd fonts use this method or will be reversed
  for (uint8_t i = 0;i < 6;i++) {
		uint8_t line;
		if (i == 5) {
			line = 0x0;
		} else {
			line = pgm_read_byte(font+(c*5)+i);
		}
		for (uint8_t j = 0; j<8; j++) {
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
	if (c == '\n') {
		_cursor_y += _textsize * 8;
		_cursor_x  = 0;
	} else if (c == '\r') {
    // skip em
	} else {
		drawChar(_cursor_x, _cursor_y, c, _txtColour, _textsize);
		_cursor_x += _textsize * 6;
		if (_wrap && (_cursor_x > (_width - (_textsize * 6)))) {
			_cursor_y += _textsize * 8;
			_cursor_x = 0;
		}
	}
	return 1;
}
