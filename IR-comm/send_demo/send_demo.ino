//code belongs to a YouTube tutorial on YouTube changel LogMaker360
//https://www.youtube.com/watch?v=uL3eWaehEQI
//code writen by Moz for Youtube changel LogMaker360
#include <IRremote.h>
#include <IRremoteInt.h>


IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  if (Serial.read() != -1) {
    for (int i = 0; i < 3; i++) {
      irsend.sendRC5("a", 12); // Sony TV power code
      delay(100);
    }
  }
}   
