void receiveControllerInfo() {
  if (radio.available()) {
    radio.read(&trailerPacket, sizeof(trailerPacket));
    int servoPosition = trailerPacket.trailer;
    bool updateEeprom = trailerPacket.updateEeprom;

    CalculateRearAxlePosition();
//    if (servoPosition != 0) {
//      if (updateEeprom == true) {
//        Serial.println(F("Received new eeprom value"));
//
//        //new value for trailer, overrule steering value with new servoMinPulse
//        servoMinPulse = servoPosition;
//        setDependantValues(servoMinPulse, minValueMeasuredForPot);
//        
//        //write new value to eeprom
//        Write_EEPROM();
//      }
//      updateServoPositions(limitToMaxPositionsServo(servoPosition));
//    } 
  //printPacketContents();  
  }
  else {
 //   Serial.println(F("no radio available"));
  }
}

void printPacketContents() {
  Serial.print(F("Dolly: ")); Serial.println(trailerPacket.dolly);
  Serial.print(F("Receiver: ")); Serial.println(trailerPacket.receiver);
  Serial.print(F("Trailer: ")); Serial.println(trailerPacket.trailer);
  Serial.print(F("Eeprom: ")); Serial.println(trailerPacket.updateEeprom);
}

