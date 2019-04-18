void LightMode() {
//  pinMode(POWER_5V, OUTPUT);
//  digitalWrite(POWER_5V, LOW);
//  pinMode(MAGNET_POT, OUTPUT);
//  pinMode(SERVO_POT, OUTPUT);
}

void SetupMode() {
  pinMode(MAGNET_POT, INPUT);
  pinMode(SERVO_POT, INPUT);
  pinMode(MAGNET_LEFT_LED_RED, OUTPUT);
  pinMode(MAGNET_RIGHT_LED_GREEN, OUTPUT);
}

void ReadPotMeters() {
  //read magnet pot 
  int potInput = analogRead(MAGNET_POT); //input: 0-1024 
  Serial.print("Magnet input: "); Serial.println(potInput);
  
  //adjust magnet min and max
  minValueMeasuredForPot = potInput;
  maxValueMeasuredForPot = potMiddlePosition + (potMiddlePosition - minValueMeasuredForPot); //maximal range potmeter movement right

  // read servo pot
  potInput = analogRead(SERVO_POT); //input: 0-1024 
  Serial.print("Servo input: "); Serial.println(potInput);

  //adjust servo min and max
  servoMinPulse = map(potInput, 0, 1024, SERVO_MIN_POSITION, SERVO_MIDDLE_POSITION);
  servoMaxPulse = SERVO_MIDDLE_POSITION + (SERVO_MIDDLE_POSITION - servoMinPulse);
  Serial.print("Servo min and max pulse: "); Serial.print(servoMinPulse); Serial.print(" - "); Serial.println(servoMaxPulse);
}

void SetMagnetLEDs(int potInput) {
  int output = 0;
  if (potInput > HALL_CENTER) {
    //past middle to the right

    //set left full
    analogWrite(MAGNET_RIGHT_LED_GREEN, 0);

    //scale right down
    output = map(abs(potInput - HALL_CENTER), 0, HALL_CENTER, 0, 255);
    analogWrite(MAGNET_LEFT_LED_RED, output);
  }
  else {
    //before the middle to the left

    //set right full
    analogWrite(MAGNET_LEFT_LED_RED, 0);

    //scale down the left
    output = map(potInput, 0, HALL_CENTER, 255, 0);    
    analogWrite(MAGNET_RIGHT_LED_GREEN, output);
  }
  
}












