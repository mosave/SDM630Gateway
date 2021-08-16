#ifndef led_h
#define led_h

enum LedMode {
  Off,
  BlinkFast,
  Blink,
  BlinkInverted,
  BlinkTwice,
  BlinkSlow,
  BlinkSlowInverted,
  Standby,
#ifdef ESP8266
  Glowing,
#endif
  On
};

LedMode ledMode();
void ledMode(LedMode newMode);
LedMode ledModeNext();
void ledInit();
void ledInit( LedMode defaultMode );

#endif
