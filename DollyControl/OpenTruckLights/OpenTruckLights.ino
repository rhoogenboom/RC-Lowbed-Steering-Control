#include "AA_UserConfig.h"
#include "Defines.h"
#include "Variables.h"
#include <OSL_SimpleTimer.h>

#include <PinChangeInterrupt.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Timer object for transmission of signal to trailer
OSL_SimpleTimer timer;                 
       
// WIFI
RF24 radio(NFR_CE, NFR_CSN); // NFR CE, CSN connections
//NFR pin connections:
//Pin   Pin Name            Mapped Pin Name       Connected To  Device
//1     PG5 ( OC0B )        Digital pin 4 (PWM)   CSN           NFR
//7     PE5 ( OC3C/INT5 )   Digital pin 3 (PWM)   CE            NFR
//11    GND GND GND NFR
//20    PB1 ( SCK/PCINT1 )  Digital pin 52 (SCK)  SCK           NFR
//21    PB2 ( MOSI/PCINT2 ) Digital pin 51 (MOSI) MOSI          NFR
//22    PB3 ( MISO/PCINT3 ) Digital pin 50 (MISO) MISO          NFR

//controller structure for NFR transmission
typedef struct
{
  uint8_t state;
} OSLLight;

typedef struct
{
  bool updateEeprom;
  uint16_t controller3;
  OSLLight lights[12];
} OSLLightPacket;

volatile OSLLightPacket packet;

void setup()
{
  Serial.begin(BaudRate);  
    
  //Initialize_EEPROM();

  // initialize setup button
  buttonState = 0;
  pinMode(SETUP_BUTTON, INPUT);
  // set one pin to high to provide 5v for the magnet and servo pot meter and to read the setup button
  pinMode(POWER_5V, OUTPUT);
  digitalWrite(POWER_5V, HIGH);

  long Temp;
  // Get EEPROM Initialization value
  Serial.println("eeprom initial read");
  eeprom_read(Temp, E_InitNum);

  // If EEPROM has never been initialized before, do so now
  if (Temp == EEPROM_Init)
  {
    Serial.println("eeprom not initialized");
    Initialize_EEPROM();
  }
  else
  {
    Serial.println("load eeprom");
    Load_EEPROM();         
  }


  if (INTERUPT_IO) { 
    // Hook up interrupt handler functions for when data comes in
    attachInterrupt(digitalPinToInterrupt(MixSteeringChannel_Pin), calcMixSteeringChannel, CHANGE);
  } else {
      pinMode(MixSteeringChannel_Pin, INPUT_PULLUP);
  }
       
  pinMode(PotMeter_Pin, INPUT); 

  //setup default controller values
  packet.controller3 = 0;
  packet.updateEeprom = false;
  for (int i=0; i<NumLights; i++)
  {
      packet.lights[i].state = 0;                 
  }

  // check setup pin
  buttonState = digitalRead(SETUP_BUTTON);
  Serial.print(F("Setup button pressed: "));
  Serial.println(buttonState);
  // switch to setup mode (or light mode)
  if (buttonState == HIGH) {
    Serial.println("Setup button pressed, setup mode active");

    SetupMode();
    ReadPotMeters();
    setupActive = true;
  }
  else {
    Serial.println("Normal operating mode active");
    LightMode();
  }

  //WIFI
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR
  radio.setDataRate(RF24_250KBPS); //RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS
  radio.openWritingPipe(address);
  radio.stopListening();
}


// ====================================================================================================================================================>
//  MAIN LOOP
// ====================================================================================================================================================>

void loop()
{
  //RUN ONCE
  if (Startup)
  {       
      transmissionTimerID = timer.setInterval(transmissionInterval, transmitControllerInfo);
      Startup = false;           
  }

  //ETERNAL LOOP

  timer.run();

  GetRxCommands();

  // if we are in setup mode, read the setup button
  if (setupActive == true) {
    // check setup pin
    buttonState = digitalRead(SETUP_BUTTON);

    // if the button is pressed, we want to read new pot values and store them in EEPROM
    if (buttonState == HIGH) {
      Serial.println("Setup button pressed, update mode");
      ReadPotMeters();
 
      Serial.println("Write updated params to eeprom");
      Write_EEPROM();      

      //send servo EEPROM value over to trailer
      packet.updateEeprom = true;
    }
  }

  //set magnet LEDs only when we are in setup mode
  if (setupActive == true) {
    SetMagnetLEDs(potInput);
  }
  
} 

