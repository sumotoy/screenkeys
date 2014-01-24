#include <stdio.h>
#include <inttypes.h>
#include <Arduino.h>
#include "LC16.h"

#ifndef EXTSWITCH
volatile bool LC16::_keyPressed = false;
#endif

// Instance
//mainly this set some var and check for possible range error
LC16::LC16(const uint8_t switches,const uint8_t csPin,const uint8_t startAdrs,const uint8_t prgClock,const uint8_t clkEnable){
	_width = _XRES = LC16_XRES;
	_height = _YRES = LC16_YRES;
	_cursor_x = 0;
	_cursor_y = 0;
	_textsize = 1;
	_rotation = 0;
	_switches = switches;
	_wrap = true;
	_cs = csPin;
	if (startAdrs >= 0x20 && startAdrs <= 0x26){//HAEN works between 0x20...0x27, reserve 1 for keyscan
		_adrs = startAdrs;
		_error = 0;
	} else {
		_error = 1;
	}
	if (switches > SWITCHLIMIT) {
		_error = 3;//too many chip! out of range
	} else if (switches <= 16){//1 chip
	} else if (switches > 16 && switches <= 32){//2 chips
		if (_adrs >= 0x25) _error = 2;//you need to reserve at list 4 consecutive addresses in the range 0x20..0x27
	} else if (switches > 32 && switches <= 48){//3,4 chips
		if (_adrs >= 0x23) _error = 2;//you need to reserve at list 6 consecutive addresses in the range 0x20..0x27
	} else if (switches > 48 && switches <= 64){//4 chips
		_adrs = 0x20;//should be the start address for 8 chip! (4 for LCD and 4 for keyscan)
	}
	#ifndef __USEEXTCLK
		_mainClock = MAIN_CLK_PIN;
	#endif
	_programClock = prgClock;
	_clockEnable = clkEnable;
}

//main initialization routine
//this initialize all GPIO's chip and prepare hardware for being used
void LC16::begin(bool protocolInitOverride) {
	if (_error == 0){
		uint8_t i;
		pinMode(_programClock,OUTPUT);//set port direction
		pinMode(_clockEnable,OUTPUT);//set port direction
		digitalWrite(_programClock,HIGH);//pullup
		digitalWrite(_clockEnable,HIGH);//pullup
		//Are we using the internal clock generator or an external one?
		#ifndef __USEEXTCLK
			pinMode(_mainClock,OUTPUT);//set direction
			analogWriteFrequency(_mainClock, CLK_FRQ);
			analogWrite(_mainClock,120);//in my system this give me a resonable balanced squarewave(400Khz)
		#endif
		//GPIO's setup, check how many GPIO's and how configured
		#if SWGPIOS < 2 //in this case only, half GPIO can be used for switches
			_gpios_out[0].postSetup(_cs,_adrs);
			_gpios_out[0].begin(protocolInitOverride);
			#ifndef EXTSWITCH//Using internal switch scan so BankA as Out and BankB as IN
				_gpios_out[0].gpioRegisterWriteByte(_gpios_out[0].IOCON,0b00101000);//set conf
				_gpios_out[0].gpioPinMode(0b1111111100000000);//set in/out direction A=OUT,B=IN
				_gpios_out[0].gpioRegisterWriteWord(_gpios_out[0].GPPU,0b1111111100000000);//set pullup on Bank B (use pullup command? will check later)
				_gpios_out[0].gpioRegisterWriteWord(_gpios_out[0].IPOL,0b1111111100000000);// invert polarity bank B
				_gpios_out[0].gpioRegisterWriteWord(_gpios_out[0].GPINTEN,0b1111111100000000);// enable interrupt in Bank B for all bank B ins
				_gpios_out[0].gpioRegisterRead(_gpios_out[0].INTCAP+1);// read from interrupt B, capture ports to clear them
				pinMode(INTpin, INPUT);//set direction for MCU INT pin
				digitalWrite(INTpin, HIGH);//PullUp it
				enableKeyInt(keypress);//enable Interrupt routine and set the fallback function
				_keyPressed = false;
			#else//Not using switch scan(16 switches in this mode since we using all chip for this)
				_gpios_out[0].gpioPinMode(OUTPUT);//A=OUT,B=OUT
				_gpios_out[0].gpioPort(0xFFFF);//pullup
			#endif
			delay(10);
		#else //more than 1 GPIO
			#ifndef EXTSWITCH//this mode divide GPIO's in 2 block of chips, first half for OUT and second half for IN
				for (i=0;i<(SWGPIOS/2);i++){//first half, GPIO's as OUT
					_gpios_out[i].postSetup(_cs,_adrs+(1*i));//20,21,22,23.
					_gpios_out[i].begin(protocolInitOverride);
					_gpios_out[i].gpioPinMode(OUTPUT);//A=OUT,B=OUT
					_gpios_out[i].gpioPort(0xFFFF);//pullup
				}
				for (i=0;i<(SWGPIOS/2);i++){//second half, GPIO's as IN
					_gpios_in[i].postSetup(_cs,_adrs+(SWGPIOS/2)+(1*i));//24,25,26,27.
					_gpios_in[i].begin(protocolInitOverride);
					_gpios_in[i].gpioPinMode(INPUT);//A=IN,B=IN
					// Now special initializations for set as scanners (begin here)
					// TODO
				}
				pinMode(INTpin, INPUT);//set direction of INT pin for MCU
				digitalWrite(INTpin, HIGH);//pullup
				enableKeyInt(keypress);//enable Interrupt routine and set the fallback function
				_keyPressed = false;
			#else//not using internal switch scan, all GPIO's used for SW DATA
				for (i=0;i<(SWGPIOS);i++){//for each GPIO
					_gpios_out[i].postSetup(_cs,_adrs+(1*i));//20,21,22,23,24,etc.
					_gpios_out[i].begin(protocolInitOverride);
					_gpios_out[i].gpioPinMode(OUTPUT);//set direction
					_gpios_out[i].gpioPort(0xFFFF);//pullup
				}
			#endif	
			delay(10);
		#endif// end 1/many GPIO
		//now the MCU has full control of GPIO's and Clock logic, we can initialize Screenkeys modules
		for (i=0;i<_switches;i++){
			init_lcdChip(i);//init each switch
		}
	}
}

//used for detect witch error has occoured during initialization
uint8_t LC16::getError(){//for debug
	return _error;
}

//this triggers a clock cicle (of n Ticks)
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
	digitalWriteFast(_clockEnable,!mode);
	/*
  if (mode){
    digitalWriteFast(_clockEnable,LOW);
  } 
  else {
    digitalWriteFast(_clockEnable,HIGH);
  }
  */
}

void LC16::_sendGpio(uint8_t chip,uint16_t data){
	#if SWGPIOS > 1
		_gpios_out[chip].gpioPort(data);//send to gpio
	#else
		_gpios_out[0].gpioPort(data);//send to gpio
	#endif
}


//used to calculate at witch GPIO chip instance belongs the key
uint8_t LC16::_witchGpio(uint8_t key){
	#if SWGPIOS < 2
		return 0;
	#else
	if (key >= SWITCHLIMIT) return 255;
	if (key < 16){						//1st chip
		return 0;
	} else if (key > 15 && key < 32){	//2nd chip
		return 1;
	} else if (key > 31 && key < 48){	//3nd chip
		return 2;
	} else {							//4nd chip
		return 3;
	}
	#endif
}


//this is the core routine that does all the magic.
//It does a lot of stuff and should be performed exact as listed.
void LC16::sendData(uint8_t key,unsigned char cDataByte, unsigned char cParity){
	uint8_t chip = _witchGpio(key);//calculate once witch GPIO chip it's needed
	if (chip == 255) return;//out of range, terminate now
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
	_sendGpio(chip,temp);//send
	//Set data up
	PClock(1);//Toggle clock line
	//Now send 8 data bits - lsb first
	for(i = 0; i < 8; i++) {
		//Get lsb of data
		cBit = (cDataByte & 0x01);
		bitWrite(temp,key,cBit);
		_sendGpio(chip,temp);
		PClock(1) ; 
		//Shift data right to ready next bit
		cDataByte = cDataByte >> 1 ;
	}
	//Send computed parity bit
	bitWrite(temp,key,cParity);
	_sendGpio(chip,temp);
	PClock(1) ;
	bitSet(temp,key);
	_sendGpio(chip,temp);
	PClock(1) ;
	_sendGpio(chip,temp);
	PClock(1) ;
	progEnable(false);//reconnect main clock
} 


void LC16::sendByte(uint8_t key,byte reg,byte val){
	sendData(key,LC_STARTRG,0);//start key
	sendData(key,reg,1);
	sendData(key,val,1);
	sendData(key,LC_STOPTRG,0);//stop key
}


void LC16::sendWord(uint8_t key,byte reg,byte val1,byte val2){
	sendData(key,LC_STARTRG,0);//start key
	sendData(key,reg,1);
	sendData(key,val1,1);
	sendData(key,val2,1);
	sendData(key,LC_STOPTRG,0);//stop key
}

//LC16 initialization routine
void LC16::init_lcdChip(uint8_t key){
  sendByte(key,LC_FREQREG,SCRKEY_CLK);//Frequency scan
  sendWord(key,LC_MUXREG,0x02,0x05);//set mux (LC16)
  clear();
  refresh(key);
  setColor(key,BL_NONE);
  delay(10);
}

//this send to the LC16 a raw content
void LC16::printImage(uint8_t key, unsigned char * data){
  uint8_t i;
  sendData(key,LC_STARTRG,0);//start key
  sendData(key,LC_PIXREG,1);
  for (i=0; i<LC16_BFSIZE; i++){
    sendData(key,data[i],1);
  }
  sendData(key,LC_STOPTRG,0);//stop key
}

//instruct to print the buffer content
void LC16::refresh(uint8_t key){
	printImage(key,_buffer);
}

//change background color
void LC16::setColor(uint8_t key,byte color){
	sendByte(key,LC_COLREG,color);
}

//fill the buffer with a specific color
void LC16::fill(uint8_t color){
	unsigned char col = 0xFF;	//Mmmm...
	if (color != 0) col = 0x00; //Mmmmmm... again...
	for (uint8_t i=0; i<LC16_BFSIZE; i++){
		_buffer[i] = col;
	}
}

//fill the buffer with white
void LC16::clear(){
	fill(WHITE);
}


#ifndef EXTSWITCH

void LC16::keypress(){
	_keyPressed = true;
}


uint8_t LC16::keypressScan(){
	if (_keyPressed){
		disableKeyInt();//prevent an interrupt, disable
		uint8_t val = 0;
		delay(30);  // de-bounce before we re-enable interrupts
		#if SWGPIOS < 2//special case, just one GPIO
		if (_gpios_out[0].gpioRegisterRead(_gpios_out[0].INTF+1)) val |= _gpios_out[0].gpioRegisterRead(_gpios_out[0].INTCAP+1);
		for (uint8_t sw = 0; sw < 8; sw++) {
			if (val & (1 << sw)){
				_keyPressed = false;
				enableKeyInt(keypress);//enable again interrupt
				return sw;
			}
		}
		#else//Multiple GPIO
		// TODO
		#endif
		enableKeyInt(keypress);//enable again interrupt
		_keyPressed = false;
		return 255;
	} else {
		return 255;
	}
}
#endif
