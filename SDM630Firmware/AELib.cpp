#include "Config.h"

#define AELIB_MaxLoops 16

unsigned int aelibLoopCount=0;

LOOP aelibLoops[ AELIB_MaxLoops ];

#ifdef ShowLoopTimes
unsigned long aelibCount = 0;
unsigned long aelibMillis = 0;
#endif

void registerLoop( LOOP loop ) {
  if( aelibLoopCount < AELIB_MaxLoops ) {
    aelibLoops[aelibLoopCount] = loop;
    aelibLoopCount++;
  }
}
void Loop() {
 
  for(int i=0; i<aelibLoopCount; i++ ) {
    if( aelibLoops[i] != NULL ) aelibLoops[i]();
  }

#ifdef ShowLoopTimes
  unsigned long t = millis();
  if(aelibMillis == 0 ) aelibMillis = t;
  if( (unsigned long)(t - aelibMillis) > 10000) {
    Serial.print("loop time = ");Serial.print( ((double)(millis()-aelibMillis))/((double)aelibCount) ); Serial.println("ms");
    aelibMillis = t;
    aelibCount=0;
  } else {
    aelibCount++;
  }
#endif 
}
