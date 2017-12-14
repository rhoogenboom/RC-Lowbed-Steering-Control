#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define CHANNEL_1_PIN 2

int pulse_time;

volatile unsigned long timer_start;

volatile int last_interrupt_time; //calcSignal is the interrupt handler

void calcSignal() {
  //record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  last_interrupt_time = micros();
  //if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(CHANNEL_1_PIN) == HIGH) {
    timer_start = micros();
  }
  //otherwise, the pin has gone LOW
  else {
    //only worry about this if the timer has actually started
    if (timer_start != 0)     {
      //record the pulse time
      pulse_time = ((volatile int)micros() - timer_start);
      //restart the timer
      timer_start = 0;
    }
  }
}

//this is all normal arduino stuff
void setup() {
  timer_start = 0;
  attachInterrupt(digitalPinToInterrupt(CHANNEL_1_PIN), calcSignal, CHANGE);
  Serial.begin(9600);
}

void loop() {
  Serial.println(pulse_time);
  delay(20);
}
