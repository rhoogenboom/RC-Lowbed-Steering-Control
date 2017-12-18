/*
  v1 - eerste versie met 2 servo en variabele instelling
  v2 - added serial communication
  v3 - adding average value to determine max change
*/

#include <Servo.h>
#include <SoftwareSerial.h>
//#include <IRremote.h>

#define inputHistoryLength 10

int SERVO_VOOR_PIN = 6;
int SERVO_ACHTER_PIN = 7;

SoftwareSerial serialInput(10, 11); // RX, TX
//IRrecv irrecv(IR_RECV_PIN);
//decode_results results;

bool startFound = false;
bool endFound = false;

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

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
  serialInput.begin(57600);

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

int getSummedTotal() {
  int total = 0;

    for (int i=0;i<inputHistoryLength;i++) {
      total += previousAnalogValues[i];
      previousAnalogValues[i] = previousAnalogValues[i+1];
    }

  return total;
}

bool isNewPotPositionEnough(int potPosition) {
  //calculate average 
  int average = getSummedTotal() / inputHistoryLength;
  int deviation = abs(average - potPosition);
  char output[300];

  //check if current position greater than X percent
  if ( deviation > potDeviation ) {
    sprintf(output, "Average: %d\nDeviation: %d\nPosition: %d\nLower position: %d\nHigher position: %d\n", average, deviation, potPosition, (average - deviation), (average + deviation));
    Serial.println(output);
    return true;
  } else {
    Serial.print(".");
    return false;
  }
}

void storeLatestAnalogValue(int analogPotmeter) {
  //store latest pot position 
  //TODO: should be done based on the last X measurements which lead to a movement of the servos, only servo movements should be averaged out
  int summedPositions =  0;

  //loop through every previous value, sum them and shift them 1 position to the beginning eventually making room at the end for the latest value
  for (int i=0;i<inputHistoryLength;i++) {
    summedPositions += previousAnalogValues[i];
    previousAnalogValues[i] = previousAnalogValues[i+1];
  }
  
  //store latest at the end
  previousAnalogValues[inputHistoryLength+1] = analogPotmeter; 
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


int limitToMaxPositionsServo(int input) {
  //below limits the range which we consider the full movement between truck and trailer angle, either left of right
  
  //if input is smaller than the minimum we would accept, update to minimum
  if (input < servoMinPulse) {
    input = servoMinPulse;
  }
  //if input is larger than the maximum we would accept, update to maximum
  if (input > servoMaxPulse) {
    input = servoMaxPulse;
  }
  return input;
}

void loop() {
  // read input from IR/serial
  int analogPotmeterInput = readSerialInput();   

//  if ( isNewPotPositionEnough(analogPotmeterInput) ) {
//storeLatestAnalogValue(analogPotmeterInput);

  
  if (analogPotmeterInput != -1) {
    analogPotmeterInput = limitToMaxPositionsServo(analogPotmeterInput);
    debugSettings(analogPotmeterInput);
    updateServoPositions(analogPotmeterInput);
    delay(250);
  }  
}

