#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  1

#define RC_CH1  0

#define RC_CH1_INPUT  A0

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { calc_input(RC_CH1, RC_CH1_INPUT); }

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_INPUT, INPUT);


  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
}

void loop() {
  rc_read_values();

  Serial.write(27); Serial.print("[2J"); // clear screen command
  Serial.write(27); Serial.print("[H"); // home cursor
  
  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.println("\t");
  delay(200);
}
