void updateServoPositions(int relativePosition) {
  servoFront.write(relativePosition);
  servoRear.write(relativePosition);
}

int limitToMaxPositionsServo(int input) {
  //below limits the range which we consider the full movement between truck and trailer angle, either left of right
  
  //if input is smaller than the minimum we would accept, update to minimum
  if (input < servoMinPulse) {
    input = servoMinPulse;
  }
  //if input is larger than the maximum we would accept, update to maximum
  if (input > servoMaxPulse) {
    input = servoMaxPulse;
  }
  return input;
}

//translate the potmeter input to the same values as the receiver input so they can be easily subtracted from each other.
int convertPotmeterToReceiverValues(int input) {
  return map(input, minValueMeasuredForPot, maxValueMeasuredForPot, MIN_CHANNEL, MAX_CHANNEL);
}

//check if input is within the allowed min max range and limit it
int limitToMaxPositionsFromPotmeter(int input) {
  if (input <= minValueMeasuredForPot) {
    return minValueMeasuredForPot;
  } 
  else {
    if (input > maxValueMeasuredForPot) {
      return maxValueMeasuredForPot;
    }
    else {
        return input;
    }
  }
}


int CalculateRearAxlePosition() {
  // reads the value of the receiver stick position
  int receiver = trailerPacket.receiver;
    
  // reads the value of the potentiometer
  int potInput = trailerPacket.dolly; //input: 0-1024 

  int limitedPotmeterInput = limitToMaxPositionsFromPotmeter(potInput); //input range: 0-1023 limit to --> min and max for potmeter
  
  int analogPotmeterInput = convertPotmeterToReceiverValues(limitedPotmeterInput); //input range: 0-1023 limit: 875-2125

   //validat input from receiver
  int analogReceiverInput = limitToMaxPositionsFromReceiver(receiver); //input range: 875-2125 limit: 875-2125
  
  //mix the receiver and the pot together but only when the sticks are out of center
  int resultingSteering = mixPlateAndReceiverInput(analogReceiverInput, analogPotmeterInput); //input: 875-2125 / 875-2125; 

  if (DEBUG_INPUT) {
    Serial.print(F("Min pot value - Max pot value : "));
    Serial.print(minValueMeasuredForPot); Serial.print(F(" - ")); Serial.println(maxValueMeasuredForPot);
    
    Serial.print(F("Potmeter Input"));
    Serial.print(F("   "));
    Serial.print(F("Potmeter Limit"));
    Serial.print(F("   "));
    Serial.print(F("Potmeter Converted"));
    Serial.print(F("   "));
    Serial.print(F("Steering Input"));
    Serial.print(F("   "));
    Serial.print(F("Steering Result"));
    Serial.print(F("   "));
    Serial.println(F("Trailer steering"));
    Serial.print(potInput); 
    Serial.print(F("             "));
    Serial.print(limitedPotmeterInput); 
    Serial.print(F("                  "));
    Serial.print(analogPotmeterInput); 
    Serial.print(F("                  "));
    Serial.print(analogReceiverInput); 
    Serial.print(F("                  "));
    Serial.print(resultingSteering); 
    Serial.print(F("               "));
    Serial.println(map(resultingSteering, MIN_CHANNEL, MAX_CHANNEL, servoMinPulse, servoMaxPulse)); 
    delay(1000);
  }
  return map(resultingSteering, MIN_CHANNEL, MAX_CHANNEL, servoMinPulse, servoMaxPulse);
}

