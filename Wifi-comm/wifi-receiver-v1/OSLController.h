#ifndef OSLController_h
#define OSLController_h

#include "Arduino.h"

#endif

class OSLController {
  public:
    OSLController();

    int getController1();
    int getController2();
    int getController3();
    bool getReceiverState();
    void setController1(int pulse);
    void setController2(int pulse);
    void setController3(int pulse);
    void setReceiverState(bool connected);
    int getDriveMode();
    int setDriveMode(int mode);
    int getLight(int lightNum);
    void setLight(int lightNum, int lightState);
    int processSettings(char input[32]);
    String getControllerInfo();
    String getLightsInfo1_4();
    String getLightsInfo5_8();
    String getLightsInfo9_12();
    String printDebugInfo();

  private:
    void handleControllerInput(char input[32]);
    int handleLightInput(char input[32], char whichLight);
    void setDisconnectedState();
  
    #define NumLights 13
    
    bool _connectedRX;
    int _positionController1;
    int _positionController2;
    int _positionController3;
    int _driveMode;
    int _lights[NumLights];

    
};


