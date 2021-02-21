#include "Config.h"
#include "Modbus.h"

// Modbus device Slave id
uint16_t sdm630Data[400];
unsigned long sdm630Updated;

SDM630 sdm630;


/*
float round( float v, int decimals ) {
  switch(decimals) {
    case 0: return round(v);
    case 1: return round(v*10)/10;
    case 2: return round(v*100)/100;
    case 3: return round(v*1000)/1000;
    case 4: return round(v*10000)/10000;
    default: return v;
  }
}
*/

float getFloat( uint16_t parameter ) {
  float f=0.0;
  uint16_t* pf = (uint16_t*)(&f);
  pf[0] = sdm630Data[(parameter-1)*2+1];
  pf[1] = sdm630Data[(parameter-1)*2];
  return f;
}

char* printFloat(char* s, float v, int decimals ){
  dtostrf(v, (decimals>0) ? decimals+2 : 1, decimals, s);
  return s;
}

// Prepare "v" and "v,v1,v2,v3" strings using float values
void printVector( char* value, char* valueL, float v, float v1, float v2, float v3, int decimals ) {
  char t[SDM630_VALUE_SIZE_L];
  strcpy( valueL, printFloat( value, v, decimals ) ); 
  strcat( valueL, "," );
  strcat( valueL, printFloat( t, v1, decimals ) );
  strcat( valueL, "," );
  strcat( valueL, printFloat( t, v2, decimals ) );
  strcat( valueL, "," );
  strcat( valueL, printFloat( t, v3, decimals ) );
}
void printVectorL( char* value, char* valueL, uint16_t L0, uint16_t L1, uint16_t L2, uint16_t L3, int decimals ) {
  printVector( value, valueL, getFloat(L0), getFloat(L1), getFloat(L2), getFloat(L3), decimals );
}

void printVectorL( char* value, char* valueL, uint16_t L1, uint16_t L2, uint16_t L3, int decimals ) {
  float v1 = getFloat(L1);
  float v2 = getFloat(L2);
  float v3 = getFloat(L3);
  printVector( value, valueL, v1 + v2 + v3, v1, v2, v3, decimals);
}

void printVectorLA( char* value, char* valueL, uint16_t L1, uint16_t L2, uint16_t L3, int decimals ) {
  float v = 0;
  float v1 = getFloat(L1);
  float v2 = getFloat(L2);
  float v3 = getFloat(L3);
  int n = 0;
  float m = 0;
  if( m<abs(v1)) m=abs(v1);
  if( m<abs(v2)) m=abs(v2);
  if( m<abs(v3)) m=abs(v3);
  // Set 1% confidence
  if( abs(v1) > m/100.0 ) { v += v1; n+=1; }
  if( abs(v2) > m/100.0 ) { v += v2; n+=1; }
  if( abs(v3) > m/100.0 ) { v += v3; n+=1; }
  if( n>0 ) v = v/n;
  printVector( value, valueL, v, v1, v2, v3, decimals );
}

void sdm630UpdateState() {
  strcpy( sdm630.Alive, ((unsigned long)(millis() - sdm630Updated) < SDM630AliveTimeout) ? "1" : "0");
  printFloat( sdm630.Frequency, getFloat(36), 1);
  printVectorLA( sdm630.Voltage, sdm630.VoltageL, 1, 2, 3, 0 );
  printVectorL( sdm630.Current, sdm630.CurrentL, 4, 5, 6, 2 );
  printVectorL( sdm630.Power, sdm630.PowerL, 7, 8, 9, 0 );
  printVectorL( sdm630.PowerFactor, sdm630.PowerFactorL, 32, 16, 17, 18, 2 );
  printVectorL( sdm630.PhaseAngle, sdm630.PhaseAngleL, 34, 19, 20, 21, 2 );
  printVectorL( sdm630.VA, sdm630.VAL, 10, 11, 12, 0 );
  printVectorL( sdm630.VAr, sdm630.VArL, 13, 14, 15, 0 );
  printVectorL( sdm630.KWH, sdm630.KWHL, 180, 181, 182, 2 );
  printVectorL( sdm630.KWHImport, sdm630.KWHImportL, 174, 175, 176, 2 );
  printVectorL( sdm630.KWHExport, sdm630.KWHExportL, 177, 178, 179, 2 );
}
