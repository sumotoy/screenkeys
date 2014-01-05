#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "LC16.h"

#include <../SPI/SPI.h>//this chip needs SPI
#include <../gpio_expander/mcp23s17.h>


LC16::LC16(const uint8_t switches,const uint8_t csPin,const uint8_t startAdrs,const uint8_t prgClock,const uint8_t clkEnable){
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
	_key_buff[64] = {0};
}

void LC16::begin(bool protocolInitOverride) {
	if (_error == 0){
		if (!protocolInitOverride){
			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
		pinMode(_cs, OUTPUT);
		digitalWrite(_cs, HIGH);
		// init gpio here
		delay(100);
		pinMode(_mainClock,OUTPUT);
		pinMode(_programClock,OUTPUT);
		pinMode(_clockEnable,OUTPUT);
		digitalWrite(_programClock,HIGH);
		digitalWrite(_clockEnable,HIGH);
		analogWriteFrequency(_mainClock, CLK_FRQ);
		analogWrite(_mainClock,120);
	}
}
