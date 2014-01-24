#include <SPI.h>        // needed!
#include <mcp23s17.h>   // needed!
#include <LC16.h>       // needed!
#include <fonts/SystemFont5x7.h>//ok
//#include <fonts/squarer10x11.h>//ok
//#include <fonts/beatbox10x11.h>//ok
//#include <fonts/beatbox10x11.h>//ok
//#include <fonts/binBold10x8.h>//ok
//#include <fonts/binReg10x9.h>//ok
//#include <fonts/binSmall10x8.h>//ok
//#include <fonts/block10x10.h>//ok
//#include <fonts/block10x9.h>//ok
//#include <fonts/block10x7.h>//ok
#include <fonts/block10x7.h>//ok

volatile uint8_t key = 0;
/*
#include "fonts/_default.h"//this is necessary, always!
 #include "fonts/Arial__9.h"
 */
// definitions -------------------
#define   SWITCHES            8
#define   CS_COMMON_PIN       10
#define   START_ADRS          0x20
#define   PRG_CLOCK_PIN       3
#define   ENABLE_CLOCK        6
// test image --------------------
unsigned char exit_img[64] = {
  0x04, 0x00, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00,
  0x1f, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00,
  0xc4, 0x31, 0x40, 0x0c,
  0x04, 0x4a, 0x60, 0x12,
  0x04, 0x42, 0x40, 0x02,
  0x04, 0x21, 0x49, 0x0e,
  0x04, 0x12, 0x46, 0x12,
  0x04, 0x0a, 0x46, 0x12,
  0xc4, 0x79, 0xe9, 0x0c,
  0x04, 0x00, 0x00, 0x40,
  0x04, 0x00, 0x00, 0x60,
  0xff, 0xff, 0xff, 0xff,
  0x04, 0x00, 0x00, 0x60,
  0x04, 0x00, 0x00, 0x40 }; 

/*
No of switches / CS pin / start address / prg clock pin / en clock pin
 Master clock pin it's defined in the library
 */
LC16 lcdKeys(SWITCHES,CS_COMMON_PIN,START_ADRS,PRG_CLOCK_PIN,ENABLE_CLOCK);

void setup(){

  //Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }
  //delay(2000);
  //Serial.println("started");
//  if (lcdKeys.getError() > 0){
//    Serial.println("error");
//    Serial.println(lcdKeys.getError(),DEC);
//  }

  lcdKeys.begin();//initialize all

  lcdKeys.setColor(0,BR_GREEN);//switch no,color
  //  lcdKeys.printImage(0,exit_img);//switch no,image
  //lcdKeys.setFont(&_default);
  //lcdKeys.setFont(&Arial__9);
  lcdKeys.setFont(SystemFont5x7);
  //lcdKeys.setFont(squarer10x11);
  //lcdKeys.setFont(block10x7);
  //lcdKeys.setFont(minifont);
  //  delay(1000);
  //  lcdKeys.clear(0);
  //  lcdKeys.refresh(0);
  //lcdKeys.fillRect(0,0,31,15,0);
  //lcdKeys.drawPixel(0,0);
  //lcdKeys.drawRect(0,0,31,15);
  lcdKeys.setCursor(1,1);
  lcdKeys.print("ABC");
  //  lcdKeys.drawRect(0,0,15,15);
  //  lcdKeys.drawRect(16,0,6,15);
  //  lcdKeys.fillRect(16,0,6,8);
  //lcdKeys.invert();
  lcdKeys.refresh(0);
//  lcdKeys.clear();
//  lcdKeys.setCursor(1,1);
//  lcdKeys.print("EFGH");
//  lcdKeys.refresh(1);
  //Serial.println("inited");
 
}

void loop() {
  if (lcdKeys.getError() > 0){
//    Serial.println("error");
//    Serial.println(lcdKeys.getError(),DEC);
  }
  key = lcdKeys.keypressScan();
  if (key < 255){
    lcdKeys.clear();
    lcdKeys.setCursor(1,1);
    lcdKeys.print("S:");
    lcdKeys.print(key,DEC);
    lcdKeys.refresh(0);
    //Serial.println(key,DEC);
  }
}




