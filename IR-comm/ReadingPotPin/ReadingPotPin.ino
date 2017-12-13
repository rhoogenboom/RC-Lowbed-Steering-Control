int POT_PIN = A2; //potmeter pin

void setup() {
  // put your setup code here, to run once:
  pinMode(POT_PIN, INPUT); 
    Serial.begin(9600);
    
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(analogRead(POT_PIN));

  delay(100);    
}
