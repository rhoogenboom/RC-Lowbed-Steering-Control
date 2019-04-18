// THIS FILE HAS SETTINGS WHICH THE USER CAN CONFIGURE

// NUMBER OF SCHEMES
// ------------------------------------------------------------------------------------------------------------------------------------------------>
    #define NumSchemes  1                           // The number of lighting schemes implemented. Theoretically it can be anything up the memory limit. Defaults to 2. 
                                                    // MAKE SURE THIS NUMBER MATCHES THE NUMBER OF SCHEMES DEFINED IN AA_LIGHT_SETUP !!
// LIGHT SETTINGS - DIM LEVEL
// ------------------------------------------------------------------------------------------------------------------------------------------------>
    #define DimLevel                    20          // Number from 0-255, with 0 being off, 255 being full on. Often numbers much greater than half (128) are hard to distinguish from full on. 
                                                    // Experiment to get the number that makes your lights as dim as you want them. 
    
// LIGHT SETTINGS - BLINKING
// ------------------------------------------------------------------------------------------------------------------------------------------------>    
    #define BlinkInterval              650          // A value in milliseconds that sets the blink rate for blinking lights set to "BLINK" or "SOFTBLINK" (for example, turn signals). 1000 = 1 second
    #define FastBlinkInterval           15          // A value in milliseconds that sets the fast blink rate for lights set to "FASTBLINK"

    #define SoftBlinkFadeInDelay         6          // The SOFTBLINK effect fades the light in and out as it blinks. Each fade consists of 20 steps. You can set the amount of delay between each
    #define SoftBlinkFadeOutDelay       17          // step, and the delay can be different for fade in and out. Take for example a FadeInDelay of 5 mS. There are 20 steps to fade in, 
                                                    // so 20 * 5 = 100mS for the light to fade in. The total amount of time for the fade in + the fade out should not exceed BlinkInterval. 



// DEBUGGING
// ------------------------------------------------------------------------------------------------------------------------------------------------>
    #define DEBUG                     false         // Set this to true to receive debugging messages out the serial port. NOTE: This will make the program less responsive, 
                                                    // so turn it off when you are done troubleshooting. 
    #define LED_DEBUG                 false          // If this is set to true, the Green LED on the board will be on whenever the car is moving forward, 
                                                    // the Red LED will come on whenever the car is moving in reverse, 
                                                    // both LEDs will turn OFF when the car is stopped,  
                                                    // both LEDs will turn ON when the car is braking,
                                                    // the Red LED will blink quickly if you are turning left, and
                                                    // the Green LED will blink quickly if you are turning right.
                                                    // You can use these to verify the board is working correctly without having any lights connected.
                                                    // LED_DEBUG does not affect the performance of the circuit, so you can leave it on.                                                  
// SERIAL
// ------------------------------------------------------------------------------------------------------------------------------------------------>
    #define BaudRate                9600          // Set baud rate here if you know what you're doing and don't like the default value

                                                    
                                                    

