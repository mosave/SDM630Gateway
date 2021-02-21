#include <Arduino.h>
#include "Buttons.h"
#include "Config.h"
#include "Storage.h"
#include "Comms.h"
//#define Debug

// Maximum number of buttons supported
#define ButtonsSize 8
#define ButtonNameSize 16
// Anticlush filtering timeout, ms
#define ClashTimeout ((unsigned long)100)
#define LongPressTimeout ((unsigned long)1000)
#define VeryLongPressTimeout ((unsigned long)10000)

// "%s" will be button name
static char* TOPIC_State PROGMEM = "%s/State";
static char* TOPIC_ShortPressed PROGMEM = "%s/ShortPressed";
static char* TOPIC_LongPressed PROGMEM = "%s/LongPressed";
static char* TOPIC_VeryLongPressed PROGMEM = "%s/VeryLongPressed";

// Normal button: 
// LOW == pressed
// HIGH == released

struct Btn {
  byte pin; // Pin number
  bool inverted; // false if button connects to ground
  bool pressed; // inverted ? (digitalRead(pin) == HIGH) : (digitalRead(pin) == LOW );
  BtnDefaultFunction bdf;
  char name[ButtonNameSize];
  unsigned long triggeredOn;
  unsigned long lastPressedOn;
  int fastCount;
  
  bool wasPressed;
  bool wasReleased;
  bool wasShortPressed;
  bool wasLongPressed;
  bool wasVeryLongPressed;
  int reportedState;
};

byte btnCount = 0;

Btn buttons[ButtonsSize];


short int btnIndex( byte btnPin ) {
  for( int i=0; i<btnCount; i++ ) {
    if(buttons[i].pin == btnPin ) return i;
  }
  return -1;
}

#ifdef Debug
void btnShowStatus( Btn* btn ) {
  Serial.print( (strlen(btn->name)>0) ? btn->name : "Button" );
  Serial.print(F(" #")); Serial.print(btn->pin); Serial.print(": "); 
  if( btn->wasPressed ) Serial.print(F("wasPressed ")); 
  if( btn->wasShortPressed ) Serial.print(F("wasShortPressed ")); 
  if( btn->wasLongPressed ) Serial.print(F("wasLongPressed ")); 
  if( btn->wasVeryLongPressed ) Serial.print(F("wasVeryLongPressed ")); 
  if( btn->wasReleased ) Serial.print(F("wasReleased ")); 
  Serial.println();
}
#endif


void btnLoop( Btn* btn, bool pressed ) {
  unsigned long t = millis();
  char topic[64];

  if( strlen(btn->name)>0 ) {
    int state = pressed ? 1 : 0;
    if( (state != btn->reportedState) && mqttPublish( TOPIC_State, btn->name, state, false  ) ) btn->reportedState = state;
  }

  if( pressed != btn-> pressed ) {
    btn->pressed = pressed;
    if( pressed ) { // button pressed
      if( (unsigned long)(t - btn->lastPressedOn) < (unsigned long)1000 ) {
        btn->fastCount++;
        if( btn->fastCount>=10 ) {
          if( btn->bdf == BDF_Reset ) commsRestart();
          if( btn->bdf == BDF_FactoryReset ) storageReset();
          if( btn->bdf == BDF_EnableOTA ) commsEnableOTA();
          if( btn->bdf == BDF_SwitchWIFI ) {
            if( wifiEnabled() ) {
              wifiDisable();
            } else {
              wifiEnable();
            }
          }
          btn->fastCount = -1000;
        }
      } else {
        btn->fastCount = 1;
      }
      btn->lastPressedOn = t;
      btn->wasPressed = true;
      btn->wasReleased = false;
      btn->wasShortPressed = false;
      btn->wasLongPressed = false;
      btn->wasVeryLongPressed = false;
    } else { // button released
      btn->wasPressed = false;
      btn->wasReleased = true;
      if( (unsigned long)(t - btn->triggeredOn) < LongPressTimeout ) {
        btn->wasShortPressed = true;
        if( strlen(btn->name)>0 ) mqttPublish( TOPIC_ShortPressed, btn->name, "", false );
      } else if( (unsigned long)(t - btn->triggeredOn) < VeryLongPressTimeout ) {
        btn->wasLongPressed = true;
        if( strlen(btn->name)>0 ) mqttPublish( TOPIC_LongPressed, btn->name, "", false  );
      } else {
        btn->wasVeryLongPressed = true;
        if( strlen(btn->name)>0 ) mqttPublish( TOPIC_VeryLongPressed, btn->name, "", false  );
      }
    }
    btn->triggeredOn = t;
#ifdef Debug
    btnShowStatus(btn);
#endif        
  }

}

void btnRegister( byte btnPin, char* mqttName, bool inverted, bool pullUp ) {
  if(btnCount < ButtonsSize-1) {
    pinMode( btnPin, pullUp ? INPUT_PULLUP : INPUT);
    buttons[btnCount].pin = btnPin;
    if( (mqttName != NULL) && (strlen(mqttName)>0) ) {
      strncpy( buttons[btnCount].name, mqttName, ButtonNameSize );
    }
    buttons[btnCount].inverted = inverted;
    buttons[btnCount].fastCount = 0;
    buttons[btnCount].bdf = BDF_None;
    buttons[btnCount].pressed = false;
    buttons[btnCount].reportedState = -1;
    btnCount++;
    btnLoop(&buttons[btnCount-1], false);
  }
}
void btnDefaultFunction( byte btnPin, BtnDefaultFunction bdf ) {
  short int index = btnIndex(btnPin);
  if( index>=0 ) buttons[index].bdf = bdf;
}

bool btnState( byte btnPin ) {
  short int index = btnIndex(btnPin);
  return ( (index>=0) && buttons[index].pressed );
}

bool btnState( byte btnPin, byte btnPin2 ) {
  short int index = btnIndex(btnPin);
  short int index2 = btnIndex(btnPin2);
  return ( (index>=0) && buttons[index].pressed && (index2>=0) && buttons[index2].pressed );
}

bool btnPressed( byte btnPin ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasPressed ) return false;
  buttons[index].wasPressed = false; 
  return true;
}

bool btnPressed( byte btnPin, byte btnPin2 ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasPressed ) return false;

  short int index2 = btnIndex(btnPin2);
  if( (index2<0) || !buttons[index2].wasPressed ) return false;
  
  buttons[index].wasPressed = false; 
  buttons[index2].wasPressed = false; 
  return true;
}

bool btnReleased( byte btnPin ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasReleased ) return false;
  buttons[index].wasReleased = false; 
  return true;
}

bool btnReleased( byte btnPin, byte btnPin2 ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasReleased ) return false;

  short int index2 = btnIndex(btnPin2);
  if( (index2<0) || !buttons[index2].wasReleased ) return false;

  buttons[index].wasReleased = false; 
  buttons[index2].wasReleased = false; 
  return true;
}

bool btnShortPressed( byte btnPin ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasShortPressed ) return false;
  buttons[index].wasShortPressed = false; 
  return true;
}

bool btnShortPressed( byte btnPin, byte btnPin2 ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasShortPressed ) return false;

  short int index2 = btnIndex(btnPin2);
  if( (index2<0) || !buttons[index2].wasShortPressed ) return false;

  buttons[index].wasShortPressed = false; 
  buttons[index2].wasShortPressed = false; 
  return true;
}

bool btnLongPressed( byte btnPin ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasLongPressed ) return false;
  buttons[index].wasLongPressed = false; 
  return true;
}

bool btnLongPressed( byte btnPin, byte btnPin2 ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasLongPressed ) return false;

  short int index2 = btnIndex(btnPin2);
  if( (index2<0) || !buttons[index2].wasLongPressed ) return false;

  buttons[index].wasLongPressed = false; 
  buttons[index2].wasLongPressed = false; 
  return true;
}

bool btnVeryLongPressed( byte btnPin ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasVeryLongPressed ) return false;
  buttons[index].wasVeryLongPressed = false; 
  return true;
}

bool btnVeryLongPressed( byte btnPin, byte btnPin2 ) {
  short int index = btnIndex(btnPin);
  if( (index<0) || !buttons[index].wasVeryLongPressed ) return false;

  short int index2 = btnIndex(btnPin2);
  if( (index2<0) || !buttons[index2].wasVeryLongPressed ) return false;

  buttons[index].wasVeryLongPressed = false; 
  buttons[index2].wasVeryLongPressed = false; 
  return true;
}

void btnsLoop() {
  static byte _states = 0b1111111;
  static unsigned long changedOn = 0;
  byte states = 0;
  unsigned long t = millis();
  bool triggered = false;

  for(int i=0; i<btnCount; i++ ) {
    bool pressed = (digitalRead(buttons[i].pin) == LOW);
    if( buttons[i].inverted ) pressed = !pressed;
    states |= (pressed ? (1 << i) : 0 );
  }

  if( states != _states ) { // button states changed
    _states = states;
    changedOn = t;
  } else { 
    if( (changedOn>0) && (unsigned long)(t - changedOn) > ClashTimeout ) {
      triggerActivity();
      changedOn = 0;
      // Process states;
      for(int i=0; i<btnCount; i++ ) {
        btnLoop( &buttons[i], ((states & (1<<i)) != 0) );
      }
    }
  }
  
}

void btnInit() {
  registerLoop(btnsLoop);
}
