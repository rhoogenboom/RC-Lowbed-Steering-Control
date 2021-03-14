#include <Servo.h>

#define RECEIVER_OUT (9)
#define POTPIN (3)
#define ESC_PIN (3)

Servo servo;
int val;
int pos;
int val2;

void setup() {
  // put your setup code here, to run once:
  //initialize boot vars
  Serial.begin(9600);
  Serial.println(F("Setting up"));

  //initialize pins

//  pinMode(ESC_PIN, INPUT);
  pinMode(POTPIN, INPUT);

  //servo.attach(RECEIVER_OUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  val = analogRead(POTPIN);
  //val2 = digitalRead(ESC_PIN);
  pos = map(val, 1, 1024, 1, 180);
  Serial.println("Val: " + String(val) + ", pos: " + String(pos));
  delay(500);
  //servo.write(pos);              // tell servo to go to position in variable 'pos'

}
