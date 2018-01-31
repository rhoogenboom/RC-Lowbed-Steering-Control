#include "Arduino.h"
#include "OSLController.h"

OSLController::OSLController() {

    _connectedRX = false;
    _positionController1 = 1500;
    _positionController2 = 1500;
    _positionController3 = 1500;
    _driveMode = 0;
    for (int i=0; i<NumLights; i++) {
      _lights[NumLights] = 0;  
    }
}

bool OSLController::getReceiverState() {
  return _connectedRX;
}

void OSLController::setReceiverState(bool connected) {
  _connectedRX = connected;
}

int OSLController::getController1() {
  return _positionController1;
}

void OSLController::setController1(int pulse) {
  _connectedRX = true;
  _positionController1 = pulse;
}

int OSLController::getController2() {
  return _positionController2;
}

void OSLController::setController2(int pulse) {
  _connectedRX = true;
  _positionController2 = pulse;
}

int OSLController::getController3() {
  return _positionController3;
}

void OSLController::setController3(int pulse) {
  _connectedRX = true;
  _positionController3 = pulse;
}

int OSLController::getDriveMode() {
  return _driveMode;
}

int OSLController::setDriveMode(int mode) {
  _connectedRX = true;
  _driveMode = mode;
}

int OSLController::getLight(int lightNum) {
  return _lights[lightNum-1];
}

void OSLController::setLight(int lightNum, int lightState) {
  _connectedRX = true;
  _lights[lightNum-1] = lightState;
}

int OSLController::processSettings(char input[32]){
  //set a bool that we have updates?
  switch ((char)input[0]) {
    case 'c':
      _connectedRX = true;
      handleControllerInput(input);  
      return 0;
    break;
    case 'm':
      _connectedRX = true;
      return handleLightInput(input, input[1]);
    break;
    case 'd':
      setDisconnectedState();
      return -1;
    default:
      //maybe return false when structure isn't as expected
      setDisconnectedState();
      return -1;
    break;
  }
}

void OSLController::setDisconnectedState() {
  _connectedRX = false;
}

void OSLController::handleControllerInput(char input[32]) {
  String values = String(input);
  int equalLocation = values.indexOf("=")+1;
  int separatorLocation = values.indexOf(";");
  _positionController1 = values.substring(equalLocation, separatorLocation).toInt();
  values = values.substring(separatorLocation+1);
  equalLocation = values.indexOf("=")+1;
  separatorLocation = values.indexOf(";");
  _positionController2 = values.substring(equalLocation, separatorLocation).toInt();
  values = values.substring(separatorLocation+1);
  equalLocation = values.indexOf("=")+1;
  separatorLocation = values.indexOf(";");
  _positionController3 = values.substring(equalLocation, separatorLocation).toInt();
  values = values.substring(separatorLocation+1);
  equalLocation = values.indexOf("=")+1;
  separatorLocation = values.indexOf(";");
  _driveMode = values.substring(equalLocation, separatorLocation).toInt();
}

int OSLController::handleLightInput(char input[32], char whichLight) {
  String values = String(input);
  int equalLocation = values.indexOf("=")+1;
  int separatorLocation = values.indexOf(";");

  if (whichLight == '1') {
    _lights[0] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[1] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[2] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[3] = values.substring(equalLocation, separatorLocation).toInt();
    return 1;
  } 
  if (whichLight == '5') {
    _lights[4] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[5] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[6] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[7] = values.substring(equalLocation, separatorLocation).toInt();
    return 2;
  }
  if (whichLight == '9') {
    _lights[8] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[9] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[10] = values.substring(equalLocation, separatorLocation).toInt();
    values = values.substring(separatorLocation+1);
    equalLocation = values.indexOf("=")+1;
    separatorLocation = values.indexOf(";");
    _lights[11] = values.substring(equalLocation, separatorLocation).toInt();
    return 3;
  }
}

String OSLController::getControllerInfo() {
  char result[32];

  if (_connectedRX) {
    sprintf(result, "c1=%d;c2=%d;c3=%d;mode=%d;", 
      _positionController1, _positionController2, _positionController3, _driveMode);
  } else {
    sprintf(result, "disconnected RX");
  }

  return String(result);
}

String OSLController::getLightsInfo1_4() {
  char result[32];

  if (_connectedRX) {
    sprintf(result, "m1=%d;m2=%d;m3=%d;m4=%d;", 
      _lights[0], _lights[1], _lights[2], _lights[3]);
  } else {
    sprintf(result, "disconnected RX");
  }

  return String(result);
}

String OSLController::getLightsInfo5_8() {
  char result[32];

  if (_connectedRX) {
    sprintf(result, "m5=%d;m6=%d;m7=%d;m8=%d;", 
      _lights[4], _lights[5], _lights[6], _lights[7]);
  } else {
    sprintf(result, "disconnected RX");
  }

  return String(result);
}

String OSLController::getLightsInfo9_12() {
  char result[32];

  if (_connectedRX) {
    sprintf(result, "m5=%d;m6=%d;m7=%d;m8=%d;", 
      _lights[8], _lights[9], _lights[10], _lights[11]);
  } else {
    sprintf(result, "disconnected RX");
  }

  return String(result);
}

String OSLController::printDebugInfo() {
  char result[96];

  sprintf(result, "c1=%d;c2=%d;c3=%d;mode=%d;m1=%d;m2=%d;m3=%d;m4=%d;m5=%d;m6=%d;m7=%d;m8=%d;m9=%d;m10=%d;m11=%d;m12=%d;", 
    _positionController1, _positionController2, _positionController3, _driveMode,
    _lights[0], _lights[1], _lights[2], _lights[3], _lights[4], _lights[5], _lights[6], _lights[7], _lights[8], _lights[9], _lights[10], _lights[11]);

  return String(result);
}


