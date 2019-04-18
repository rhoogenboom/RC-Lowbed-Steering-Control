void GetRxCommands()
{
    GetMixedSteeringInput(); 
}

int GetMixedSteeringInput()
{
    int MixedTurnPulse;
    
    //read the input from the RC receiver overruling the 5th wheel position
    if (INTERUPT_IO) {
      MixedTurnPulse = pulse_time;
    } else {
      MixedTurnPulse = pulseIn(MixSteeringChannel_Pin, HIGH, ServoTimeout);
    }
    
    //update the controller object for sending the data over
    packet.controller3 = CalculateRearAxlePosition(MixedTurnPulse);  
}  


void calcChannel(int pinNumber) 
{
    //if the pin has gone HIGH, record the microseconds since the Arduino started up 
    if(digitalRead(pinNumber) == HIGH) 
    { 
        receiver_pulse_start = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(receiver_pulse_start != 0)
        { 
            //record the pulse time
            pulse_time = ((volatile int)micros() - receiver_pulse_start);
            //restart the timer
            receiver_pulse_start = 0;
        }
    } 
} 

void calcMixSteeringChannel() { calcChannel(MixSteeringChannel_Pin); }

