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
int potDeviation = 50;

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

const int minValueMeasuredForPot = 0;
const int maxValueMeasuredForPot = 1023;
int potMiddlePosition = maxValueMeasuredForPot / 2; //waarde als potmeter in het midden staat

int potMaxPositionLeft = 300; //minimale stand van potmeter links
int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

const int servoMinPulse = 750;
const int servoMaxPulse = 2250;

int maxPositionLeftFrontServo = servoMinPulse;//60; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = servoMaxPulse;//120; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = servoMinPulse;//40; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = servoMaxPulse;//140; //maximale uitslag naar rechts achterste servo

void setup() {
  // debug and needed for IR
  Serial.begin(9600);
  printDebugInfo = 0;
  oldPosition = -1;

  needsToCenter = false;
  centered = true;
  outOfCenterLeft = false;
  outOfCenterRight = false;
    
  // first steering servo at the front
  servoFront.attach(SERVO_VOOR_PIN);  
  
  // second steering servo at the front
  servoRear.attach(SERVO_ACHTER_PIN); 

  //setup a number of default middle positions
  for (int i=0;i<analogInputHistoryLength;i++) {
    previousAnalogValues[i] =  potMiddlePosition;
  }
  
  // Start the IR receiver
//  irrecv.enableIRIn(); 
}

void debugSettings(int potmeter) {

    return;

    Serial.println("============================");
    Serial.print("Potmeter analog: ");
    Serial.println(potmeter);

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

}

void updateServoPositions(int relativePosition) {
  servoFront.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
  servoRear.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));
}
 
void translatePosition(int analogPotmeter) {

//  averagePotmeter = storeLatestAnalogValue(positionPotmeter);
int averagePotmeter;

  if ((analogPotmeter >= (potMiddlePosition - deadCentreWidth)) && (analogPotmeter <= (potMiddlePosition + deadCentreWidth))) {
    if (outOfCenterLeft || outOfCenterRight) {
      //we were out of center, so we need to adjust in the other direction and then go straight again.
      if (outOfCenterRight) {
        //adjust to the left (since we were right)
//        updateServoPositions(potMiddlePosition - (deadCentreWidth*2));
        outOfCenterRight = false;
//        delay(100);
      }
      if (outOfCenterLeft) {
        //adjust to the right (since we were left)
//        updateServoPositions(potMiddlePosition + (deadCentreWidth*5));
        outOfCenterLeft = false;
//        delay(100);
      }
    }
    
    analogPotmeter = potMiddlePosition;  
    centered = true;
    needsToCenter = false;
  }
 
  int positionPotmeter; 

  debugSettings(analogPotmeter);
  
 // if (oldPosition != analogPotmeter) {
    updateServoPositions(analogPotmeter);
    oldPosition = analogPotmeter;
 
   //check if we are off center left or right 
    if (analogPotmeter <= (potMiddlePosition - deadCentreWidth)) {
      outOfCenterLeft = true; 
    } else if (analogPotmeter >= (potMiddlePosition + deadCentreWidth)) {
      outOfCenterRight = true;
    }
//  }
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

bool isNewPotPositionEnough(int potPosition) {
  //calculate average 
  int average = getSummedTotal() / analogInputHistoryLength;
  int deviation = abs(average - potPosition);
  char output[300];

  //check if current position greater than X percent
  if ( deviation > potDeviation ) {
//    sprintf(output, "Average: %d\nDeviation: %d\nPosition: %d\nLower position: %d\nHigher position: %d\n", average, deviation, potPosition, (average - deviation), (average + deviation));
//    Serial.println(output);
    return true;
  } else {
    return false;
  }
}

void loop() {
//  if (irrecv.decode(&results)) {
//    Serial.print(char(results.value));
//    irrecv.resume(); // Receive the next value    
//  }

  // reads the value of the potentiometer
  analogPotmeterInput = analogRead(POT_PIN);             
  
  // limit the pot values to what we expect them to be max left and right
  analogPotmeterInput = limitToMaxPositionsOnPlate(analogPotmeterInput);  

  if ( isNewPotPositionEnough(analogPotmeterInput) ) { 
    storeLatestAnalogValue(analogPotmeterInput);
    //and then update the servo positions

    // translate plate position to relative position between max left and right
    positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, minValueMeasuredForPot, maxValueMeasuredForPot); 
    
    // translate the relative pot position to a servo position and update servo positions when necessary
    translatePosition(positionPotmeter); 
  }
  

  //wait a while and loop
  delay(100);                           
}

