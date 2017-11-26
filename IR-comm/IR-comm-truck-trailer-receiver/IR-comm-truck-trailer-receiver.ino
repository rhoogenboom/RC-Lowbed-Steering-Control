#include <IRremote.h>
#include <IRremoteInt.h>

/*
 *  v1 - initial version
 * 
 * Wiring :
 * S      -> pin 11
 * Middle -> 5v 
 * -      -> GND
 * 
 * com6
 * 
 */

#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {

    Serial.print(char(results.value));
    irrecv.resume(); // Receive the next value    
  }

}
