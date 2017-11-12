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

int positionPotmeter;    //huidige uitslag van potmeter vertaald van uitslag op plaat naar volledige bereik 
int analogPotmeter;  //uitlezing van potmeter op pin
int oldPosition;

int printDebugInfo;
int printNow;

int potMiddlePosition = 1023 / 2; //waarde als potmeter in het midden staat

int potMaxPositionLeft = 0;//313;//0; //minimale stand van potmeter links
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

//  irrecv.enableIRIn(); // Start the receiver
}

void debugSettings(int analogPotmeter, int positionPotmeter) {

  printNow++;
  if ((printDebugInfo != analogPotmeter) && (printNow % 20 == 0)) {
    Serial.println("============================");
    Serial.print("Potmeter analog: ");
    Serial.println(analogPotmeter);
    Serial.print("Potmeter translated: ");
    Serial.println(positionPotmeter);

    Serial.print("Needs to center: ");
    Serial.println(needsToCenter == 0 ? "false" : "true");
    Serial.print("Centered: ");
    Serial.println(centered == 0 ? "false" : "true");
    Serial.print("Out of center left: ");
    Serial.println(outOfCenterLeft == 0 ? "false" : "true");
    Serial.print("Out of center right: ");
    Serial.println(outOfCenterRight == 0 ? "false" : "true");
    
    Serial.println("Potmeter analog: ");
    Serial.print("Servo voor:  ");
    Serial.println(map(analogPotmeter, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    Serial.print("Servo achter: ");
    Serial.println(map(analogPotmeter, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftRearServo, maxPositionRightRearServo));

    Serial.println("Potmeter translated: ");
    Serial.print("Servo voor:  ");
    Serial.println(map(positionPotmeter, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    Serial.print("Servo achter: ");
    Serial.println(map(positionPotmeter, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftRearServo, maxPositionRightRearServo));

    printDebugInfo = analogPotmeter;
    printNow = 0;
  }

}

void updateServoPositions(int relativePosition) {
  servoFront.write(map(relativePosition, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftFrontServo, maxPositionRightFrontServo));
  servoRear.write(map(relativePosition, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftRearServo, maxPositionRightRearServo));
  //  servoFront.write(map(positionPotmeter, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftFrontServo, maxPositionRightFrontServo));
  //  servoRear.write(map(positionPotmeter, potMaxPositionLeft, potMaxPositionRight, maxPositionLeftRearServo, maxPositionRightRearServo));
}
 
void translatePosition(int analogPotmeter) {


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
 
  if (analogPotmeter < potMaxPositionLeft) {
    analogPotmeter = potMaxPositionLeft;
  }
  if (analogPotmeter > potMaxPositionRight) {
    analogPotmeter = potMaxPositionRight;
  }

  positionPotmeter = map(analogPotmeter, 0, 1023, potMaxPositionLeft, potMaxPositionRight);  

  debugSettings(analogPotmeter, positionPotmeter);
  
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

void loop() {
//  if (irrecv.decode(&results)) {
//    Serial.print(char(results.value));
//    irrecv.resume(); // Receive the next value    
//  }
  
  analogPotmeter = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 1023)
  translatePosition(analogPotmeter);
  delay(15);                           
}

