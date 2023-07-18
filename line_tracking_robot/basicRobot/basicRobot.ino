#include <hp_BH1750.h>
hp_BH1750 lightSensor;

#include <Adafruit_MotorShield.h>

// Defining motor terminals
#define MOTOR_FRONT_LEFT 1
#define MOTOR_FRONT_RIGHT 2
#define MOTOR_BACK_LEFT 3
#define MOTOR_BACK_RIGHT 4
#define IR_LEFT_PIN 9
#define IR_RIGHT_PIN 10


// Instantiating the motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  // default board constructor

//Accessing the motors from the motorshield
Adafruit_DCMotor *MOTOR_A = AFMS.getMotor(MOTOR_FRONT_LEFT);
Adafruit_DCMotor *MOTOR_B = AFMS.getMotor(MOTOR_FRONT_RIGHT);
Adafruit_DCMotor *MOTOR_C = AFMS.getMotor(MOTOR_BACK_LEFT);
Adafruit_DCMotor *MOTOR_D = AFMS.getMotor(MOTOR_BACK_RIGHT);



void setup() {

  // LightSensor
  lightSensor.begin(BH1750_TO_GROUND);
  // IR
  pinMode(IR_RIGHT_PIN, INPUT);
  pinMode(IR_LEFT_PIN, INPUT);

  // Turn on the motorshield and PWM driver
  AFMS.begin();
  //Reset Motors (ser speed to 0 and release all the gates)
  MOTOR_A->setSpeed(0);
  MOTOR_A->run(RELEASE);
  MOTOR_B->setSpeed(0);
  MOTOR_B->run(RELEASE);
  MOTOR_C->setSpeed(0);
  MOTOR_C->run(RELEASE);
  MOTOR_D->setSpeed(0);
  MOTOR_D->run(RELEASE);


  //pinMode(LED_PIN, OUTPUT);
}


bool started = false; 

void loop() {

  // LightSensor
  lightSensor.start();
  int lux = lightSensor.getLux();

  if (lux > 900) {
    started = true;
  }
  while(started){
      //IR
  int IR_LEFT = digitalRead(IR_LEFT_PIN);
  int IR_RIGHT = digitalRead(IR_RIGHT_PIN);
  if (IR_LEFT == 0 && IR_RIGHT == 0){
    MOTOR_C->setSpeed(255);
    MOTOR_D->setSpeed(255);
    MOTOR_A->setSpeed(255);
    MOTOR_B->setSpeed(255);
    MOTOR_C->run(FORWARD);
    MOTOR_D->run(FORWARD);
    MOTOR_A->run(FORWARD);
    MOTOR_B->run(FORWARD);
  
  } else if (IR_LEFT == 0 && IR_RIGHT == 1) {
    //turn left
    MOTOR_C->setSpeed(0);
    MOTOR_D->setSpeed(125);
    MOTOR_A->setSpeed(255);
    MOTOR_B->setSpeed(255);
    MOTOR_C->run(RELEASE);
    MOTOR_D->run(BACKWARD);
    MOTOR_A->run(FORWARD);
    MOTOR_B->run(FORWARD);

    delay(11);
  } else if (IR_LEFT == 1 && IR_RIGHT == 0){
    //turn right
    MOTOR_C->setSpeed(255);
    MOTOR_D->setSpeed(255);
    MOTOR_A->setSpeed(125);
    MOTOR_B->setSpeed(0);
    MOTOR_A->run(BACKWARD);
    MOTOR_B->run(RELEASE);
    MOTOR_C->run(FORWARD);
    MOTOR_D->run(FORWARD);
    delay(11);
  } else if (IR_LEFT == 1 && IR_RIGHT == 1) {
    MOTOR_A->setSpeed(0);
    MOTOR_A->run(RELEASE);
    MOTOR_B->setSpeed(0);
    MOTOR_B->run(RELEASE);
    MOTOR_C->setSpeed(0);
    MOTOR_C->run(RELEASE);
    MOTOR_D->setSpeed(0);
    MOTOR_D->run(RELEASE);
    started = false;
    break;
  } 
  }
}