/*
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

  v1 - plate control with receiver overrule
  v2 - include timeout and working receiverless
  v3 - adding wireless receiver
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(3, 4); // CE, CSN
//TODO remove 10,11 for serial comms

#define RC_CH1_INPUT  46 //2 //receiver pin
#define POT_PIN A1  //potentiometer pin

//http://www.firewall.cx/cisco-technical-knowledgebase/cisco-wireless/828-cisco-wireless-wlan-keygen.html
const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63};   
const char *structure1 = "c1=%d;c2=%d;c3=%d;mode=%d";
const char *structure2 = "m1=%d;m2=%d;m3=%d;m4=%d";
const char *structure3 = "m5=%d;m6=%d;m7=%d;m8=%d";

const int CHANNEL_CENTER = 511;
const int CHANNEL_DEADCENTER = 10;
const int MIN_CHANNEL = 875;
const int MAX_CHANNEL = 2125;

int deadCentreWidth = 2;
int potDeviation = 25;

int previousPlatePosition;

const int minValueMeasuredForPot = 0;
const int maxValueMeasuredForPot = 1023;
int potMiddlePosition = 504; //overruled the middle position of maxValueMeasuredForPot divided by two 2 due to the magnetic center

const int potMaxPositionLeft = 350; //minimale range potmeter movement left
const int potMaxPositionRight = potMiddlePosition + (potMiddlePosition - potMaxPositionLeft); //maximal range potmeter movement right

const int servoMinPulse = 1200;
const int servoMaxPulse = 1800;

int maxPositionLeftFrontServo = servoMinPulse; //maximale uitslag naar links voorste servo
int maxPositionRightFrontServo = servoMaxPulse; //maximale uitslag naar rechts voorste servo

int maxPositionLeftRearServo = servoMinPulse; //maximale uitslag naar links achterste servo
int maxPositionRightRearServo = servoMaxPulse; //maximale uitslag naar rechts achterste servo

volatile int pulseChannel1;

volatile unsigned long timer_start;

//volatile int last_interrupt_time; //calcSignal is the interrupt handler

void calcSignal() {
  //record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  //last_interrupt_time = micros();
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
  previousPlatePosition = -1;

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void debugSettings(int potmeter, int receiver) {

  if ( abs(potmeter-previousPlatePosition) > 3) {
    //write the calculated value to the trailer
    writeValueToTrailer(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));

    //Serial.write(27); Serial.print("[2J"); // clear screen command
    //Serial.write(27); Serial.print("[H"); // home cursor
  
    Serial.print("Chnl: "); // Print the value of 
    Serial.print(receiver);        // each c`hannel
    Serial.print("\t");
      
    Serial.print("Pot:  ");
    Serial.print(potmeter);
    Serial.print("\t");
      
    Serial.print("Servo:  ");
    Serial.println(map(potmeter, 0, maxValueMeasuredForPot, maxPositionLeftFrontServo, maxPositionRightFrontServo));
    previousPlatePosition = potmeter;
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

  //check if input is within allowed range, if not, the receive might be disconnected so we just return the middle
  if (input == 0 || input < MIN_CHANNEL || input > MAX_CHANNEL) {
    return CHANNEL_CENTER;
  } else {
    //TODO fix so we can use full stick range but still link to plate range for movement
    
    //below limits the receiver input range to the pot range so we can we can subtract or add them together
    return map(input, MIN_CHANNEL, MAX_CHANNEL, potMaxPositionLeft, potMaxPositionRight);
  }
}

int mixPlateAndReceiverInput(int receiver, int potmeter) {
  //if receiver is centered we can just get out of here with just the potmeter
  if ( receiver == 0 || receiver < (CHANNEL_CENTER - CHANNEL_DEADCENTER) || receiver > (CHANNEL_CENTER + CHANNEL_DEADCENTER)) {
    //receiver is not centered, so do something with it
    if (receiver <= CHANNEL_CENTER) {
      // we need to subtract it from potmeter
      return (potmeter - abs((CHANNEL_CENTER-receiver)));
    } else {
      //we need to add it to potmeter
      return (potmeter + (receiver-CHANNEL_CENTER));
    }
  } else {
    return potmeter;
  }
}

void writeValueToTrailer(int value) {
  //write value
  char string[32];
  sprintf(string, structure1, 
    value, //channel1=%d;
    0,   //channel2=%d;
    0,   //channel3=%d;
    0   //drivemode=%d;
  );
  radio.write(&string, sizeof(string));
  
  sprintf(string, structure2, 
    0,   //multi1=%d;
    0,   //multi2=%d;
    0,   //multi3=%d;
    0    //multi4=%d;
    );
  radio.write(&string, sizeof(string));
    
  sprintf(string, structure3, 
    0,   //multi5=%d;
    0,   //multi6=%d;
    0,   //multi7=%d;
    0   //multi8=%d;
    );
  radio.write(&string, sizeof(string));
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

  // translate plate position to relative position between max left and right
  int positionPotmeter = map(analogPotmeterInput, potMaxPositionLeft, potMaxPositionRight, minValueMeasuredForPot, maxValueMeasuredForPot); 
  
  // translate the relative pot position to a servo position and update servo positions when necessary
  debugSettings(positionPotmeter, analogReceiverInput); 
  delay(250);                           
}

