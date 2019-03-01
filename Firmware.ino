#include "Arduino.h"
#include "Joystick.h"
#include "Servo.h"

#define JOYSTICK_PIN_SW  2
#define JOYSTICK_PIN_VRX  A3
#define JOYSTICK_PIN_VRY  A4
#define SERVOSM1_1_PIN_SIG  3
#define SERVOSM2_2_PIN_SIG  4
#define SERVOSM3_3_PIN_SIG  5
#define Min 0
#define Max 1
#define Start 2

const int baseMin = 20;
const int baseMax = 90; 
const int baseStart = 55;


const int armMin = 20;
const int armMax = 60;
const int armStart = 60;


const int gripperMin = 0;
const int gripperMax = 90;
const int gripperStart = 90;

int rest;

int pos[3][3] = {baseMin, baseMax, baseStart,
      armMin, armMax, armStart, 
      gripperMin, gripperMax, gripperStart};

      
Joystick joystick(JOYSTICK_PIN_VRX,JOYSTICK_PIN_VRY,JOYSTICK_PIN_SW);
Servo activeServo;
short int active = 0;
void setup() 
{
    Serial.begin(9600);
    while (!Serial) ;
    Serial.println("start");
    
    activeServo.attach(SERVOSM1_1_PIN_SIG);
    activeServo.write(baseStart);
    delay(100);
    activeServo.detach();
    activeServo.attach(SERVOSM2_2_PIN_SIG);
    activeServo.write(armStart);
    delay(100);
    activeServo.detach();
    activeServo.attach(SERVOSM3_3_PIN_SIG);
    activeServo.write(gripperStart);
    delay(100);
    activeServo.detach();

    activeServo.attach(SERVOSM1_1_PIN_SIG);

    rest = joystick.getX();
}

void nextServo() {
  if (active == 0) {
    activeServo.attach(SERVOSM1_1_PIN_SIG);
  } else if (active == 1) {
    activeServo.attach(SERVOSM2_2_PIN_SIG);
  } else {
    activeServo.attach(SERVOSM3_3_PIN_SIG);
  }
  active++;
  active %= 3;
  while(abs(joystick.getX()-rest) < 100);
}


void test() {
  activeServo.write(pos[active][Min]);
  delay(500);
  activeServo.write(pos[active][Max]);
  delay(500);
  activeServo.write(pos[active][Start]);
  delay(500);
}

void moveServo(int dir) {
  int nextPos;
  int curPos = activeServo.read();
  if (abs(dir - rest) < 100) {
    return;
  } else if (dir > rest) {
    nextPos = (curPos != pos[active][Max])?(curPos+1):(curPos);
  } else {
    nextPos = (curPos != pos[active][Min])?(curPos-1):(curPos);
  }
  activeServo.write(nextPos);
}

void loop() {
  
  int dir =  joystick.getX();
  bool button =  joystick.getSW();
  if (button) {
    nextServo();
  }
  moveServo(dir);
  delay(10);
  
}
