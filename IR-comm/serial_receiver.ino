#include <SoftwareSerial.h>
#include <Servo.h>
#include <Math.h>

SoftwareSerial mySerial(8, 9); // RX, TX

Servo ch1;
Servo ch2;
Servo ch3;

// Sender Information
unsigned char START_BYTE = 0x53; // ASCII "S"

unsigned char channel1up = 0;
unsigned char channel1down = 0;
unsigned char ch1plus = 0;
unsigned char ch1min = 0;

unsigned char channel2up = 0;
unsigned char channel2down = 0;
unsigned char ch2plus = 0;
unsigned char ch2min = 0;

unsigned char channel3up = 0;
unsigned char channel3down = 0;
unsigned char ch3plus = 0;
unsigned char ch3min = 0;

// Sync Byte flag
boolean syncByteFound = 0;

int calculatedch1up = 0;
int calculatedch1down = 0;

int calculatedch2up = 0;
int calculatedch2down = 0;

int calculatedch3up = 0;
int calculatedch3down = 0;


void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);

  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
}

void loop() {

  getData();



}

void writeChannels()
{
  calculatedch1up = map(channel1up, 0, 100, 1000 - ch1min, 1500);
  calculatedch1down = map(channel1down, 100, 0, 1500, 2000 + ch1plus);

  calculatedch2up = map(channel2up, 0, 100, 1000 - ch2min, 1500);
  calculatedch2down = map(channel2down, 100, 0, 1500, 2000 + ch2plus);

  calculatedch3up = map(channel3up, 0, 100, 1000 - ch3min, 1500);
  calculatedch3down = map(channel3down, 100, 0, 1500, 2000 + ch3plus);

  calculatedch1up = constrain(calculatedch1up, 1000 - ch1min, 1500);
  calculatedch1down = constrain(calculatedch1down, 1500, 2000 + ch1plus);

  calculatedch2up = constrain(calculatedch2up, 1000 - ch2min, 1500);
  calculatedch2down = constrain(calculatedch2down, 1500, 2000 + ch2plus);

  calculatedch3up = constrain(calculatedch3up, 1000 - ch3min, 1500);
  calculatedch3down = constrain(calculatedch3down, 1500, 2000 + ch3plus);

  if (calculatedch1up < 1500 && calculatedch1down == 1500)
  {
    ch1.writeMicroseconds(calculatedch1up);
  } else
  {
    ch1.writeMicroseconds(calculatedch1down);
  }

  if (calculatedch2up < 1500 && calculatedch2down == 1500)
  {
    ch2.writeMicroseconds(calculatedch2up);
  } else
  {
    ch2.writeMicroseconds(calculatedch2down);
  }

  if (calculatedch3up < 1500 && calculatedch3down == 1500)
  {
    ch3.writeMicroseconds(calculatedch3up);
  } else
  {
    ch3.writeMicroseconds(calculatedch3down);
  }

}

void getData()
{
  unsigned char rxByte = 0;
  unsigned char calculatedChecksum = 0;

  // Check to see if there's something to read
  if (mySerial.available() > 0 ) {

    // If we're waiting for a new packet, check for the sync byte
    if (syncByteFound == 0) {
      rxByte = mySerial.read();
      if (rxByte == 0x53)
        writeChannels();
      syncByteFound = 1;
    }

    // If we've found our sync byte, check for expected number of bytes
    if (mySerial.available() > 8) {
      channel1up = mySerial.read();
      channel1down = mySerial.read();
      ch1plus = mySerial.read();
      ch1min = mySerial.read();

      channel2up = mySerial.read();
      channel2down = mySerial.read();
      ch2plus = mySerial.read();
      ch2min = mySerial.read();

      channel3up = mySerial.read();
      channel3down = mySerial.read();
      ch3plus = mySerial.read();
      ch3min = mySerial.read();

      // Print out our serial information to debug
      Serial.print("["); Serial.print("Start"); Serial.println("]");
      Serial.print("channel1up ms = "); Serial.print("["); Serial.print(calculatedch1up); Serial.println("]");
      Serial.print("channel1down ms = "); Serial.print("["); Serial.print(calculatedch1down); Serial.println("]");

      Serial.print("channel2up ms = "); Serial.print("["); Serial.print(calculatedch2up); Serial.println("]");
      Serial.print("channel2down ms = "); Serial.print("["); Serial.print(calculatedch2down); Serial.println("]");

      Serial.print("channel3up ms = "); Serial.print("["); Serial.print(calculatedch3up); Serial.println("]");
      Serial.print("channel3down ms = "); Serial.print("["); Serial.print(calculatedch3down); Serial.println("]");

      syncByteFound = 0;
    }
  }
}

