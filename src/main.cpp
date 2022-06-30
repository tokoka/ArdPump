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
//--------Using accelstepper -----------------
//50k = 20mm
//2500 distance = 1mm

#include <Arduino.h>
#include <AccelStepper.h>

//Motor 1 connections
#define MOTOR_1_CW 17  //white
#define MOTOR_1_CCW 18 //orange
#define MOTOR_1_C_ON 4 //red

//Motor 2 connections
#define MOTOR_2_CW 33  //white
#define MOTOR_2_CCW 32 //orange
#define MOTOR_2_C_ON 2 //red

//End stops
#define END_SWITCH_1_INPUT 12 //End Switch #1 (interrupts?)
#define END_SWITCH_2_INPUT 14 //End Switch #2 (interrupts?)

#define STEPS_PER_MM 1250
#define STEP_DISTANCE 0.0008

//Interrupts vars
volatile bool m1Homed = false;
volatile bool m2Homed = false;


AccelStepper stepper1(2,MOTOR_1_CW, MOTOR_1_CCW);
AccelStepper stepper2(2,MOTOR_2_CW, MOTOR_2_CCW);



int homingPulseWidth = 50;

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
    delayMicroseconds(homingPulseWidth);
    digitalWrite(MOTOR_1_CCW, LOW);//check if correct direction
    delayMicroseconds(homingPulseWidth);
  }
  //Move 5mm out
  int counter = 0;
  while (counter<6250){
    //Microsecond pulse loop
    digitalWrite(MOTOR_1_CW, HIGH);//check if correct direction
    delayMicroseconds(homingPulseWidth);
    digitalWrite(MOTOR_1_CW, LOW);//check if correct direction
    delayMicroseconds(homingPulseWidth);
    counter++;
  }
  //home motor 2
  while (m2Homed!=true){
    //Microsecond pulse loop
    digitalWrite(MOTOR_2_CCW, HIGH);//check if correct direction
    delayMicroseconds(homingPulseWidth);
    digitalWrite(MOTOR_2_CCW, LOW);//check if correct direction
    delayMicroseconds(homingPulseWidth);
  }
  counter = 0;
  while (counter<6250){
    //Microsecond pulse loop
    digitalWrite(MOTOR_2_CW, HIGH);//check if correct direction
    delayMicroseconds(homingPulseWidth);
    digitalWrite(MOTOR_2_CW, LOW);//check if correct direction
    delayMicroseconds(homingPulseWidth);
    counter++;
  }
}

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
  //Interrupts
  attachInterrupt(digitalPinToInterrupt(END_SWITCH_1_INPUT),endSwitch1trig,RISING); //check if triggered on boot up
  attachInterrupt(digitalPinToInterrupt(END_SWITCH_2_INPUT),endSwitch2trig,RISING); //check if triggered on boot up
  //Check endstops status
  m1Homed = digitalRead(END_SWITCH_1_INPUT);   // read the input pin
  m2Homed = digitalRead(END_SWITCH_2_INPUT);

  //Stepper 1 AccelStepper params 
  stepper1.setMaxSpeed(10000.0);
  stepper1.setAcceleration(10000.0);
  stepper1.setMinPulseWidth(10);
  stepper1.setSpeed (100000.0);
  //Stepper 2 AccelStepper params  
  stepper2.setMaxSpeed(10000.0);
  stepper2.setAcceleration(10000.0);
  stepper2.setMinPulseWidth(10);
  stepper2.setSpeed (100000.0);

  Serial.begin(115200);
}

//Algo stages
bool initialCompression = 1;
bool firstRunFirstCylinder = 0;
bool Fill2Run1 = 0;
bool Fill1Run2 = 0;
bool stage1inProgress = 0;
bool stage2inProgress = 0;
bool stage3inProgress = 0;
bool stage4inProgress = 0;



void loop() {
  //home; set as origin and move 5mm out
  if (m1Homed==false || m2Homed ==false){
    Serial.println("Start homing");
    homeBoth();
    stepper1.setCurrentPosition(0);
    stepper1.setCurrentPosition(0);
    Serial.println("Homing Done");
  }
  //1.Travel 4 inches negative on both cylinders to fully compress both
  if(initialCompression == true){
    stepper1.moveTo(-508000);
    stepper2.moveTo(-508000);
    initialCompression = 0;
    stage1inProgress = 1;
    Serial.println("Start Stage 1");    
  }
  //End stage 1 start stage 2
  if ((stage1inProgress == 1) && (stepper1.distanceToGo() == 0) && (stepper2.distanceToGo() == 0)){
      stage1inProgress = 0;
      stage2inProgress = 1;
      firstRunFirstCylinder = 1;
      Serial.println("End Stage 1");            
  }
  //2.Run first cylinder up(positive) 4 inches fast
  if(firstRunFirstCylinder == 1){
    stepper1.moveTo(0);
    stepper1.setMaxSpeed (30000.0);
    firstRunFirstCylinder = 0; 
    stage2inProgress = 1; 
    Serial.println("Start Stage 2");
  }
  
   //Interstage 1
  if((stepper1.distanceToGo()) == 2000 ){
    stepper2.moveTo(-506000)
  }


  //End stage 2 start stage 3
  if ((stage2inProgress == 1) && (stepper1.distanceToGo() == 0) && (stepper2.distanceToGo() == 0)){
      stage2inProgress = 0;
      stage3inProgress = 1;
      Fill2Run1 = 1;
      Serial.println("End Stage 2");
  }
  

  //3.Run 2nd cylinders up fast while bringing 1st cylinder down slow negative 4 inches
  if(Fill2Run1 == true){
    stepper1.moveTo(-508000);
    stepper1.setMaxSpeed (10000.0);
    stepper2.moveTo(0);
    stepper2.setMaxSpeed (30000.0);
    Fill2Run1 = 0; 
    stage3inProgress = 1; 
    Serial.println("Start Stage 3");
  }

 
  //End stage 3 start stage 4
  if ((stage3inProgress == 1) && (stepper1.distanceToGo() == 0) && (stepper2.distanceToGo() == 0)){
      stage3inProgress = 0;
      stage4inProgress = 1;
      Fill1Run2 = 1;
      Serial.println("End Stage 3");
  }
  
  // //4.Run 1st cylinder up fast while bringing 2nd cylinder down slow negative 4 inches
  if(Fill1Run2 == true){
    stepper1.moveTo(0);
    stepper1.setMaxSpeed (30000.0);
    stepper2.moveTo(-508000);
    stepper2.setMaxSpeed (10000.0);
    Fill1Run2 = 0; 
    stage4inProgress = 1; 
    Serial.println("Start Stage 4");
  }
  
  //End stage 3 start stage 4
  if ((stage4inProgress == 1) && (stepper1.distanceToGo() == 0) && (stepper2.distanceToGo() == 0)){
      stage4inProgress = 0;
      stage3inProgress = 1;
      Fill2Run1 = 1;
      Serial.println("End Stage 4");
  }
  

  //accelstepper 
  //0.0002 mm per 1 position of Accelstepper

  // //Travel distance of 4 inches(101.6mm) in steps
  // long stepper1TravelDistance = 254000;
  // long stepper2TravelDistance = 254000;
 
  stepper1.run();
  stepper2.run();
  
}