#include <Wire.h>
#include <PinChangeInterrupt.h>
// next line is optional
#include <Adafruit_PWMServoDriver.h>

#define SERVO_IN_PIN 10
volatile unsigned long ulServoInShared= 0;
volatile unsigned long ulServoStart = 0;
volatile int multi_servo[17];
volatile int current_servo = 0;

//next three lines are optional
// flag to indicate the i2c servo driver is attached
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
boolean servos_exist = false;

void setup() {
  byte error, address;

  Serial.begin(9600); // other values include 9600, 14400, 57600 etc.

  // set up interrupt for receiver input
  attachInterrupt(SERVO_IN_PIN, calcServo, CHANGE);
  
  //from here to "end optional" comment is optional
  address = 0x40;  // i2c address for servo driver
  // check to see if i2c device exists
  Wire.begin();
  Wire.beginTransmission(address);
  error = Wire.endTransmission();
  
  //device exists
  if (error == 0)
  {
    Serial.print("I2C device found at address ");
    Serial.println(address);

    servos_exist = true;

    pwm.begin();

    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  //set the default values for all 16 servos - 366 = mid range of driver for servo (1500)
  //driver is kind of odd... values range from 0 to 1000
  // which represent 0 milliseconds to 4096 milliseconds long pulse.
  // we want a min pulse of 1000, mid of 1500, max of 2000 milliseconds
  // that equates to a min val of 244, mid of 366, max of 488
    uint8_t servonum = 0;
    for (servonum = 0; servonum < 16; servonum++) {
      pwm.setPWM(servonum, 0, 366);
      multi_servo[servonum] = 1500;
    }
  }
  //end optional
}

void loop() {
  int servonum = 0;
  int servoval = 360;
  float tempval = 360.0;
  
  // array slot zero is sync - 1 thru 9 are valid servo pulses
  for (servonum = 1; servonum < 9; servonum++) {
    //from here to "end optional" comment is optional
    if (servos_exist == true) {
    // convert from rx level to driver level
    tempval = multi_servo[servonum];
    tempval = tempval * 0.244140625;
    servoval = round(tempval);
    // want to use servo slots 0 to 8
    pwm.setPWM(servonum-1, 0, servoval);
    }
    // end optional
    
    Serial.print("Servo: ");
    Serial.print(servonum);
    Serial.print(", ");
    Serial.println(multi_servo[servonum]);
  }
}


void calcServo()
{
  if (digitalRead(SERVO_IN_PIN) == HIGH)
  {
    ulServoStart = micros();
  }
  else
  {
    ulServoInShared = (uint16_t)(micros() - ulServoStart);
    current_servo++;
    
    if (ulServoInShared < 1000) {
      current_servo = 0;
    }

    if (ulServoInShared > 2000) {
      current_servo = 0;
    }
    
    if (current_servo < 10) {
      multi_servo[current_servo] = ulServoInShared;
    }
  }
}
