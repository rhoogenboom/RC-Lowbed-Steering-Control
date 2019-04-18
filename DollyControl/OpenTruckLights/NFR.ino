void transmitControllerInfo() {
  // Transmits the OSL controller info over wifi
  // RHO TODO: only transmit dirty data? consumes less bandwidth, but can OSL deal with it on the other end?  

  if (packet.updateEeprom == true) {
    //send new values to trailer, overrule steering value with new servoMinPulse
    packet.controller3 = servoMinPulse;
    radio.write(&packet, sizeof(packet));  
    packet.updateEeprom = false;
    Serial.println(F("Sent updated eeprom value"));
  }
  else {
    radio.write(&packet, sizeof(packet));  
  }

  if (DEBUG_OUTPUT) {
    Serial.print(F("Steering position"));
    Serial.print(F("   "));
    Serial.println(F(""));
    Serial.print(packet.controller3); 
    Serial.print(F("   "));
    Serial.println(F("")); 
  }  
}

