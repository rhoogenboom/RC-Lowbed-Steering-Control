void transmitControllerInfo() {
  // Transmits the OSL controller info over wifi
  // RHO TODO: only transmit dirty data? consumes less bandwidth, but can OSL deal with it on the other end?  

  //write controller info
  char text[32];
  controller.getControllerInfo().toCharArray(text, 32);
  radio.write(&text, sizeof(text));
 
  //write light 0-4
  controller.getLightsInfo1_4().toCharArray(text, 32);
  radio.write(&text, sizeof(text));

  //write light 5-7
  controller.getLightsInfo5_8().toCharArray(text, 32);
  radio.write(&text, sizeof(text));
}

void receiveControllerInfo() {
  //return value of what controller processed from wifi input:
  // -1 = false/disconnected/nothing
  //  0 = controller information
  //  1 = lights 1-4
  //  2 = lights 5-8
  //  3 = lights 9-12
  int whatToProcess = -1;  
  if (radio.available()) {
    char wifiBuffer[32] = "";
    radio.read(&wifiBuffer, sizeof(wifiBuffer));
    whatToProcess = controller.processSettings(wifiBuffer);

  }
//    delay(15);
    
  switch (whatToProcess) {
    case 0 : 
      Serial.print(controller.getControllerInfo()); Serial.println("   ");
      
//      //deal with axl update
//      //only update when there is a valid position
//      if (controller.getController3() != -1) {
//        // only update if significant change
//        if (abs(previousControllerPosition - controller.getController3()) > 3) {
//          previousControllerPosition = controller.getController3();
//          updateServoPositions(limitToMaxPositionsServo(controller.getController3()));
//        }
//      }       
      break;
    case 1 :
    case 2 :
    case 3 :
      //deal with lights
      break;
  }
}

