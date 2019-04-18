void transmitControllerInfo() {
  PrepareToTransmit();
//  if (trailerPacket.updateEeprom == true) {
//    //send new values to trailer, overrule steering value with new servoMinPulse
//    trailerPacket.controller3 = servoMinPulse;
//    radio.write(&trailerPacket, sizeof(trailerPacket));  
//    trailerPacket.updateEeprom = false;
//    Serial.println(F("Sent updated eeprom value"));
//  }
//  else {
    radio.write(&trailerPacket, sizeof(trailerPacket));  
//  }

  if (DEBUG_OUTPUT) {
    Serial.print(F("Steering position"));
    Serial.print(F("   "));
    Serial.println(F(""));
    Serial.print(trailerPacket.trailer); 
    Serial.print(F("   "));
    Serial.println(F("")); 
  }  
}

void receivePlateInfo() {
  PrepareToReceive();
  if (radio.available()) {
    radio.read(&dollyPacket, sizeof(dollyPacket));
    trailerPacket.dolly = dollyPacket.dolly;
//    bool updateEeprom = dollyPacket.updateEeprom;
  }
  else {
 //   Serial.println(F("no radio available"));
  }
}

void PrepareToReceive() {
  radio.openReadingPipe(0, TRUCK_ADDRESS); //stuff written to the truck
  radio.startListening();
}

void PrepareToTransmit() {
  radio.stopListening();
  radio.openWritingPipe(TRAILER_ADDRESS); //stuff written to the trailer
}

