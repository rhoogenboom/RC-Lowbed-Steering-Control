/*
 * v1 - initial version
 * 
 * Wiring:
 * IR led  +  100 ohms resistor  between pin 3 arduino and GND
 * 
 * 
 * 
 */

#include <IRremote.h>
IRsend irsend;

void setup() {
  Serial.begin(9600);
  int i=0;
}

void loop() {
  String send = "Hi Instructables ! ";  

  Serial.println(send);

  //send the characters over the IR line
  for (int i = 0; i < send.length(); i++){

    irsend.sendRC5(send.charAt(i), 12); 
    delay(20);
  }

  irsend.sendRC5(13, 12);  //send a RETURN
  irsend.sendRC5(10, 12); 

  delay(15);
}
