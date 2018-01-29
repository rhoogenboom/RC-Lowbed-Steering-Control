//assume that pin 32 is receiving PWM input
#define CHANNEL_1_PIN 21
#define CHANNEL_2_PIN 20
#define CHANNEL_3_PIN 19
#define CHANNEL_4_PIN 18

volatile unsigned long timer_start[4];
volatile int pulse_time[4]; //calcSignal is the interrupt handler 



void calcSignal(int channel, int pinNumber) 
{
    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter 
//    last_interrupt_time = micros(); 
    //if the pin has gone HIGH, record the microseconds since the Arduino started up 
    if(digitalRead(pinNumber) == HIGH) 
    { 
        timer_start[channel] = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(timer_start[channel] != 0)
        { 
            //record the pulse time
            pulse_time[channel] = ((volatile int)micros() - timer_start[channel]);
            //restart the timer
            timer_start[channel] = 0;
        }
    } 
} 


void calc_ch1() { calcSignal(0, CHANNEL_1_PIN); }
void calc_ch2() { calcSignal(1, CHANNEL_2_PIN); }
void calc_ch3() { calcSignal(2, CHANNEL_3_PIN); }
void calc_ch4() { calcSignal(3, CHANNEL_4_PIN); }
 
//this is all normal arduino stuff 
void setup() 
{
    timer_start[0] = 0; 
    timer_start[1] = 0; 
    timer_start[2] = 0; 
    timer_start[3] = 0; 
    attachInterrupt(digitalPinToInterrupt(CHANNEL_1_PIN), calc_ch1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CHANNEL_2_PIN), calc_ch2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CHANNEL_3_PIN), calc_ch3, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CHANNEL_4_PIN), calc_ch4, CHANGE);
   
    Serial.begin(9600);
} 
 
void loop()
{
    //if (pulse_time != 0) {
      Serial.print(pulse_time[0]);
      Serial.print("                    ");
      Serial.print(pulse_time[1]);
      Serial.print("                    ");
      Serial.print(pulse_time[2]);
      Serial.print("                    ");
      Serial.println(pulse_time[3]);
    //}
    delay(20);
}
