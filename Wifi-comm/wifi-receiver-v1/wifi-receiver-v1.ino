#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
//const byte address[6] = "00001";
const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63};   

typedef struct
{
  uint8_t state;
  uint8_t type;
} OSLLight;

typedef struct
{
  uint16_t controller1;
//  uint16_t controller2;
//  uint16_t controller3;
  OSLLight lights[12];
} OSLLightPacket;

OSLLightPacket packet;

bool useStructure = true;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR
  radio.setDataRate(RF24_2MBPS); //RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS
  radio.openReadingPipe(0, address);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    if (!useStructure) {    
      char text[32] = "";    
      radio.read(&text, sizeof(text));
      Serial.println(text);
    } 
    else {
      radio.read(&packet, sizeof(packet));
      Serial.print("controller 1: ");    Serial.println(packet.controller1);
      Serial.print("Light 1: ");     Serial.println(packet.lights[0].state);
    }
  }
}
