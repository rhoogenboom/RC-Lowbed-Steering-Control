// FUNCTIONS RELATED TO THE SIMPLE TIMER 


// ------------------------------------------------------------------------------------------------------------------------------------------------>  
// BLINKLIGHTS - This flip/flops our blinker variable
// ------------------------------------------------------------------------------------------------------------------------------------------------>  
void BlinkLights()
{
    Blinker = !Blinker;
}



// ------------------------------------------------------------------------------------------------------------------------------------------------>  
// FASTBLINKLIGHTS - This flip/flops our fast blinker variable
// ------------------------------------------------------------------------------------------------------------------------------------------------>  
void FastBlinkLights()
{
    FastBlinker = !FastBlinker;
}




void StopBlinking(unsigned int TimerID)
{
    timer.deleteTimer(TimerID);
}



unsigned int StartWaiting_mS(int mS)
{
    TimeUp = false;
    return timer.setTimeout(mS, SetTimeUp);    // will call function once after ms duration
}



unsigned int StartWaiting_sec(int seconds)
{
    return StartWaiting_mS(seconds*1000);
}



void SetTimeUp()
{
    TimeUp = true;
}


