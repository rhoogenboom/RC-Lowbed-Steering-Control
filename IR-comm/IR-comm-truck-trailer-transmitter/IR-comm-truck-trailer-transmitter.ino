/*
 * v1 - initial version
 * 
 * Wiring:
 * IR led  +  100 ohms resistor  between pin 3 arduino and GND
 * 
 * com4
 * 
 */

#include <IRremote.h>
IRsend irsend;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); 
}


void sendValue(int val) {
  //character buffer to transmit
  char buffer[50];
  
  //need a string to figure out the length
  String value;
  value = (String)val;

  //transmit buffer one character at a time
  for (int i = 0; i < value.length(); i++) {
    irsend.sendRC5(value.charAt(i), 12); // transmit character, 12 bits
    delay(20);
  }
 
  irsend.sendRC5(13, 12);  //ETX
  irsend.sendRC5(10, 12);  //ETX
  delay(20);
}

int minvalue = 385;
int maxvalue = 615;
int delaytime = 100;

void loop() {
  int potValue;

  for (int i=minvalue; i <= maxvalue; i+=10) {
    sendValue(i);
    delay(delaytime);
    Serial.println(i);

  }
  for (int i=maxvalue; i >= minvalue; i-=10) {
    sendValue(i);
    delay(delaytime);
    Serial.println(i);
    
  }
}
