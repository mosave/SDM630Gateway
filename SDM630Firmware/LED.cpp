#include <Arduino.h>
#include <EEPROM.h>
#include "Config.h"
#include "LED.h"

#define PatternSize 6
LedMode _ledMode = Off;
bool ledOn = false;
unsigned int triggeredOn;
unsigned int updateTimeout;
unsigned int pattern[6];
int patternP;
int glowStep;

LedMode ledMode() {
  return _ledMode;
}
void blinkConfig( bool initial, unsigned int p0, int p1, int p2=0, int p3=0, int p4=0, int p5=0 ) {
  ledOn = initial; updateTimeout = 10;
  digitalWrite( LED_Pin, ledOn ? LOW : HIGH );
  pattern[0]=p0; pattern[1]=p1; pattern[2]=p2;
  pattern[3]=p3; pattern[4]=p4; pattern[5]=p5;
}

void ledMode(LedMode newMode) {
  if( (LED_Pin<=0) || (newMode == _ledMode) ) return;
  _ledMode = newMode;
  triggeredOn = millis();
  patternP = 0;
  if( _ledMode == On ) {
    ledOn = true;
    updateTimeout = (unsigned long)(-1);
    digitalWrite( LED_Pin, LOW );
  } else if( _ledMode ==  BlinkFast ) {
    blinkConfig( false, 190, 10 );
  } else if( _ledMode ==  Blink) {
    blinkConfig( false, 1950, 50 );
  } else if( _ledMode ==  BlinkInverted) {
    blinkConfig( true, 1900, 100 );
  } else if( _ledMode ==  BlinkTwice) {
    blinkConfig( false, 1300, 50, 100, 50 );
  } else if( _ledMode ==  BlinkSlow ) {
    blinkConfig( false, 4700, 300 );
  } else if( _ledMode ==  BlinkSlowInverted ) {
    blinkConfig( true, 4800, 200 );
  } else if( _ledMode ==  Standby) {
    blinkConfig( false, 9950, 50 );
  } else if( _ledMode ==  Glowing) {
    ledOn = true; updateTimeout = 50;
    patternP = 0; glowStep = -60;
    analogWrite( LED_Pin, patternP );
  } else {
    _ledMode = Off;
    ledOn = false;  updateTimeout = (unsigned long)(-1);
    digitalWrite( LED_Pin, HIGH );
  }
//  printf("Mode %d: %lu / %lu\r\n", mode, timeoutOn, timeoutOff );
}

LedMode ledModeNext() {
    LedMode lm = (LedMode)(((char)_ledMode)+1);
    if( lm>On ) lm = Off;
    ledMode(lm);
    return _ledMode;
}


void ledLoop() {
  static unsigned long updatedOn = 0;

  unsigned long t = millis();
  if( (unsigned long)(t - updatedOn) > updateTimeout ) {
    bool switchOn = ledOn;
    updatedOn = t;
    if( (LED_Pin<=0) || ( _ledMode == Off) ) {
      switchOn = false;
    } else if ( _ledMode == On) {
      switchOn = true;
    } else if ( _ledMode == Glowing) {
      patternP += glowStep;
      if( patternP>=1000 ) {
        glowStep = -glowStep;
        patternP = 1000;
      } else if( patternP<=0 ) {
        glowStep = -glowStep;
        patternP = 0;
      }
      analogWrite( LED_Pin, patternP );
    } else {
      if( ((unsigned long)(t - triggeredOn) > (unsigned long)pattern[patternP] ) ) {
          switchOn = !ledOn;
      }
      if( switchOn != ledOn ) {
        ledOn = switchOn;
        triggeredOn = t;
        patternP++;
        if( (patternP>=PatternSize) || (pattern[patternP]==0) ) patternP=0;
        digitalWrite( LED_Pin, ledOn ? LOW : HIGH );
      }
    }
  }
}



void ledInit( LedMode defaultMode) {
  triggeredOn = 0;
  if( LED_Pin>0 ) {
    pinMode(LED_Pin, OUTPUT);
    digitalWrite( LED_Pin, HIGH );
  }
  _ledMode = Off;
  ledMode( defaultMode );
  registerLoop(ledLoop);
}

void ledInit() {
  ledInit( Off );
}
