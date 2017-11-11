/*
   v1 - eerste versie met 2 servo en variabele instelling



*/

#include <Servo.h>

Servo servoVoor; // voorste servo (3 stuurassen)
Servo servoAchter; // achterste servo (2 stuurassen)

int doodpuntBreedte = 30;

int potpin = 0; //potmeter pin
int uitslagPotmeter;    //huidige uitslag van potmeter vertaald van uitslag op plaat naar volledige bereik 
int analogPotmeter;  //uitlezing van potmeter op pin

int debugUitslag;
int printNow;


int PotuitslagMidden = 1023 / 2; //waarde als potmeter in het midden staat

int PotuitslagLinks = 313;//0; //minimale stand van potmeter links
int PotuitslagRechts = 713;//PotuitslagMidden + (PotuitslagMidden - PotuitslagLinks); //maximale stand van potmeter rechts

int uitslagLinksServoVoor = 40; //maximale uitslag naar links voorste servo
int uitslagRechtsServerVoor = 140; //maximale uitslag naar rechts voorste servo

int uitslagLinksServoAchter = 60; //maximale uitslag naar links achterste servo
int uitslagRechtsServerAchter = 120; //maximale uitslag naar rechts achterste servo

void setup() {
  //debug
  Serial.begin(9600);
  debugUitslag = 0;

  servoVoor.attach(9);  // servo voor
  servoAchter.attach(10); // servo achter
}

void debugSettings(int analogPotmeter, int uitslagPotmeter) {

  printNow++;
  if ((debugUitslag != analogPotmeter) && (printNow % 15 == 0)) {
    Serial.println("============================");
    Serial.println("");
    Serial.print("Potmeter analog: ");
    Serial.println(analogPotmeter);
    Serial.print("Potmeter translated: ");
    Serial.println(uitslagPotmeter);

    Serial.println("Potmeter analog: ");
    Serial.print("Servo voor:  ");
    Serial.println(map(analogPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoVoor, uitslagRechtsServerVoor));
    Serial.print("Servo achter: ");
    Serial.println(map(analogPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoAchter, uitslagRechtsServerAchter));

    Serial.println("Potmeter translated: ");
    Serial.print("Servo voor:  ");
    Serial.println(map(uitslagPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoVoor, uitslagRechtsServerVoor));
    Serial.print("Servo achter: ");
    Serial.println(map(uitslagPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoAchter, uitslagRechtsServerAchter));

    debugUitslag = analogPotmeter;
    printNow = 0;
  }

}


void loop() {

  analogPotmeter = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)

  if ((analogPotmeter >= (PotuitslagMidden - doodpuntBreedte)) && (analogPotmeter <= (PotuitslagMidden + doodpuntBreedte))) {
    analogPotmeter = PotuitslagMidden;  
  }
 
  if (analogPotmeter < PotuitslagLinks) {
    analogPotmeter = PotuitslagLinks;
  }
  if (analogPotmeter > PotuitslagRechts) {
    analogPotmeter = PotuitslagRechts;
  }

  uitslagPotmeter = map(analogPotmeter, 0, 1023, PotuitslagLinks, PotuitslagRechts);  
//uitslagPotmeter = map(analogPotmeter, PotuitslagLinks, PotuitslagRechts, 0, 1023);  

 // debugSettings(analogPotmeter, uitslagPotmeter);

  servoVoor.write(map(analogPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoVoor, uitslagRechtsServerVoor));
  servoAchter.write(map(analogPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoAchter, uitslagRechtsServerAchter));
//  servoVoor.write(map(uitslagPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoVoor, uitslagRechtsServerVoor));
//  servoAchter.write(map(uitslagPotmeter, PotuitslagLinks, PotuitslagRechts, uitslagLinksServoAchter, uitslagRechtsServerAchter));

  delay(15);                           // waits for the servo to get there
}

