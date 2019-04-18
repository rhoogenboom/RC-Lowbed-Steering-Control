void receiveControllerInfo() {
  if (radio.available()) {
    radio.read(&packet, sizeof(packet));
    int servoPosition = packet.controller3;
    bool updateEeprom = packet.updateEeprom;

    if (servoPosition != 0) {
      if (updateEeprom == true) {
        Serial.println(F("Received new eeprom value"));

        //new value for trailer, overrule steering value with new servoMinPulse
        servoMinPulse = servoPosition;
        setDependantValues(servoMinPulse, minValueMeasuredForPot);
        
        //write new value to eeprom
        Write_EEPROM();
      }

      updateServoPositions(limitToMaxPositionsServo(servoPosition));
    } 

  //SetLights();
  //printPacketContents();  
  }
  else {
 //   Serial.println(F("no radio available"));
  }
}

void printPacketContents() {
  Serial.print(F("Controller: ")); Serial.println(packet.controller3);
  Serial.print(F("Eeprom: ")); Serial.println(packet.updateEeprom);
  for (int i=0;i<12;i++) {
    Serial.print(F("Light:  ")); Serial.println(packet.lights[i].state);
  }
}

