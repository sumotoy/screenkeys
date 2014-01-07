#include <SPI.h>        // needed!
#include <mcp23s17.h>   // needed!
#include <LC16.h>       // needed!


// definitions -------------------
#define   SWITCHES            16
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
  
  //font8x8
//font6x8
//font4x6
/*
No of switches / CS pin / start address / prg clock pin / en clock pin
Master clock pin it's defined in the library
*/
LC16 lcdKeys(SWITCHES,CS_COMMON_PIN,START_ADRS,PRG_CLOCK_PIN,ENABLE_CLOCK);

void setup(){
  //Serial.begin(38400);
  lcdKeys.begin();//initialize all
  lcdKeys.setColor(0,BR_GREEN);//switch no,color
//  lcdKeys.printImage(0,exit_img);//switch no,image
  //lcdKeys.setFont(font8x8);
//  delay(1000);
//  lcdKeys.clear(0);
//  lcdKeys.refresh(0);
  //lcdKeys.fillRect(0,0,31,15,0);
  //lcdKeys.drawPixel(31,15);
  lcdKeys.drawRect(0,0,31,15);
lcdKeys.setCursor(5,5);
lcdKeys.print("Ab12");
//  lcdKeys.drawRect(0,0,15,15);
//  lcdKeys.drawRect(16,0,6,15);
//  lcdKeys.fillRect(16,0,6,8);
  lcdKeys.refresh(0);
}

void loop() {
  if (lcdKeys.getError() > 0){
    //Serial.println("error");
  }
}