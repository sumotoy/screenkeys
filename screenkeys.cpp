#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <Arduino.h>
#include "screenkeys.h"

//#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

screenkeys::screenkeys() 
{
	_width = 0;//mod by rotation
	_height = 0;//mod by rotation
	_cursor_x = 0;
	_cursor_y = 0;
	_rotation = 0;
	_textsize = 1;
	_foreground = BLACK;
	_background = WHITE;
	_wrap = true;
	clearBuffer();
	//_bufferAddressing = DEFAULT_BUFFER_ADDRESSING;
	#if defined(_DBG)
	_dbg = 1;
	#endif
}

void screenkeys::clearBuffer() {
	for (uint8_t i=0;i<64;i++){
		_buffer[i] = 0x00;
	}	
}

void screenkeys::invert() {
	for (uint8_t i=0;i<64;i++){
		_buffer[i] ^= 0xFF;
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
	_foreground = c;
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
	//if (_bufferAddressing == TL_ORIGIN){
	#if defined(TL_ORIGIN)
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
	#elif defined(BR_ORIGIN)
	//} else if (_bufferAddressing == BR_ORIGIN){
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
	#elif defined(BL_ORIGIN)	
	//} else if (_bufferAddressing == BL_ORIGIN){
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
	#else
	//} else {//TR_ORIGIN
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
	#endif	
	//}
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


size_t screenkeys::write(uint8_t c){
	if (_cursor_x > _width || _cursor_y > _height) return 1;
	struct FontHeader header;
	memcpy_P(&header, _font, sizeof(FontHeader));
	//se il carattere è troppo alto esci.
	if (_cursor_y+header.height < 0) return 1;
	if (_cursor_x > 0) drawLine(_cursor_x - 1,_cursor_y,_cursor_x - 1,(_cursor_y + header.height) - 1, _background);
		if(c == '\n') { // Newline
			_cursor_y = _cursor_y - header.height - 1;
		} else if (c == '\r') {	
		} else {
			//scrive il carattere e riceve il suo ingombro!
			int charWide = drawChar(_cursor_x, _cursor_y, c, _foreground, _background);
			if (charWide > 0) {//carattere scritto,cancelliamo un pezzo di quello che segue per evitare sporcature
				#if defined(_PRECLN)
				drawFastVLine(_cursor_x + charWide, _cursor_y, _cursor_y + header.height-1, _background);
				#endif
			}
		}
	return 1;
}

uint8_t screenkeys::drawChar(uint8_t x,uint8_t y, unsigned char c,bool colour,bool _background){
	if (x >= _width || y >= _height) {
		return 0;
	}
	struct FontHeader header;
	memcpy_P(&header, (void*)_font, sizeof(FontHeader));
	if (c == ' ') {//scrive uno spazio
		uint8_t charWide = charWidth(' ');
		fillRect(x,y-1,x + charWide,y + header.height,_background);
		return charWide;
	}
	//se il carattere è fuori range non continuare!
	if (c < header.firstChar || c >= (header.firstChar + header.charCount)) {
		return 0;
	}
	uint8_t width = 0;
	uint16_t index = 0;
	//calcola di quanti bytes è una colonna
	uint8_t bytes = (header.height + 7) / 8;
	//calcola il carattere (non c'è tutta la scala)
	c -= header.firstChar;
	// ora si valuta se è un Fixed Width o Variabile
	if (header.size == 0) {//fixed width
		width = header.fixedWidth;//il width è fisso
		//questo calcola quanto è grande l'indice intero di tutti i bits del font
		index = sizeof(FontHeader) + (c*(bytes * width));
	} else {
		// variable width font, read width data, to get the index
		for (uint8_t i = 0; i < c; i++) {
			index += pgm_read_byte(this->_font + sizeof(FontHeader) + i);
		}
		index = index * bytes + sizeof(FontHeader) + header.charCount;
		width = pgm_read_byte(this->_font + sizeof(FontHeader) + c);
	}
	//ora si valuta se il carattere è dentro lo schermo, altrimenti si esce
	if (x < -width || y < -header.height) return 0;
	//per evitare sporcature, scrivo una riga con lo stesso colore del background
	#if defined(_PRECLN)
	drawFastVLine(x>0?x:0,y>0?y:0,(y+header.height)>(_height-1)?(_height-1):(y+header.height)-1,_background);
	#endif
	for (int16_t ox = 0; ox < width; ox++) {
		if (ox+x >= _width) break;
		int16_t oy = 0;
		for (int8_t byte_y = bytes-1; byte_y >= 0; byte_y--) {
			uint8_t data = pgm_read_byte(this->_font + index + ox + (byte_y * width));
			int8_t start_bit;
			int8_t end_bit;
			if (bytes == 1) {
				start_bit = header.height-1;
				end_bit = 0;
			} else {
				start_bit = 7;
				end_bit = (byte_y < (bytes-1)) ? 0: 7-((header.height-1)%8);
			}
			for(int8_t bit_y = start_bit; bit_y >= end_bit; bit_y--) {
				if ((oy + y) < _height && (ox + x) >= 0 && (oy + y) >= 0) {
					 //drawPixel(x+ox,y+(((header.height-1)-bit_y)-(byte_y*(1+start_bit-end_bit))), (data & 1<<bit_y) ? colour : _background);
					 drawPixel(x+ox,y+(((header.height-1)-bit_y)-(byte_y*(1+start_bit-end_bit))),(data & (1 << bit_y)) ? colour : _background);
				}
				oy++;
				if (oy == header.height) break;
			}
		}
		_cursor_x++;//avanzamento cursore
	}
	_cursor_x++;//avanzamento cursore
	return width;
}


uint8_t screenkeys::charWidth(unsigned char c) {
	if (c == ' ') c = 'n';
	uint8_t width = 0;
	struct FontHeader header;
	memcpy_P(&header,(void*)_font, sizeof(FontHeader));
	if (c < header.firstChar || c >= (header.firstChar + header.charCount))  return 0;
	c -= header.firstChar;
	if (header.size == 0) {
		return header.fixedWidth;
	} else {
		width = pgm_read_byte(_font + sizeof(FontHeader) + c);
	}
	return width;
}

/*
static void screenkeys::intFunction() {
}
*/
