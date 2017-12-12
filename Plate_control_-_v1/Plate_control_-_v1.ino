/*
   v1 - place control with receiver overrule
*/

#include <EnableInterrupt.h>
//#include <IRremote.h>

#define RC_NUM_CHANNELS  1
#define RC_CH1  0
#define RC_CH1_INPUT  A0

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

#define POT_PIN A2  //potmeter pin

#define TRAILER_OUTPUT_PIN 3 //output to the trailer connector

int analogReceiverInput;

const int CHANNEL_CENTER = 511;
const int CHANNEL_DEADCENTER = 10;
const int MIN_CHANNEL = 875;
const int MAX_CHANNEL = 2125;

//IRrecv irrecv(IR_RECV_PIN);
//decode_results results;

int deadCentreWidth = 2;
int potDeviation = 25;

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
int potMiddlePosition = 504; //492; //overrule door plaatsing van magneet maxValueMeasuredForPot / 2; //waarde als potmeter in het midden staat

const int potMaxPositionLeft = 300; //minimale stand van potmeter links
const int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximale stand van potmeter rechts

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

int maxPositionLeftFrontServo = servoMinPulse; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = servoMaxPulse; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = servoMinPulse; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = servoMaxPulse; //maximale uitslag naar rechts achterste servo

String characterValues = ""; //concatenated numbers read over IR


void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { calc_input(RC_CH1, RC_CH1_INPUT); }


void setup() {
  //read the input from the receiver
  pinMode(RC_CH1_INPUT, INPUT); //receiver channel 1
  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);

  //read the input from the potmeter
  pinMode(POT_PIN, INPUT); 

  //send the calculated value to the trailer
  pinMode(TRAILER_OUTPUT_PIN, OUTPUT);

  // debug and needed for IR
  Serial.begin(9600);
  oldPosition = -1;

  //setup a number of default middle positions
  for (int i=0;i<analogInputHistoryLength;i++) {
    previousAnalogValues[i] =  potMiddlePosition;
  }
}

void debugSettings(int potmeter) {

  if ( abs(potmeter-oldPosition) > 3) {
    //Serial.write(27); Serial.print("[2J"); // clear screen command
    //Serial.write(27); Serial.print("[H"); // home cursor
  
    //Serial.println("============================");

    Serial.print("Chnl: "); // Print the value of 
    Serial.print(analogReceiverInput);        // each channel
    Serial.print("\t");
      
    Serial.print("Pot:  ");
    Serial.print(potmeter);
    Serial.print("\t");
      
    Serial.print("Servo:  ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    oldPosition = potmeter;
  }
}

void translatePosition(int analogPotmeter) {
    debugSettings(analogPotmeter);
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

void loop() {
  // reads the value of the potentiometer
  int analogPotmeterInput = analogRead(POT_PIN); 

  // reads the value from the receiver
  rc_read_values();

  //translate the receiver input to the same range of the pot input
  analogReceiverInput = limitToMaxPositionsFromReceiver(rc_values[RC_CH1]);

  //mix the receiver and the pot together but only when the sticks are out of center
  analogPotmeterInput = mixPlateAndReceiverInput(analogReceiverInput,analogPotmeterInput);
  
  // limit the pot values to what we expect them to be max left and right
  analogPotmeterInput = limitToMaxPositionsOnPlate(analogPotmeterInput);  

  //write the calculated value to the trailer
  analogWrite(TRAILER_OUTPUT_PIN , analogPotmeterInput);
  
  storeLatestAnalogValue(analogPotmeterInput);
  
  // translate plate position to relative position between max left and right
  positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, minValueMeasuredForPot, maxValueMeasuredForPot); 
  
  // translate the relative pot position to a servo position and update servo positions when necessary
  translatePosition(positionPotmeter); 
  delay(5);                           
}

