#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

bool startFound = false;
bool endFound = false;
String result = "";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  
}

void loop() { // run over and over
  if (mySerial.available()) {
    //read one byte
    unsigned char input = mySerial.read();
    switch (input) {
      case 0x02:
        startFound = true;
        endFound = false;       
        break;
     case 0x03:
        startFound = false;
        endFound = true;
        break;
     default:
        result = result + char(input);
    }

    if (endFound) {
      Serial.println(result);
      endFound = false;
      result = "";  
    }
  }
}
