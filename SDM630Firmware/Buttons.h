#ifndef buttons_h
#define buttons_h
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
bool btnShortPressed( byte btnPin );
bool btnLongPressed( byte btnPin );
bool btnVeryLongPressed( byte btnPin );
bool btnReleased( byte btnPin );

bool btnState( byte btnPin, byte btnPin2 );
bool btnPressed( byte btnPin, byte btnPin2 );
bool btnShortPressed( byte btnPin, byte btnPin2 );
bool btnLongPressed( byte btnPin, byte btnPin2 );
bool btnVeryLongPressed( byte btnPin, byte btnPin2 );
bool btnReleased( byte btnPin, byte btnPin2 );


void btnInit();
#endif
