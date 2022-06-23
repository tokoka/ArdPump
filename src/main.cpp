#include <Arduino.h>

#define CW 4  //white
#define CCW 5 //orange
#define C_ON 6 //red

void setup() {
  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  pinMode(C_ON, OUTPUT);
  digitalWrite(C_ON, HIGH);
}

void loop() {
// port manipulation pulse loop
//PIND = 0x10 ;//pin 4
PIND = 0x20 ;//pin 5
//8 nop(wait) is approximately 500ns
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t"); 
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
//PIND = 0x10 ;//pin 4
PIND = 0x20 ;//pin 5
//8 nop(wait) is approximately 500ns
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");
__asm__("nop\n\t");

  //Microsecond pulse loop
  // digitalWrite(CCW, HIGH);
  // delayMicroseconds(1);
  // digitalWrite(CCW, LOW);
  // delayMicroseconds(1);
}