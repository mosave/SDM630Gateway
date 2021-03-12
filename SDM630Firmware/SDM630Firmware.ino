#include "Config.h"
#include "LED.h"
#include "Storage.h"
#include "Comms.h"
#include "Buttons.h"
#include "Modbus.h"

// Button:
#define Btn 0

// D5/D6/D7/
#define D5 14
#define D6 12
#define D7 13


#define SDM630PublishTimeout ((unsigned long)5000)

//https://github.com/emelianov/modbus-esp8266
static char* TOPIC_SDM630 PROGMEM = "SDM630";

//*****************************************************************************************
// MQTT support
//*****************************************************************************************
void mqttConnect() {
  //mqttSubscribeTopic( TOPIC_SetPosition );
}

bool mqttCallback(char* topic, byte* payload, unsigned int length) {
  //printf("mqttCallback(\"%s\", %u, %u )\r\n", topic, payload, length);
  return false;
}

void publishVal( char* Topic, char* _value, char* value, bool retained=false) {
  if( (strcmp( _value, value) != 0 ) && mqttPublish( Topic, value, retained ) ) {
    //print(_value ); print(" != "); println(value);
      strcpy( _value, value );
  }
  
}

static SDM630 _sdm630;

void publishState() {
  unsigned long t=millis();
  static unsigned long sdm630PublishedOn = 0;

  if( (unsigned long)(t - sdm630PublishedOn) > SDM630PublishTimeout ) {
    char json[ 512 ];
    sdm630UpdateState();
    publishVal("SDM630/Alive", _sdm630.Alive, sdm630.Alive);
    publishVal("SDM630/Frequency", _sdm630.Frequency, sdm630.Frequency);
    publishVal("SDM630/Voltage", _sdm630.Voltage, sdm630.Voltage);
    publishVal("SDM630/VoltageL", _sdm630.VoltageL, sdm630.VoltageL);
    publishVal("SDM630/Current", _sdm630.Current, sdm630.Current);
    publishVal("SDM630/CurrentL", _sdm630.CurrentL, sdm630.CurrentL);
    publishVal("SDM630/Power", _sdm630.Power, sdm630.Power);
    publishVal("SDM630/PowerL", _sdm630.PowerL, sdm630.PowerL);

    publishVal("SDM630/PowerFactor", _sdm630.PowerFactor, sdm630.PowerFactor);
    publishVal("SDM630/PowerFactorL", _sdm630.PowerFactorL, sdm630.PowerFactorL);
    publishVal("SDM630/PhaseAngle", _sdm630.PhaseAngle, sdm630.PhaseAngle);
    publishVal("SDM630/PhaseAngleL", _sdm630.PhaseAngleL, sdm630.PhaseAngleL);

    publishVal("SDM630/VA", _sdm630.VA, sdm630.VA);
    publishVal("SDM630/VAL", _sdm630.VAL, sdm630.VAL);
    publishVal("SDM630/VAr", _sdm630.VAr, sdm630.VAr);
    publishVal("SDM630/VArL", _sdm630.VArL, sdm630.VArL);
    publishVal("SDM630/KWH", _sdm630.KWH, sdm630.KWH, true);
    publishVal("SDM630/KWHL", _sdm630.KWHL, sdm630.KWHL, true);
    publishVal("SDM630/KWHImport", _sdm630.KWHImport, sdm630.KWHImport);
    publishVal("SDM630/KWHImportL", _sdm630.KWHImportL, sdm630.KWHImportL);
    publishVal("SDM630/KWHExport", _sdm630.KWHExport, sdm630.KWHExport);
    publishVal("SDM630/KWHExportL", _sdm630.KWHExportL, sdm630.KWHExportL);
    
    sdm630PublishedOn = millis();
  }
}


//*****************************************************************************************
// Setup
//*****************************************************************************************
void setup() {
  ledInit( On );
  Serial.begin(115200);
  delay(500); 
  aePrintln();  aePrintln("Initializing");

  storageInit();
  commsInit();
  btnInit();
  btnRegister( Btn, "Button", false, true);
  btnRegister( D5, "D5", false, true );
  btnRegister( D6, "D6", false, true );
  btnRegister( D7, "D7", false, true );

  modbusInit();
  ledMode(Off);
}


void loop() {
  Loop();

  publishState();

  if ( mqttConnected() ) {
    ledMode( Standby ) ;
  } else {
    ledMode( BlinkFast ) ;
  }
  delay(10);
}
