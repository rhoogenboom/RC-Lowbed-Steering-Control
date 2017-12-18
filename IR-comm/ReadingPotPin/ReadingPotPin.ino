#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

bool startFound = false;
bool endFound = false;
String result = "";
int oldValue = -1;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  
}

int readSerialInput() {
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
      //Serial.println(result);
      endFound = false;
      int r = result.toInt();
      result = ""; 
      return r;
    }
  }

  return -1;  
}

void loop() { // run over and over

  int pot = readSerialInput();
  if (pot != -1 && oldValue != pot) {
    Serial.print("new value:  ");
    Serial.println(pot);
    oldValue = pot;
  }

}
