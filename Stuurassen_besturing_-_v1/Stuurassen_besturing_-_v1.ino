/*
  v1 - eerste versie met 2 servo en variabele instelling
  v2 - added serial communication
*/

#include <Servo.h>
#include <SoftwareSerial.h>
//#include <IRremote.h>

int SERVO_VOOR_PIN = 6;
int SERVO_ACHTER_PIN = 7;
int POT_PIN = A2; //potmeter pin

SoftwareSerial serialInput(10, 11); // RX, TX
//IRrecv irrecv(IR_RECV_PIN);
//decode_results results;

bool startFound = false;
bool endFound = false;


Servo servoFront; // voorste servo (3 stuurassen)
Servo servoRear; // achterste servo (2 stuurassen)

int deadCentreWidth = 2;
int potDeviation = 25;

int averagePotmeter;  //average value measured
int positionPotmeter;
  
//keep track of previous analog input values in configurable length array, more values more precies but slower
const int analogInputHistoryLength = 10;
int previousAnalogValues[analogInputHistoryLength];

int oldPosition;

const int minValueMeasuredForPot = 0;
const int maxValueMeasuredForPot = 1023;
int potMiddlePosition = 504; //overrule door plaatsing van magneet maxValueMeasuredForPot / 2; //waarde als potmeter in het midden staat

const int potMaxPositionLeft = 300; //minimale stand van potmeter links
const int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

int maxPositionLeftFrontServo = servoMinPulse; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = servoMaxPulse; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = servoMinPulse; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = servoMaxPulse; //maximale uitslag naar rechts achterste servo

String characterValues = ""; //concatenated numbers read over IR

void setup() {
  pinMode(POT_PIN, INPUT); 

  // debug and needed for IR
  Serial.begin(9600);
  oldPosition = -1;

  // first steering servo at the front
  servoFront.attach(SERVO_VOOR_PIN);  
  
  // second steering servo at the front
  servoRear.attach(SERVO_ACHTER_PIN); 

  //setup a number of default middle positions
  for (int i=0;i<analogInputHistoryLength;i++) {
    previousAnalogValues[i] =  potMiddlePosition;
  }
  
  // Start the IR receiver
  //irrecv.enableIRIn(); 

  // set the data rate for the SoftwareSerial port
  serialInput.begin(9600);
}

void debugSettings(int potmeter) {

  if ( abs(potmeter-oldPosition) > 3) {
    //Serial.write(27); Serial.print("[2J"); // clear screen command
    //Serial.write(27); Serial.print("[H"); // home cursor
  
    Serial.print("Potmeter analog:    ");
    Serial.print(potmeter);
  
    Serial.print("    Servo voor:   ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
   
    oldPosition = potmeter;
  }

}

void updateServoPositions(int relativePosition) {
  servoFront.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
  servoRear.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));
}
 
void translatePosition(int analogPotmeter) {
    debugSettings(analogPotmeter);
    updateServoPositions(analogPotmeter);
}

void storeLatestAnalogValue(int analogPotmeter) {
  //store latest pot position 
  //TODO: should be done based on the last X measurements which lead to a movement of the servos, only servo movements should be averaged out
  
  int summedPositions =  0;

  //loop through every previous value, sum them and shift them 1 position to the beginning eventually making room at the end for the latest value
  for (int i=0;i<analogInputHistoryLength;i++) {
    summedPositions += previousAnalogValues[i];
    previousAnalogValues[i] = previousAnalogValues[i+1];
  }
  
  //store latest at the end
  previousAnalogValues[analogInputHistoryLength+1] = analogPotmeter; 
}

int limitToMaxPositionsOnPlate(int input) {
  //below limits the range which we consider the full movement between truck and trailer angle, either left of right
  
  //if input is smaller than the minimum we would accept, update to minimum
  if (input < potMaxPositionLeft) {
    input = potMaxPositionLeft;
  }
  //if input is larger than the maximum we would accept, update to maximum
  if (input > potMaxPositionRight) {
    input = potMaxPositionRight;
  }
  return input;
}

int getSummedTotal() {
  int total = 0;

    for (int i=0;i<analogInputHistoryLength;i++) {
      total += previousAnalogValues[i];
      previousAnalogValues[i] = previousAnalogValues[i+1];
    }

  return total;
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
  
    // limit the pot values to what we expect them to be max left and right
    analogPotmeterInput = limitToMaxPositionsOnPlate(analogPotmeterInput);  
  
    //storeLatestAnalogValue(analogPotmeterInput);
    // translate plate position to relative position between max left and right
    //positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, minValueMeasuredForPot, maxValueMeasuredForPot); 
    
    // translate the relative pot position to a servo position and update servo positions when necessary
    translatePosition(analogPotmeterInput); 
  }

  //wait a while and loop
  //delay(10);                           
}

