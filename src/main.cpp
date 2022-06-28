//500ppr on driver
//stepper has a 5:1 gear reduction
//2500 pulses per revolution
//lead screw pitch 2 mm
//1250 pules /mm
//Movement requirements
// 4 inch stroke
// 1 inch accel
// 2 inch full speed
// 1 inch deccel
// homing at full speed
//alternating movement
//move const speed(distance,speed,motor)
//move velocity change (distance,start speed,end speed,motor)

#include <Arduino.h>

//Motor 1 connections
#define MOTOR_1_CW 4  //white
#define MOTOR_1_CCW 5 //orange
#define MOTOR_1_C_ON 6 //red

//Motor 2 connections
#define MOTOR_2_CW 7  //white
#define MOTOR_2_CCW 8 //orange
#define MOTOR_2_C_ON 9 //red

//End stops
#define END_SWITCH_1_INPUT 2 //End Switch #1 (interrupts?)
#define END_SWITCH_2_INPUT 3 //End Switch #2 (interrupts?)

volatile bool m1Homed = false;
volatile bool m2Homed = false;

int pulseWidth = 1; //width of a single pulse
int homingSpeedPulse = 1; //microseconds pulse delay for motor control when homing

//interrupt for motor1 endstop
void endSwitch1trig(){
 m1Homed = true;
}

//interrupt for motor2 endstop
void endSwitch2trig(){
  m2Homed = true;
}

void homeBoth(){
  //home motor 1
  while (m1Homed!=true){
    //Microsecond pulse loop
    digitalWrite(MOTOR_1_CCW, HIGH);//check if correct direction
    delayMicroseconds(homingSpeedPulse);
    digitalWrite(MOTOR_1_CCW, LOW);//check if correct direction
    delayMicroseconds(homingSpeedPulse);
  }
  
  //home motor 2
  while (m2Homed!=true){
    //Microsecond pulse loop
    digitalWrite(MOTOR_2_CCW, HIGH);//check if correct direction
    delayMicroseconds(homingSpeedPulse);
    digitalWrite(MOTOR_2_CCW, LOW);//check if correct direction
    delayMicroseconds(homingSpeedPulse);
    
  }
}

void m1Move(int dir, int steps){
  int curStep = 0;
  while (curStep<steps){
    //Microsecond pulse loop
    digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
    delayMicroseconds(homingSpeedPulse);
    digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
    delayMicroseconds(homingSpeedPulse);
    curStep++;
  }
}

void setup() {
  //testonly
  Serial.begin(9600); // open the serial port at 9600 bps:
  pinMode(MOTOR_1_CW, OUTPUT);
  pinMode(MOTOR_1_CCW, OUTPUT);
  pinMode(MOTOR_1_C_ON, OUTPUT);
  pinMode(MOTOR_2_CW, OUTPUT);
  pinMode(MOTOR_2_CCW, OUTPUT);
  pinMode(MOTOR_2_C_ON, OUTPUT);
  pinMode (END_SWITCH_1_INPUT,INPUT_PULLUP);
  pinMode (END_SWITCH_2_INPUT,INPUT_PULLUP);
  digitalWrite(MOTOR_1_C_ON, HIGH);
  digitalWrite(MOTOR_2_C_ON, HIGH);
  //Interrupts
  attachInterrupt(digitalPinToInterrupt(END_SWITCH_1_INPUT),endSwitch1trig,RISING); //check if triggered on boot up
  attachInterrupt(digitalPinToInterrupt(END_SWITCH_2_INPUT),endSwitch2trig,RISING); //check if triggered on boot up
}

int doneHoming= 0;

void loop() {
  //home
  if (m1Homed==false || m2Homed ==false){
    homeBoth();
  // } else if (doneHoming == 0) {
  //   doneHoming = 1;    
  }
  // digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
  // delayMicroseconds(pulseWidth); 
  // digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
  // delayMicroseconds(10);   
}