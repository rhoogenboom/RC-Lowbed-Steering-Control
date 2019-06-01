#include "Defines.h"
#include "Variables.h"
#include <OSL_SimpleTimer.h>

#include <PinChangeInterrupt.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//// Timer object for transmission of signal to trailer
//OSL_SimpleTimer timer;                 
       
// WIFI
RF24 radio(NFR_CE, NFR_CSN); // NFR CE, CSN connections

//controller structure for NFR transmission
typedef struct
{
  uint8_t state;
} OSLLight;

typedef struct
{
  bool updateEeprom;
  uint16_t receiver;
  uint16_t dolly;
  uint16_t trailer;
  OSLLight lights[12];
} OSLLightPacket;

volatile OSLLightPacket trailerPacket;

void setup() {
  Serial.begin(BAUDRATE);  
  
  //WIFI
  radio.begin();
  radio.setCRCLength( RF24_CRC_16 );
  radio.setAutoAck(true);
  radio.setPALevel(RF24_PA_HIGH); //RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR
  radio.setDataRate(RF24_250KBPS); //RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS
  radio.setRetries(0,5);  //0=250 - 15=4000

  radio.openReadingPipe(0, TRAILER_ADDRESS); //stuff written to the trailer
  radio.startListening();
}

void loop() {
  if (Startup)
  {       
      Startup = false;
  }

  receiveControllerInfo();
}
