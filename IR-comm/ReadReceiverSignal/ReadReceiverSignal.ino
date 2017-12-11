/*
receiver on:


*/
 
int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;

void setup() {

  pinMode(5, INPUT); //receiver channel 1
  pinMode(6, INPUT); //receiver channel 2

  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

  ch1 = pulseIn(5, HIGH, 25000); // Read the pulse width of 
  ch2 = pulseIn(6, HIGH, 25000); // each channel

  Serial.write(27); Serial.print("[2J"); // clear screen command
  Serial.write(27); Serial.print("[H"); // home cursor
  
  Serial.print("Channel 1:"); // Print the value of 
  Serial.println(ch1);        // each channel

  Serial.print("Channel 2:");
  Serial.println(ch2);

  delay(1000); // I put this here just to make the terminal 
              // window happier
}
