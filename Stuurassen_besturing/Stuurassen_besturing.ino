/*
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
  
  v1 - eerste versie met 2 servo en variabele instelling
  v2 - added serial communication
*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

//http://www.firewall.cx/cisco-technical-knowledgebase/cisco-wireless/828-cisco-wireless-wlan-keygen.html
const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63};   

//RH TODO remove pin 10 and 11 old IR comms

int SERVO_VOOR_PIN = 6;
int SERVO_ACHTER_PIN = 7;

bool startFound = false;
bool endFound = false;

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

Servo servoFront; // voorste servo (3 stuurassen)
Servo servoRear; // achterste servo (2 stuurassen)

int oldPosition;

String characterValues = ""; //concatenated numbers read over IR

void setup() {
  Serial.begin(9600);
  oldPosition = -1;

  // first steering servo at the front
  servoFront.attach(SERVO_VOOR_PIN);  
  // second steering servo at the front
  servoRear.attach(SERVO_ACHTER_PIN); 

  updateServoPositions(1500);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
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

int readWifiInput() {
 if (radio.available()) {
    radio.read(&characterValues, sizeof(characterValues));
    int r = characterValues.toInt();
    characterValues = ""; 
    return r;
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
  int analogPotmeterInput = readWifiInput();   
  
  if (analogPotmeterInput != -1) {
    analogPotmeterInput = limitToMaxPositionsServo(analogPotmeterInput);
    debugSettings(analogPotmeterInput);
    updateServoPositions(analogPotmeterInput);
    delay(250);
  }  
}

