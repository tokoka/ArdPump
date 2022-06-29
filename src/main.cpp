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

AccelStepper stepper1 (2,MOTOR_1_CW,MOTOR_2_CCW);

volatile bool m1Homed = false;
volatile bool m2Homed = false;

int pulseWidth = 10;  //width of a single pulse in  microseconds
int defaultPulseGap = 100; //Default delay between pulses in microseconds
int homingSpeedPulseGap = 100; //microseconds pulse delay for motor control when homing

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
    delayMicroseconds(homingSpeedPulseGap);
    digitalWrite(MOTOR_1_CCW, LOW);//check if correct direction
    delayMicroseconds(homingSpeedPulseGap);
  }
  
  //home motor 2
  while (m2Homed!=true){
    //Microsecond pulse loop
    digitalWrite(MOTOR_2_CCW, HIGH);//check if correct direction
    delayMicroseconds(homingSpeedPulseGap);
    digitalWrite(MOTOR_2_CCW, LOW);//check if correct direction
    delayMicroseconds(homingSpeedPulseGap);
    
  }
}

void m1MoveConstantV(float distance, int pulseGap){
  long curStep = 0;
  long distanceInSteps = long((abs(distance))/STEP_DISTANCE); // get absolute value desired distance in mm and divide by step distance to get desired step distance and covert to int 
  
  if(distance>0){ //positive values moves towards origin endstop(CCW)
    while (curStep<distanceInSteps){
      //Microsecond pulse loop
      digitalWrite(MOTOR_1_CCW, HIGH);//check if correct direction
      delayMicroseconds(pulseWidth);
      digitalWrite(MOTOR_1_CCW, LOW);//check if correct direction
      delayMicroseconds(pulseGap);
      curStep++;
    }
  } else if (distance <0){ //negative values moves away from origin endstop(CW)
    while (curStep<distanceInSteps){
      //Microsecond pulse loop
      digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
      delayMicroseconds(pulseWidth);
      digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
      delayMicroseconds(pulseGap);
      curStep++;
    }
  }
}

//move const speed(distance,speed,motor)
//move velocity change (distance,start speed,end speed,motor)

void algoMoveAccel1(int dir, int pulseGap){
  if (dir == 0 ){
    digitalWrite(MOTOR_1_CCW, HIGH);//check if correct direction
    delayMicroseconds(pulseGap);
    digitalWrite(MOTOR_1_CCW, LOW);//check if correct direction
    delayMicroseconds(pulseGap);
  } else if (dir ==1){
    digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
    delayMicroseconds(pulseGap);
    digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
    delayMicroseconds(pulseGap);
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
  //Check endstops status
  m1Homed = digitalRead(END_SWITCH_1_INPUT);   // read the input pin
  m2Homed = digitalRead(END_SWITCH_2_INPUT);
  //Stepper config
  stepper1.setMaxSpeed(1000);
  stepper1.setSpeed(50);
 

}

//int doneHoming= 0;

void loop() {
  //home
  if (m1Homed==false || m2Homed ==false){
    homeBoth();
  // } else if (doneHoming == 0) {
  //   doneHoming = 1;    
  }
  //m1MoveConstantV(-25,200);
 
  //1 inch = 25.4 mm
  //25.4 devided by 0.0008  (step distance )= 31,750
  //1 inch is 31.750 steps
  //accelerate for 1 inch
  //0 dir towards endstop
  //1 dir away from endstop
  // params: dir, Pulse gap
  int pulseGap = 5000; //microseconds
  long curStep = 0; 
  // 1 inch accel 
  while (curStep<31750){
    digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
    delayMicroseconds(pulseGap);
    if(pulseGap>100){
      pulseGap=pulseGap-1;
    }
    digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
    delayMicroseconds(pulseGap);
    if(pulseGap>100){
      pulseGap=pulseGap-1;
    }
    curStep++;
  }
  curStep = 0; 
  pulseGap = 100;
  while (curStep<31750){
  digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
  delayMicroseconds(pulseGap);
  if(pulseGap<=5000){
    pulseGap=pulseGap+1;
  }
  digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
  delayMicroseconds(pulseGap);
  if(pulseGap<=5000){
    pulseGap=pulseGap+1;
  }
    curStep++;
  }
  // while (curStep<31750){
  //     algoMoveAccel1(1,pulseGap);
  //     if(pulseGap>4000){
  //       pulseGap=pulseGap-50;
  //     } else if (pulseGap>3000){
  //       pulseGap = pulseGap -40;
  //     } else if (pulseGap>2500){
  //       pulseGap = pulseGap -20;
  //     } else if (pulseGap>2000){
  //       pulseGap = pulseGap -12;
  //     } else if (pulseGap>1500){
  //       pulseGap = pulseGap -10;
  //     } else if (pulseGap>1250){
  //       pulseGap = pulseGap -8;
  //     } else if (pulseGap>1000){
  //       pulseGap = pulseGap -4;
  //     } else if (pulseGap>750){
  //       pulseGap = pulseGap -3;
  //     } else if (pulseGap>500){
  //       pulseGap = pulseGap -2;
  //     } else if (pulseGap>100){
  //       pulseGap = pulseGap -1;
  //     }
  //     curStep++;
  //   }
  //stepper1.runSpeed();

  m1Homed = false;
  // m2Homed = false;
  // digitalWrite(MOTOR_2_CW, HIGH);//check if correct direction
  // delayMicroseconds(10); 
  // digitalWrite(MOTOR_2_CW, LOW);//check if correct direction
  // delayMicroseconds(100);   
}