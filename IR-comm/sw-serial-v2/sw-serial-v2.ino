#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over

  for(int i=1;i<=100;i++) { 
    mySerial.print("And the next number: ");
    mySerial.println(i);
    delay(20);
  }

    mySerial.println();
    mySerial.println("***");
    mySerial.println();
}
