void GetPlateInput() {
    // reads the value of the potentiometer
  dollyPacket.dolly = analogRead(POTMETER_PIN); //input: 0-1024 
}

