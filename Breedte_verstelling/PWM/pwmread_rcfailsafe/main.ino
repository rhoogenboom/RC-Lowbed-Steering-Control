#include <Servo.h>

int val;
int pos;
Servo servo;
#define RECEIVER_OUT (9)
#define POTPIN (3)

void setup() {
    setup_pwmRead();                      
    Serial.begin(9600);

    servo.attach(RECEIVER_OUT);
}

void loop() {  
    
    // Print RC receiver frame length and frame rate
 
    if (PWM_read(1)){                                      // if a new pulse is detected on channel 1
      Serial.print(PWM_period(),0);Serial.print("uS ");     
      Serial.print(PWM_freq());Serial.println("Hz");
    }

    val = analogRead(POTPIN);
    pos = map(val, 1, 1024, 1, 180);
    Serial.println("Val: " + String(val) + ", pos: " + String(pos));
    delay(100);
    servo.write(pos);              // tell servo to go to position in variable 'pos'
}
