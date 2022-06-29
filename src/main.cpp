#include <Arduino.h>
#define MOTOR_1_CW 17  //white
#define MOTOR_1_CCW 18 //orange
#define MOTOR_1_C_ON 4 //red
int toggleGap = 300; // starting gap in microseconds
int counter = 0;
void setup() {
  pinMode(MOTOR_1_CW, OUTPUT);
  pinMode(MOTOR_1_CCW, OUTPUT);
  pinMode(MOTOR_1_C_ON, OUTPUT);
  digitalWrite(MOTOR_1_C_ON, HIGH);
}
void loop() {
  
  digitalWrite(MOTOR_1_CW, HIGH);
  delayMicroseconds(toggleGap);
  digitalWrite(MOTOR_1_CW, LOW);
  delayMicroseconds(toggleGap);
  if(toggleGap>50){
    if (counter == 50){
      toggleGap--;   
      counter = 0;
    }
    counter++;   
  }
}