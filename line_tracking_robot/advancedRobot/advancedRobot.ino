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
#define IR_LEFT_INNER 11
#define IR_RIGHT_INNER 12


// Instantiating the motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  // default board constructor

//Accessing the motors from the motorshield
Adafruit_DCMotor *MOTOR_A = AFMS.getMotor(MOTOR_FRONT_LEFT);
Adafruit_DCMotor *MOTOR_B = AFMS.getMotor(MOTOR_FRONT_RIGHT);
Adafruit_DCMotor *MOTOR_C = AFMS.getMotor(MOTOR_BACK_LEFT);
Adafruit_DCMotor *MOTOR_D = AFMS.getMotor(MOTOR_BACK_RIGHT);

int lux = 0;
int lux_loop = 0;
bool started = false;

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

  // LightSensor
  while (!started) {
    lightSensor.start();
    lux = lightSensor.getLux();
    Serial.print("Lux_setup:");
    Serial.println(lux);
    if (lux > 900) {
      delay(500);
      started = true;
      break;
    }
  }
}

void loop() {

  // LightSensor
  lightSensor.start();
  lux_loop = lightSensor.getLux();
  if (lux_loop > 900) {
    started = false;
  }
  if (started) {
    int IR_LEFT = digitalRead(IR_LEFT_PIN);
    int IR_RIGHT = digitalRead(IR_RIGHT_PIN);
    int IR_LEFT_IN = digitalRead(IR_LEFT_INNER);
    int IR_RIGHT_IN = digitalRead(IR_RIGHT_INNER);

    if ((IR_LEFT == 1 && IR_RIGHT == 0 && IR_LEFT_IN == 1 && IR_RIGHT_IN == 1) || (IR_LEFT_IN == 1 && IR_RIGHT_IN == 0 && IR_RIGHT == 0 && IR_LEFT == 1)) {
      //turn left
      MOTOR_C->setSpeed(0);
      MOTOR_D->setSpeed(0);
      MOTOR_C->run(RELEASE);
      MOTOR_D->run(RELEASE);
      delay(5);
      MOTOR_C->setSpeed(125);
      MOTOR_D->setSpeed(125);
      MOTOR_A->setSpeed(233);
      MOTOR_B->setSpeed(233);
      MOTOR_C->run(BACKWARD);
      MOTOR_D->run(BACKWARD);
      MOTOR_A->run(FORWARD);
      MOTOR_B->run(FORWARD);
      delay(10);

    } else if ((IR_LEFT == 0 && IR_RIGHT == 1 && IR_LEFT_IN == 1 && IR_RIGHT_IN == 1) || (IR_LEFT == 0 && IR_LEFT_IN == 0 && IR_RIGHT_IN == 1 && IR_RIGHT == 1)) {
      //turn right

      MOTOR_A->setSpeed(0);
      MOTOR_B->setSpeed(0);
      MOTOR_A->run(RELEASE);
      MOTOR_B->run(RELEASE);
      delay(5);
      MOTOR_C->setSpeed(233);
      MOTOR_D->setSpeed(233);
      MOTOR_A->setSpeed(125);
      MOTOR_B->setSpeed(125);
      MOTOR_A->run(BACKWARD);
      MOTOR_B->run(BACKWARD);
      MOTOR_C->run(FORWARD);
      MOTOR_D->run(FORWARD);
      delay(10);
      
    } else if (IR_LEFT == IR_RIGHT) {
      MOTOR_C->setSpeed(252);
      MOTOR_D->setSpeed(252);
      MOTOR_A->setSpeed(252);
      MOTOR_B->setSpeed(252);
      MOTOR_C->run(FORWARD);
      MOTOR_D->run(FORWARD);
      MOTOR_A->run(FORWARD);
      MOTOR_B->run(FORWARD);
    }
  } else if (!started) {
    MOTOR_A->setSpeed(0);
    MOTOR_A->run(RELEASE);
    MOTOR_B->setSpeed(0);
    MOTOR_B->run(RELEASE);
    MOTOR_C->setSpeed(0);
    MOTOR_C->run(RELEASE);
    MOTOR_D->setSpeed(0);
    MOTOR_D->run(RELEASE);
  }
}