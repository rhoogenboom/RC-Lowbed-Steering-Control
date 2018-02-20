#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OSL_SimpleTimer.h>

RF24 radio(3, 4); // CE, CSN
const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63}; 

OSL_SimpleTimer timer; 
#define transmissionInterval 500  //ms transmission interval
unsigned int transmissionTimerID;
bool startUp;

typedef struct
{
  uint8_t state;
//  uint8_t function;
} OSLLight;

typedef struct
{
  uint16_t controller1;
//  uint16_t controller2;
//  uint16_t controller3;
  OSLLight lights[12];
} OSLLightPacket;

volatile OSLLightPacket packet;

bool useStructure = true;
  
void setup() {
  startUp = true;
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR
  radio.setDataRate(RF24_2MBPS); //RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS
  radio.openWritingPipe(address);
  radio.stopListening();


  packet.controller1 = 1501;
//  packet.controller2 = 1050;
//  packet.controller3 = 2100;
  packet.lights[0].state = 1;
  packet.lights[2].state = 0;


}

void transmitControllerInfo() {
  if (!useStructure) {
    char text[32];
    sprintf(text, "It is now: %d ms", millis());
    radio.write(&text, sizeof(text));
  }
  else {
    packet.controller1 = millis();
    radio.write(&packet, sizeof(packet));
  }
}


void loop() {
  if (startUp) {
    transmissionTimerID = timer.setInterval(transmissionInterval, transmitControllerInfo);
    startUp = false;
  }
  timer.run();  
}
