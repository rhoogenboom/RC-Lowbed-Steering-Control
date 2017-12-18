/*
   v1 - plate control with receiver overrule
*/
#include <SoftwareSerial.h>
//#include <IRremote.h>

//hook up serial comms object to pins
SoftwareSerial serialOut(10, 11); // RX, TX

#define RC_CH1_INPUT  2
#define POT_PIN A2  //potmeter pin

const int CHANNEL_CENTER = 511;
const int CHANNEL_DEADCENTER = 10;
const int MIN_CHANNEL = 875;
const int MAX_CHANNEL = 2125;

//IRrecv irrecv(IR_RECV_PIN);
//decode_results results;

int deadCentreWidth = 2;
int potDeviation = 25;

int averagePotmeter;  //average value measured
//keep track of previous analog input values in configurable length array, more values more precies but slower
const int analogInputHistoryLength = 10;
int previousAnalogValues[analogInputHistoryLength];

int oldPosition;

const int minValueMeasuredForPot = 0;
const int maxValueMeasuredForPot = 1023;
int potMiddlePosition = 504; //492; //overrule door plaatsing van magneet maxValueMeasuredForPot / 2; //waarde als potmeter in het midden staat

const int potMaxPositionLeft = 350; //minimale stand van potmeter links
const int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

int maxPositionLeftFrontServo = servoMinPulse; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = servoMaxPulse; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = servoMinPulse; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = servoMaxPulse; //maximale uitslag naar rechts achterste servo

String characterValues = ""; //concatenated numbers read over IR

volatile int pulseChannel1;

volatile unsigned long timer_start;

volatile int last_interrupt_time; //calcSignal is the interrupt handler

void calcSignal() {
  //record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  last_interrupt_time = micros();
  //if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(RC_CH1_INPUT) == HIGH) {
    timer_start = micros();
  }
  //otherwise, the pin has gone LOW
  else {
    //only worry about this if the timer has actually started
    if (timer_start != 0) {
      //record the pulse time
      pulseChannel1 = ((volatile int)micros() - timer_start);
      //restart the timer
      timer_start = 0;
    }
  }
}


void setup() {
  //init timer value
  timer_start = 0;
   
  //read the input from the receiver
  pinMode(RC_CH1_INPUT, INPUT); //receiver channel 1
  attachInterrupt(digitalPinToInterrupt(RC_CH1_INPUT), calcSignal, CHANGE);

  //read the input from the potmeter
  pinMode(POT_PIN, INPUT); 

  // debug and needed for IR
  Serial.begin(9600);
  oldPosition = -1;

  //setup a number of default middle positions
  for (int i=0;i<analogInputHistoryLength;i++) {
    previousAnalogValues[i] =  potMiddlePosition;
  }

  serialOut.begin(57600);
}

void debugSettings(int potmeter, int receiver) {

  if ( abs(potmeter-oldPosition) > 3) {
    //write the calculated value to the trailer
    writeValueToTrailer(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));

    //Serial.write(27); Serial.print("[2J"); // clear screen command
    //Serial.write(27); Serial.print("[H"); // home cursor
  
    //Serial.println("============================");

    Serial.print("Chnl: "); // Print the value of 
    Serial.print(receiver);        // each c`hannel
    Serial.print("\t");
      
    Serial.print("Pot:  ");
    Serial.print(potmeter);
    Serial.print("\t");
      
    Serial.print("Servo:  ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    oldPosition = potmeter;
  }
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

int limitToMaxPositionsFromReceiver(int input) {
  //below limits the receiver input range to the pot range so we can we can subtract or add them together
  return map(input, MIN_CHANNEL, MAX_CHANNEL, minValueMeasuredForPot, maxValueMeasuredForPot);
}

int getSummedTotal() {
  int total = 0;

    for (int i=0;i<analogInputHistoryLength;i++) {
      total += previousAnalogValues[i];
      previousAnalogValues[i] = previousAnalogValues[i+1];
    }

  return total;
}

int mixPlateAndReceiverInput(int receiver, int potmeter) {
  //if receiver is centered we can just get out of here with just the potmeter
  if ( receiver == 0 || receiver < (CHANNEL_CENTER - CHANNEL_DEADCENTER) || receiver > (CHANNEL_CENTER + CHANNEL_DEADCENTER)) {
    //receiver is not centered, so do something with it
    if (receiver <= CHANNEL_CENTER) {
      // we need to subtract it from potmeter
      //Serial.print("L");
      return (potmeter - abs((CHANNEL_CENTER-receiver)));
    } else {
      //Serial.print("R");
      //we need to add it to potmeter
      return (potmeter + (receiver-CHANNEL_CENTER));
    }
  } else {
    //Serial.print("*");
    return potmeter;
  }
}

void writeValueToTrailer(int value) {
  //write STX
  serialOut.write(char(2));

  //write value
  serialOut.print(value);
  
  //write ETX
  serialOut.write(char(3));
}


void loop() {
  // reads the value of the potentiometer
  int analogPotmeterInput = analogRead(POT_PIN); 

  //translate the receiver input to the same range of the pot input
  int analogReceiverInput = limitToMaxPositionsFromReceiver(pulseChannel1);

  //mix the receiver and the pot together but only when the sticks are out of center
  analogPotmeterInput = mixPlateAndReceiverInput(analogReceiverInput,analogPotmeterInput);
  
  // limit the pot values to what we expect them to be max left and right
  analogPotmeterInput = limitToMaxPositionsOnPlate(analogPotmeterInput);  

  //storeLatestAnalogValue(analogPotmeterInput);
  
  // translate plate position to relative position between max left and right
  int positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, minValueMeasuredForPot, maxValueMeasuredForPot); 
  
  // translate the relative pot position to a servo position and update servo positions when necessary
  debugSettings(positionPotmeter, analogReceiverInput); 
  delay(250);                           
}

