/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

        //receiver items and model inputs
        const byte ThrottleChannel_Pin =    21;                      // The Arduino pin connected to the throttle channel input - D21/pin43
        const byte SteeringChannel_Pin =    20;                      // The Arduino pin connected to the steering channel input - D20/pin44
        const byte Channel3_Pin        =    19;                      // The Arduino pin connected to the multichannel input - D19/pin45
        const byte MixSteeringChannel_Pin = 18;                      // The Arduino pin connected to the steering channel for mixing the rear axles - D18/pin46
        const byte PotMeter            =    A1;                      // The Arduino pin connected to the potentio-meter from the 5th wheel A1/pin96

        //OSL board items
        const byte GreenLED            =    A4;                      // The Arduino pin connected to the on-board Green LED A4/pin93
        const byte RedLED              =    A5;                      // The Arduino pin connected to the on-board Red LED A5/pin92 
        const byte SetupButton         =    A0;                      // The Arduino pin connected to the on-board push button A0/pin97 

        const byte NFR_CSN             =    4;                       // The Arduino pin connected to the NFR CSN pin D4/pin1
        const byte NFR_CE              =    3;                       // The Arduino pin connected to the NFR CE pin D3/pin7
        
        RF24 radio(NFR_CE, NFR_CSN); // NFR CE, CSN connections
        //NFR pin connections:
        //Arduino
        //Pin   Pin Name            Mapped Pin Name       Connected To  Device
        //1     PG5 ( OC0B )        Digital pin 4 (PWM)   CSN           NFR
        //7     PE5 ( OC3C/INT5 )   Digital pin 3 (PWM)   CE            NFR
        //11    GND GND GND NFR
        //20    PB1 ( SCK/PCINT1 )  Digital pin 52 (SCK)  SCK           NFR
        //21    PB2 ( MOSI/PCINT2 ) Digital pin 51 (MOSI) MOSI          NFR
        //22    PB3 ( MISO/PCINT3 ) Digital pin 50 (MISO) MISO          NFR
        const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63};    

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}
char text[32];

void loop() {
  sprintf(text, "milis=%d", millis());
  radio.write(&text, sizeof(text));
  Serial.println("Written text");
}
