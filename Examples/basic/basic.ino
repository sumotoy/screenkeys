#include <SPI.h>
#include <mcp23s17.h>   // import library
#include <LC16.h>   // import library


LC16 lcdKeys(16,10,0x20,3,6);

void setup(){
  lcdKeys.begin();
}

void loop() {
}