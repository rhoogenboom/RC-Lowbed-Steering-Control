//setup code
bool setupActive = false; //defines whether we are in setup mode or not
int buttonState = 0; //state of setupbutton default is open

// EEPROM
const long EEPROM_Init = 0xA03; // Change this any time the EEPROM content changes

struct __eeprom_data { //  structure that maps all of the data we are storing in EEPROM
  long E_InitNum;      // Number that indicates if EEPROM values have ever been initialized
  int E_servoMinPulse;
  int E_minValueMeasuredForPot;
};

int potInput; //hold value of analoginput of hal sensor
int potMiddlePosition = 514; //overruled the middle position of maxValueMeasuredForPot divided by two 2 due to the magnetic center

//servo and steering variables
//Limit the steering servos to these maximum values for left and right
int servoMinPulse; //position should be stored in trailer EEPROM also
int servoMaxPulse; //position should be stored in trailer EEPROM also
//adjustable min and max range for the pot/hal sensor
int minValueMeasuredForPot;
int maxValueMeasuredForPot;

//NFR 
const byte NFR_CE  = 3;                      // NFR CE pin D3/pin7
const byte NFR_CSN = 4;                      // NFR CSN pin D4/pin1
const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63}; //transmission address for NFR module

/* NFR wiring
 * VCC - 3.3v!!!!
 * GND - GND
 * CSN / CE see above
 * Nano:
 * SCK - pin 13
 * MOSI - pin 11
 * MISO = pin 12
 */

//timer for transmission interval over NFR
unsigned int transmissionTimerID;
boolean TimeUp =  true;

boolean Startup =  true; //defines whether we are running the main loop for the first time

//receiver variables 
volatile int pulse_time;  
volatile unsigned long receiver_pulse_start;                     
             

