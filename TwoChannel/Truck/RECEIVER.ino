void GetMixedSteeringInput()
{
    //read the input from the RC receiver overruling the 5th wheel position
    if (INTERUPT_IO) {
      trailerPacket.receiver = pulse_time;
    } else {
      trailerPacket.receiver = pulseIn(RECEIVER_CHANNEL_STEERING_PIN, HIGH, SERVO_TIMEOUT);
    }
}  

