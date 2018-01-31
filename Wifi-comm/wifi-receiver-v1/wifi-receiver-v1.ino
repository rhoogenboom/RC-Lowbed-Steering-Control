/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "OSLController.h"


RF24 radio(3, 4); // CE, CSN

OSLController controller;

//replace easy to guess default address "00001";
//http://www.firewall.cx/cisco-technical-knowledgebase/cisco-wireless/828-cisco-wireless-wlan-keygen.html
const byte address[6] = {0x66,0x68,0x7b,0x4a,0x63};   

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
//  char input[32] = {'c','1','=','1','2','0','3',';','c','2','=','1','5','0','0',';','c','3','=','2','3','0','4',';','m','o','d','e','=','5',';'};
//  Serial.println(controller.processSettings(input));
//  char input2[32] = {'m','1','=','1',';','m','2','=','1',';','m','3','=','0',';','m','4','=','0',';'};
//  Serial.println(controller.processSettings(input2));
//  char input3[32] = {'m','5','=','0',';','m','6','=','0',';','m','7','=','0',';','m','8','=','0',';'};
//  Serial.println(controller.processSettings(input3));

  Serial.println(controller.printDebugInfo());
}
int teller = 0;

void loop() {
//  if (radio.available()) {
//    char text[32] = "";
//    radio.read(&text, sizeof(text));
//    Serial.println(text);
//  }
//  teller++;
//  if ((teller % 1000) == 0) {
//    Serial.println(millis());
//    teller = 0;
//  }
//  delay(5); 

  receiveControllerInfo();

}
