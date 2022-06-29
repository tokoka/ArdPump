//500ppr on driver
//stepper has a 5:1 gear reduction
//2500 pulses per revolution
//lead screw pitch 2 mm
//1250 pules /mm
//step distance = 0.0008 mm (distance traveled after 1 pulse)
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
#include <AccelStepper.h>

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

#define STEPS_PER_MM 1250
#define STEP_DISTANCE 0.0008

AccelStepper stepper1(2,MOTOR_1_CW, MOTOR_1_CCW);


void setup() {
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

  
    stepper1.setMaxSpeed(30000.0);
    stepper1.setAcceleration(10000.0);
    stepper1.setMinPulseWidth(10);
    //50k = 20mm
    //2500 distance = 1mm
    stepper1.setSpeed (100000.0);
    stepper1.moveTo(50000);
}

void loop() {
  //1 inch = 25.4 mm
  //25.4 devided by 0.0008  (step distance )= 31,750
  //1 inch is 31.750 steps
  //accelerate for 1 inch
  //0 dir towards endstop
  //1 dir away from endstop
  // params: dir, Pulse gap

  if (stepper1.distanceToGo() == 0){
    stepper1.moveTo(-stepper1.currentPosition());
  }
    stepper1.run();


  // int pulseGap = 3000; //microseconds
  // long curStep = 0; 
  // // 1 inch accel 
  // while (curStep<31750){
  //   digitalWrite(MOTOR_1_CCW, HIGH);//check if correct direction
  //   delayMicroseconds(pulseGap);
  //   if(pulseGap>500){
  //     pulseGap=pulseGap-1;
  //   }
  //   digitalWrite(MOTOR_1_CCW, LOW);//check if correct direction
  //   delayMicroseconds(pulseGap);
  //   if(pulseGap>500){
  //     pulseGap=pulseGap-1;
  //   }
  //   curStep++;
  // }
}