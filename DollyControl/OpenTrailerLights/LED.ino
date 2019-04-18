
void ToggleAllLights()
{
    static boolean Status; 

    // Flip flop every other external light
    for (int j=0; j<NumLights; j += 2)
    {    
        Status ? TurnOnLight(j) : TurnOffLight(j);
        Status ? TurnOffLight(j+1) : TurnOnLight(j+1);
    }
    
    // Flop the flip
    Status = !Status;
}


