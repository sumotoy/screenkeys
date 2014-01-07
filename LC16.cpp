#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "LC16.h"


LC16::LC16(const uint8_t switches,const uint8_t csPin,const uint8_t startAdrs,const uint8_t prgClock,const uint8_t clkEnable){
	_XRES = 32;//fixed res
	_YRES = 16;//fixed res
	_cs = csPin;
	if (startAdrs >= 0x20 && startAdrs <= 0x26){//HAEN works between 0x20...0x27, reserve 1 for keyscan
		_adrs = startAdrs;
		_error = 0;
	} else {
		_error = 1;
	}
	if (switches <= 16){
		_chips = 1;
	} else if (switches > 16 && switches <= 32){
		if (_adrs >= 0x25) _error = 2;//you need to reserve at list 4 consecutive addresses in the range 0x20..0x27
		_chips = 2;
	} else if (switches > 32 && switches <= 48){
		if (_adrs >= 0x23) _error = 2;//you need to reserve at list 6 consecutive addresses in the range 0x20..0x27
		_chips = 3;
	} else if (switches > 48 && switches <= 64){
		_chips = 4;
		_adrs = 0x20;//should be the start address for 8 chip! (4 for LCD and 4 for keyscan)
	} else {
		_chips = 0;
		_error = 3;//too many chip! out of range
	}
	_mainClock = MAIN_CLK_PIN;
	_programClock = prgClock;
	_clockEnable = clkEnable;
}

void LC16::begin(bool protocolInitOverride) {
	//screenkeys::begin(protocolInitOverride);
	if (_error == 0){
		pinMode(_mainClock,OUTPUT);
		pinMode(_programClock,OUTPUT);
		pinMode(_clockEnable,OUTPUT);
		digitalWrite(_programClock,HIGH);
		digitalWrite(_clockEnable,HIGH);
		mcp.postSetup(_cs,_adrs);
		mcp.begin(protocolInitOverride);
		delay(100);
		mcp.gpioPinMode(OUTPUT);
		mcp.gpioPort(0xFFFF);
		analogWriteFrequency(_mainClock, CLK_FRQ);
		analogWrite(_mainClock,120);
		init_lcdChip(0);
	}
}

uint8_t LC16::getError(){//for debug
	return _error;
}

void LC16::PClock(uint8_t cTicks){
  uint8_t i;
  for(i = 0; i < cTicks; i++) {
    digitalWriteFast(_programClock,LOW);//CLK_toggle
    //delayMicroseconds(10);
    digitalWriteFast(_programClock,HIGH);//CLK_toggle
    //delayMicroseconds(10);
  }
} 

void LC16::progEnable(boolean mode){
  if (mode){
    digitalWriteFast(_clockEnable,LOW);
    digitalWriteFast(_programClock,HIGH);
  } 
  else {
    digitalWriteFast(_clockEnable,HIGH);
    analogWrite(_mainClock,120);
  }
}

void LC16::sendData(uint8_t key,unsigned char cDataByte, unsigned char cParity){
	progEnable(true);//disconnect main clock
	unsigned char i, cBit;
	uint16_t temp = 0xFFFF;
	//Calculate parity for this databyte type cParity = 0 => even parity (start/end byte) = 1 => odd parity (command/data byte)
	for (i = 0; i < 8; i++){
		cParity = (cDataByte >> i) ^ cParity;
	}
	cParity = cParity & 0x01;
	//First send 1 start bit
	bitClear(temp,key);
	mcp.gpioPort(temp);//send to gpio
	//Set data up
	PClock(1) ;
	//Toggle clock line
	//Now send 8 data bits - lsb first
	for(i = 0; i < 8; i++) {
		//Get lsb of data
		cBit = (cDataByte & 0x01);
		//Send bit to key
		if (cBit == 1){
			bitSet(temp,key);
		} else {
			bitClear(temp,key);
		}
		mcp.gpioPort(temp);//send to gpio
		PClock(1) ; 
		//Shift data right to ready next bit
		cDataByte = cDataByte >> 1 ;
	}
	//Send computed parity bit
	if (cParity){
		bitSet(temp,key);
	} else {
		bitClear(temp,key);
	}
	mcp.gpioPort(temp);//send to gpio
	PClock(1) ;
	bitSet(temp,key);
	mcp.gpioPort(temp);//send to gpio
	PClock(1) ;
	mcp.gpioPort(temp);//send to gpio
	PClock(1) ;
	//digitalWriteFast(PRG_CLOCK_PIN,HIGH);
	progEnable(false);//reconnect main clock
} 

void LC16::start(uint8_t key){
	sendData(key,LC_STARTRG,0);
}

void LC16::stop(uint8_t key){
	sendData(key,LC_STOPTRG,0);
}

void LC16::sendByte(uint8_t key,byte reg,byte val){
	start(key);
	sendData(key,reg,1);
	sendData(key,val,1);
	stop(key);
}


void LC16::sendWord(uint8_t key,byte reg,byte val1,byte val2){
	start(key);
	sendData(key,reg,1);
	sendData(key,val1,1);
	sendData(key,val2,1);
	stop(key);
}

void LC16::init_lcdChip(uint8_t key){
  _width = XRES;
  _height = YRES;
  _cursor_x = 0;
  _cursor_y = 0;
  _textsize = 1;
  _rotation = 0;
  _wrap = true;
  
  sendByte(key,LC_FREQREG,0x60);//for 400K
  sendWord(key,LC_MUXREG,0x02,0x05);
  clear(key);
  refresh(key);
  setColor(key,BL_NONE);
  delay(100);
}

void LC16::refresh(uint8_t key){
	printImage(key,_buffer);
}

void LC16::printImage(uint8_t key, unsigned char * data){
  uint8_t i;
  start(key);
  sendData(key,LC_PIXREG,1);
  for (i=0; i<64; i++){
    sendData(key,data[i],1);
  }
  stop(key);
}

void LC16::setColor(uint8_t key,byte color){
	sendByte(key,LC_COLREG,color);
}


void LC16::fill(uint8_t key,uint8_t color){
	unsigned char col = 0xFF;
	if (color != 0) col = 0x00;
	for (uint8_t i=0; i<64; i++){
		_buffer[i] = col;
	}
}

void LC16::clear(uint8_t key){
	fill(key,1);
}
