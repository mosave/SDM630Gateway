#ifndef buttons_h
#define buttons_h
#include "Config.h"


enum BtnDefaultFunction {
  BDF_None, // none
  BDF_EnableOTA, // enable OTA on 10th fast click
  BDF_SwitchWIFI, // enable/disable WIFI and MQTT support and restart
  BDF_FactoryReset, // Reset storage on 10th click
  BDF_Reset // Reset device on 10th click
};

void btnRegister( byte btnPin, char* mqttName, bool btnInverted, bool pullUp);
void btnDefaultFunction( byte btnPin, BtnDefaultFunction bdf );

char* btnName( byte btnPin );

bool btnState( byte btnPin );
bool btnPressed( byte btnPin );
bool btnReleased( byte btnPin );
bool btnShortPressed( byte btnPin );
bool btnLongPressed( byte btnPin );
#ifndef ButtonsEasyMode
bool btnVeryLongPressed( byte btnPin );
#endif

bool btnState( byte btnPin, byte btnPin2 );
bool btnPressed( byte btnPin, byte btnPin2 );
bool btnReleased( byte btnPin, byte btnPin2 );
bool btnShortPressed( byte btnPin, byte btnPin2 );
bool btnLongPressed( byte btnPin, byte btnPin2 );
#ifndef ButtonsEasyMode
bool btnVeryLongPressed( byte btnPin, byte btnPin2 );
#endif

void btnInit();
#endif
