/*
   v1 - eerste versie met 2 servo en variabele instelling



*/

#include <Servo.h>
#include <IRremote.h>

int IR_RECV_PIN = 11;
int SERVO_VOOR_PIN = 9;
int SERVO_ACHTER_PIN = 10;
int POT_PIN = 0; //potmeter pin


IRrecv irrecv(IR_RECV_PIN);
decode_results results;

Servo servoFront; // voorste servo (3 stuurassen)
Servo servoRear; // achterste servo (2 stuurassen)

int deadCentreWidth = 2;

bool needsToCenter;
bool centered;
bool outOfCenterLeft;
bool outOfCenterRight;

int analogPotmeterInput;  //uitlezing van potmeter op pin
int averagePotmeter;  //average value measured
int positionPotmeter;
  
//keep track of previous analog input values in configurable length array, more values more precies but slower
const int analogInputHistoryLength = 10;
int previousAnalogValues[analogInputHistoryLength];

int oldPosition;

int printDebugInfo;
int printNow;

const int maxValueMeasuredForPot = 1015;
int potMiddlePosition = 1020 / 2; //waarde als potmeter in het midden staat

int potMaxPositionLeft = 250; //minimale stand van potmeter links
int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

int maxPositionLeftFrontServo = 0;//60; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = 180;//120; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = 0;//40; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = 180;//140; //maximale uitslag naar rechts achterste servo

void setup() {
  needsToCenter = false;
  centered = true;
  outOfCenterLeft = false;
  outOfCenterRight = false;
    
  Serial.begin(9600);
  printDebugInfo = 0;
  oldPosition = -1;

  servoFront.attach(SERVO_VOOR_PIN);  // servo voor
  servoRear.attach(SERVO_ACHTER_PIN); // servo achter

  //setup a number of default middle positions
  for (int i=0;i<analogInputHistoryLength;i++) {
    previousAnalogValues[i] =  potMiddlePosition;
  }

//  irrecv.enableIRIn(); // Start the receiver
}

void debugSettings(int potmeter, int averagePotmeter) {

  printNow++;
  if ((printDebugInfo != potmeter) && (printNow % 8 == 0)) {
    Serial.println("============================");
    Serial.print("Potmeter analog: ");
    Serial.println(potmeter);
//    Serial.print("Potmeter average: ");
//    Serial.println(averagePotmeter);

    Serial.print("Needs to center: ");
    Serial.println(needsToCenter == 0 ? "false" : "true");
    Serial.print("Centered: ");
    Serial.println(centered == 0 ? "false" : "true");
    Serial.print("Out of center left: ");
    Serial.println(outOfCenterLeft == 0 ? "false" : "true");
    Serial.print("Out of center right: ");
    Serial.println(outOfCenterRight == 0 ? "false" : "true");
    
    Serial.println("Potmeter: ");
    Serial.print("Servo voor:  ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    Serial.print("Servo achter: ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));
//
//    Serial.println("Potmeter average: ");
//    Serial.print("Servo voor:  ");
//    Serial.println(map(averagePotmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
//    Serial.print("Servo achter: ");
//    Serial.println(map(averagePotmeter, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));

    printDebugInfo = potmeter;
    printNow = 0;
  }

}

void updateServoPositions(int relativePosition) {
  servoFront.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
  servoRear.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));
}
 
void translatePosition(int analogPotmeter, int averagePotmeter) {

  if ((analogPotmeter >= (potMiddlePosition - deadCentreWidth)) && (analogPotmeter <= (potMiddlePosition + deadCentreWidth))) {
    if (outOfCenterLeft || outOfCenterRight) {
      //we were out of center, so we need to adjust in the other direction and then go straight again.
      if (outOfCenterRight) {
        //adjust to the left (since we were right)
        updateServoPositions(potMiddlePosition - (deadCentreWidth*2));
        outOfCenterRight = false;
        delay(100);
      }
      if (outOfCenterLeft) {
        //adjust to the right (since we were left)
        updateServoPositions(potMiddlePosition + (deadCentreWidth*5));
        outOfCenterLeft = false;
        delay(100);
      }
    }
    
    analogPotmeter = potMiddlePosition;  
    centered = true;
    needsToCenter = false;
  }
 
  int positionPotmeter; 

  debugSettings(analogPotmeter, averagePotmeter);
  
  if (oldPosition != analogPotmeter) {
    updateServoPositions(analogPotmeter);
    oldPosition = analogPotmeter;
 
   //check if we are off center left or right 
    if (analogPotmeter <= (potMiddlePosition - deadCentreWidth)) {
      outOfCenterLeft = true; 
    } else if (analogPotmeter >= (potMiddlePosition + deadCentreWidth)) {
      outOfCenterRight = true;
    }
  }
}

int storeLatestAnalogValue(int analogPotmeter) {
  int summedPositions =  0;

  //loop through every previous value, sum them and shift them 1 position to the beginning
  for (int i=0;i<analogInputHistoryLength;i++) {
    summedPositions += previousAnalogValues[i];
    previousAnalogValues[i] = previousAnalogValues[i+1];
  }
  
  previousAnalogValues[analogInputHistoryLength+1] = analogPotmeter; //store latest at the end

  return int((summedPositions + analogPotmeter) / (analogInputHistoryLength + 1));
}

void loop() {

//  if (irrecv.decode(&results)) {
//    Serial.print(char(results.value));
//    irrecv.resume(); // Receive the next value    
//  }
  
  analogPotmeterInput = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 1023)

  positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, 0, maxValueMeasuredForPot);  

  if (positionPotmeter < potMaxPositionLeft) {
    positionPotmeter = potMaxPositionLeft;
  }
  if (positionPotmeter > potMaxPositionRight) {
    positionPotmeter = potMaxPositionRight;
  }
  
  averagePotmeter = storeLatestAnalogValue(positionPotmeter);
  translatePosition(positionPotmeter, averagePotmeter);
  delay(20);                           
}

