#ifndef config_h
#define config_h
#include <Arduino.h>

#define VERSION "1.0"
#define MQTT_MAX_PACKET_SIZE 512
#define mqtt_max_packet_size 512

#define LED_Pin 2

//#define WIFI_SSID "SSID"
//#define WIFI_Password  "Password"
//#define MQTT_Address "1.1.1.33"
//#define MQTT_Port 1883

#ifndef WIFI_SSID
    #include "Config.AE.h"
#endif

#define Btn0 16

// Define this to allow serial port output
#define aePrintf( ... ) Serial.printf( __VA_ARGS__ )
#define aePrint( ... ) Serial.print( __VA_ARGS__ )
#define aePrintln( ... ) Serial.println( __VA_ARGS__ )

#define LOOP std::function<void()>

void registerLoop( LOOP loop );
void Loop();

#endif
