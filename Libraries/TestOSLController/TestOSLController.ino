#include "OSLController.h"

OSLController controller;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

//  controller.packet.driveMode = 1;
  controller.packet.controller1 = 1500;
  Serial.println(sizeof(controller.packet));
}

void loop() {
  // put your main code here, to run repeatedly:

}
