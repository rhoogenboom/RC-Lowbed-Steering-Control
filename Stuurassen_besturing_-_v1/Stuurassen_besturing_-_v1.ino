/*
   v1 - eerste versie met 2 servo en variabele instelling



*/

#include <Servo.h>
#include <IRremote.h>

int IR_RECV_PIN = 11;
int SERVO_VOOR_PIN = 6;
int SERVO_ACHTER_PIN = 7;
int POT_PIN = A2; //potmeter pin


//IRrecv irrecv(IR_RECV_PIN);
decode_results results;

Servo servoFront; // voorste servo (3 stuurassen)
Servo servoRear; // achterste servo (2 stuurassen)

int deadCentreWidth = 2;
int potDeviation = 25;

bool needsToCenter;
bool centered;
bool outOfCenterLeft;
bool outOfCenterRight;

//int analogPotmeterInput;  //uitlezing van potmeter op pin
int averagePotmeter;  //average value measured
int positionPotmeter;
  
//keep track of previous analog input values in configurable length array, more values more precies but slower
const int analogInputHistoryLength = 10;
int previousAnalogValues[analogInputHistoryLength];

int oldPosition;

int printNow;

const int minValueMeasuredForPot = 0;
const int maxValueMeasuredForPot = 1023;
int potMiddlePosition = maxValueMeasuredForPot / 2; //waarde als potmeter in het midden staat

int potMaxPositionLeft = 300; //minimale stand van potmeter links
int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

int maxPositionLeftFrontServo = servoMinPulse;//60; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = servoMaxPulse;//120; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = servoMinPulse;//40; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = servoMaxPulse;//140; //maximale uitslag naar rechts achterste servo

String characterValues = ""; //concatenated numbers read over IR

void setup() {

  pinMode(POT_PIN, INPUT); 

  // debug and needed for IR
  Serial.begin(9600);
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
  //irrecv.enableIRIn(); 
}

void debugSettings(int potmeter) {

  if ( abs(potmeter-oldPosition) > 3) {
    Serial.println("============================");
    Serial.print("Potmeter analog: ");
    Serial.println(potmeter);

//    Serial.print("Needs to center: ");
//    Serial.println(needsToCenter == 0 ? "false" : "true");
//    Serial.print("Centered: ");
//    Serial.println(centered == 0 ? "false" : "true");
//    Serial.print("Out of center left: ");
//    Serial.println(outOfCenterLeft == 0 ? "false" : "true");
//    Serial.print("Out of center right: ");
//    Serial.println(outOfCenterRight == 0 ? "false" : "true");
    
    Serial.println("Potmeter: ");
    Serial.print("Servo voor:  ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    Serial.print("Servo achter: ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));
    
    oldPosition = potmeter;
  }

}

void updateServoPositions(int relativePosition) {
  servoFront.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
  servoRear.write(map(relativePosition, 0, maxValueMeasuredForPot, maxPositionLeftRearServo, maxPositionRightRearServo));
}
 
void translatePosition(int analogPotmeter) {

  if ((analogPotmeter >= (potMiddlePosition - deadCentreWidth)) && (analogPotmeter <= (potMiddlePosition + deadCentreWidth))) {
    centered = true;
    needsToCenter = false;
    
    if (outOfCenterRight) {
      outOfCenterRight = false;
    }

    if (outOfCenterLeft) {
      outOfCenterLeft = false;
    }
  }

    //check if we are off center left or right 
  if (analogPotmeter <= (potMiddlePosition - deadCentreWidth)) {
    outOfCenterLeft = true; 
    centered = false;
  }
  if (analogPotmeter >= (potMiddlePosition + deadCentreWidth)) {
    outOfCenterRight = true;
    centered = false;
  }

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

//bool isNewPotPositionEnough(int potPosition) {
//  //calculate average 
//  int average = getSummedTotal() / analogInputHistoryLength;
//  int deviation = abs(average - potPosition);
//  char output[300];
//
//  //check if current position greater than X percent
//  if ( deviation > potDeviation ) {
//    sprintf(output, "Average: %d\nDeviation: %d\nPosition: %d\nLower position: %d\nHigher position: %d\n", average, deviation, potPosition, (average - deviation), (average + deviation));
//    Serial.println(output);
//    return true;
//  } else {
//    Serial.print(".");
//    return false;
//  }
//}

void loop() {

  // reads the value of the potentiometer over IR
//  if (irrecv.decode(&results)) {

//    if (int(results.value) == 2528) {
      // read input from IR
      //int analogPotmeterInput = characterValues.toInt();   
      // reads the value of the potentiometer
      int analogPotmeterInput = analogRead(POT_PIN); 

      if (analogPotmeterInput == 0) Serial.println("WTF!!!!!"); 
      //reset IR value back to empty string
      characterValues = "";

      // limit the pot values to what we expect them to be max left and right
      analogPotmeterInput = limitToMaxPositionsOnPlate(analogPotmeterInput);  
    
      storeLatestAnalogValue(analogPotmeterInput);
      // translate plate position to relative position between max left and right
      positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, minValueMeasuredForPot, maxValueMeasuredForPot); 
      
      // translate the relative pot position to a servo position and update servo positions when necessary
      translatePosition(positionPotmeter); 

//    } else {
//      characterValues = characterValues + char(results.value);
//    }
//    irrecv.resume(); // Receive the next value    
//  }
  
  //wait a while and loop
  delay(10);                           
}

