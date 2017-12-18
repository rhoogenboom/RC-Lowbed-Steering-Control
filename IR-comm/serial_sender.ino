#include <SoftwareSerial.h>
#include<math.h>

SoftwareSerial mySerial(10, 11); // RX, TX

// Sender Information
unsigned char START_BYTE = 0x53; // ASCII "S"

unsigned char channel1up = 0;  // analog channel1
unsigned char channel1down = 0;
unsigned char ch1plus = 0;
unsigned char ch1min = 250;

unsigned char channel2up = 0;  // analog channel2
unsigned char channel2down = 0;
unsigned char ch2plus = 250;
unsigned char ch2min = 0;

unsigned char channel3up = 0;  // analog channel3
unsigned char channel3down = 0;
unsigned char ch3plus = 50;
unsigned char ch3min = 60;

int valuech1 = 0;
int valuech2 = 0;

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
}

void loop() {
  valuech1 = analogRead(4);
  valuech2 = analogRead(5);

  channel1up = map(valuech1, 0, 511, 0, 100);
  channel1up = constrain(channel1up, 0, 100);
  channel1down = map(valuech1, 512, 1023, 100, 0);
  channel1down = constrain(channel1down, 0, 100);

  channel2up = map(valuech2, 0, 500, 0, 100);
  channel2up = constrain(channel2up, 0, 100);
  channel2down = map(valuech2, 520, 1023, 100, 0);
  channel2down = constrain(channel2down, 0, 100);
  //channel3 = digitalRead(2);

  senddata();
  // We only need to send a packet every 50 ms. so arduino 2 can read everything correct.
  delay(50);

}

void senddata()
{
  // Important: Serial.write must be used, not print
  mySerial.write(START_BYTE);
  mySerial.write(channel1up);
  mySerial.write(channel1down);
  mySerial.write(ch1plus);
  mySerial.write(ch1min);
  mySerial.write(channel2up);
  mySerial.write(channel2down);
  mySerial.write(ch2plus);
  mySerial.write(ch2min);
  mySerial.write(channel3up);
  mySerial.write(channel3down);
  mySerial.write(ch3plus);
  mySerial.write(ch3min);
}

