/*
  v1 - eerste versie met 2 servo en variabele instelling
  v2 - added serial communication
*/

#include <Servo.h>
#include <SoftwareSerial.h>
//#include <IRremote.h>

int SERVO_VOOR_PIN = 6;
int SERVO_ACHTER_PIN = 7;

SoftwareSerial serialInput(10, 11); // RX, TX
//IRrecv irrecv(IR_RECV_PIN);
//decode_results results;

bool startFound = false;
bool endFound = false;

Servo servoFront; // voorste servo (3 stuurassen)
Servo servoRear; // achterste servo (2 stuurassen)

int oldPosition;

String characterValues = ""; //concatenated numbers read over IR

void setup() {
  // debug and needed for IR
  Serial.begin(9600);
  oldPosition = -1;

  // first steering servo at the front
  servoFront.attach(SERVO_VOOR_PIN);  
  
  // second steering servo at the front
  servoRear.attach(SERVO_ACHTER_PIN); 

  // Start the IR receiver
  //irrecv.enableIRIn(); 

  // set the data rate for the SoftwareSerial port
  serialInput.begin(9600);

  updateServoPositions(1500);
}

void debugSettings(int servo) {
  if ( abs(servo-oldPosition) > 3) {
    //Serial.write(27); Serial.print("[2J"); // clear screen command
    //Serial.write(27); Serial.print("[H"); // home cursor

    Serial.print("Servo:   ");
    Serial.println(servo);
   
    oldPosition = servo;
  }
}

void updateServoPositions(int relativePosition) {
  servoFront.write(relativePosition);
  servoRear.write(relativePosition);
}

int readSerialInput() {
 if (serialInput.available()) {
    //read one byte
    unsigned char input = serialInput.read();
    switch (input) {
      case 0x02:
        startFound = true;
        endFound = false;       
        break;
     case 0x03:
        startFound = false;
        endFound = true;
        break;
     default:
        characterValues = characterValues + char(input);
    }

    if (endFound) {
      int r = characterValues.toInt();
      endFound = false;
      characterValues = ""; 
      return r;
    }
  }

  return -1;  
}

void loop() {
  // read input from IR/serial
  int analogPotmeterInput = readSerialInput();   
  
  if (analogPotmeterInput != -1) {
    debugSettings(analogPotmeterInput);
    updateServoPositions(analogPotmeterInput);
  }
}

