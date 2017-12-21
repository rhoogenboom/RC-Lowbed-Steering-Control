// PinChangeInt is for the multiprop channel data from the receiver
#include <PinChangeInt.h>

#define MULTI_IN_PIN A0

volatile int current_servo = 0;
volatile unsigned long ulAuxInShared;
volatile unsigned long ulAuxStart;
volatile int multi_servo[10];

unsigned long servo_time;
unsigned long current_time;
int servos_exist = true;

#define SERVOMIN  2000 // this is the 'minimum' pulse length count (1000 * 2)
#define SERVOMID  3000 // this is the 'middle'  pulse length count (1500 * 2)
#define SERVOMAX  4000 // this is the 'maximum' pulse length count (2000 * 2)


void setup() {
  // put your setup code here, to run once:
  byte error, address;

  Serial.begin(38400); // other values include 9600, 14400, 57600 etc.

  servoStart();

  for (int servonum = 1; servonum < 9; servonum++) {
    multi_servo[servonum] = 1500;
  }


  ulAuxInShared = 0;

  //delay(3000);

  servo_time = millis();

  // set up interrupt for receiver input

  PCintPort::attachInterrupt(MULTI_IN_PIN, calcAux, CHANGE);
}


void loop() {
  // put your main code here, to run repeatedly:
  int servonum = 0;
  float servoval = 1500;

  for (servonum = 1; servonum < 9; servonum++) {
    servoval = multi_servo[servonum];
    servoWrite(servonum - 1, servoval);
  }

  delay(10);
}


void calcAux()
{
  if (digitalRead(MULTI_IN_PIN) == HIGH)
  {
    ulAuxStart = micros();
  }
  else
  {
    ulAuxInShared = (uint16_t)(micros() - ulAuxStart);
    current_servo++;

    if (ulAuxInShared < 1000) {
      current_servo = 0;
    }

    if (ulAuxInShared > 2000) {
      current_servo = 0;
    }

    if (current_servo < 10) {
      multi_servo[current_servo] = ulAuxInShared;
    }
  }
}

// this function initializes the controller to set each servo speed to be 0x18 - 1/5th normal speed?? documentation not really clear on this
// i do this to slow down the response of the servo so it isn't jerky like the i2c version
void servoStart() {
  unsigned char cmd [5] = {0x80, 0x01, 0x01, 0x00, 0x18 };
  int servonum = 0;

  for (servonum = 0; servonum < 8; servonum++) {
    cmd[3] = servonum;

    Serial.write(cmd, 5);
    Serial.flush();
  }
}


// you can use this function if you'd like to set the pulse length in useconds
// e.g. servoWrite(0, 1500) is a 1500usecond pulsewidth
void servoWrite(uint8_t servo_num, unsigned int pulse) {
  unsigned char cmd [6] = {0x80, 0x01, 0x04, 0x00, 0x00, 0x00 };
  unsigned int val = pulse;
  val = val * 2;

  cmd[3] = servo_num;
  cmd[4] = lowByte((val >> 7));
  cmd[5] = lowByte(0x7f & val);

  Serial.write(cmd, 6);
  Serial.flush();
}
