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

int deadCentreWidth = 30;

int positionPotmeter;    //huidige uitslag van potmeter vertaald van uitslag op plaat naar volledige bereik 
int analogPotmeter;  //uitlezing van potmeter op pin
int oldPosition;

int printDebugInfo;
int printNow;

int potMiddlePosition = 1023 / 2; //waarde als potmeter in het midden staat

int potMaxPositionLeft = 313;//0; //minimale stand van potmeter links
int potMaxPositionRight = 713;//potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

int maxPositionLeftFrontServo = 40; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = 140; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = 60; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = 120; //maximale uitslag naar rechts achterste servo

void setup() {
  Serial.begin(9600);
  printDebugInfo = 0;
  oldPosition = -1;

  servoFront.attach(SERVO_VOOR_PIN);  // servo voor
  servoRear.attach(SERVO_ACHTER_PIN); // servo achter
}

void debugSettings(int analogPotmeter, int positionPotmeter) {

  printNow++;
  if ((printDebugInfo != analogPotmeter) && (printNow % 15 == 0)) {
    Serial.println("============================");
    Serial.println("");
    Serial.print("Potmeter analog: ");
    Serial.println(analogPotmeter);
    Serial.print("Potmeter translated: ");
    Serial.println(positionPotmeter);

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
    analogPotmeter = potMiddlePosition;  
  }
 
  if (analogPotmeter < potMaxPositionLeft) {
    analogPotmeter = potMaxPositionLeft;
  }
  if (analogPotmeter > potMaxPositionRight) {
    analogPotmeter = potMaxPositionRight;
  }

  positionPotmeter = map(analogPotmeter, 0, 1023, potMaxPositionLeft, potMaxPositionRight);  

  // debugSettings(analogPotmeter, positionPotmeter);
  
  if (oldPosition != analogPotmeter) {
    updateServoPositions(analogPotmeter);
    oldPosition = analogPotmeter;
  }
}

void loop() {

  analogPotmeter = analogRead(POT_PIN);            // reads the value of the potentiometer (value between 0 and 1023)
  translatePositions(analogPotmeter);
  delay(15);                           
}

