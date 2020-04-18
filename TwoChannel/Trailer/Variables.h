#include <Servo.h>

// EEPROM
const long EEPROM_Init = 0xA03; // Change this any time the EEPROM content changes

struct __eeprom_data { //  structure that maps all of the data we are storing in EEPROM
  long E_InitNum;      // Number that indicates if EEPROM values have ever been initialized
  int E_servoMinPulse;
  int E_minValueMeasuredForPot;
};

//NFR 
const byte NFR_CE  = 3;                      // NFR CE pin D3/pin7
const byte NFR_CSN = 4;                      // NFR CSN pin D4/pin1
const byte TRUCK_ADDRESS[6] = {0x66,0x68,0x7b,0x4a,0x63}; //transmission address for NFR module
const byte TRAILER_ADDRESS[6] = {0x66,0x68,0x7b,0x4a,0x64}; //transmission address for NFR module

////TIMER
//unsigned int transmissionTimerID;
//boolean TimeUp =  true;

//MAIN LOOP GENERIC
boolean Startup =  true; //defines whether we are running the main loop for the first time

//servo and steering variables
// front servo (3 axles)
Servo servoFront;   
// rear servo (2 axles)
Servo servoRear;    
//Limit the steering servos to these maximum values for left and right
int servoMinPulse; //position should be stored in trailer EEPROM also
int servoMaxPulse; //position should be stored in trailer EEPROM also
//adjustable min and max range for the pot/hal sensor
int potMiddlePosition = 514; 
int minValueMeasuredForPot = 350;
int maxValueMeasuredForPot = potMiddlePosition + (potMiddlePosition - minValueMeasuredForPot);

