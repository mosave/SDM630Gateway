#include "Config.h"
#include "Modbus.h"
#include <ModbusRTU.h>
#include <SoftwareSerial.h>

SoftwareSerial sSerial( MODBUS_RX, MODBUS_TX );
ModbusRTU modbus;

word rangeNo = 0;
byte rangeSlaveId = 0;
word rangeFrom = 0; 
word rangeCount = 0;
uint16_t rangeData[22];

bool rangeCopyToData(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if( event == 0 ) {
    if( rangeSlaveId == SDM630_ID ) {
        memcpy( &sdm630Data[rangeFrom], rangeData, rangeCount*sizeof(uint16_t) );
        sdm630Updated = millis();
    }
  } else {
    //Serial.printf_P("ERROR %02X reading device #%d [%d..%d] \r\n", event, rangeSlaveId, (rangeFrom/2+1), (rangeFrom+rangeCount)/2+1 );
  }
  rangeNo++;
  return true;
}

void sdm630Read( int registerFrom, int registerTo ) {
  rangeSlaveId = SDM630_ID;
  rangeFrom = (registerFrom-1) * 2;
  rangeCount = (registerTo-registerFrom+1)*2;
  modbus.readIreg( rangeSlaveId, rangeFrom, rangeData, rangeCount, rangeCopyToData);
}


void modbusLoop() {
  if (!modbus.slave()) {
    switch (rangeNo ) {// target, Slave ID, from register, to register (including)
      case 1: sdm630Read( 11, 20 ); break;
      case 2: sdm630Read( 21, 30 ); break;
      case 3: sdm630Read( 31, 40 ); break;
      case 4: sdm630Read( 41, 44 ); break;
      case 5: sdm630Read( 51, 54 ); break;
      case 6: sdm630Read( 101, 104 ); break;
      case 7: sdm630Read( 113, 120 ); break;
      case 8: sdm630Read( 121, 130 ); break;
      case 9: sdm630Read( 131, 135 ); break;
      case 10: sdm630Read( 168, 177 ); break;
      case 11: sdm630Read( 178, 187 ); break;
      case 12: sdm630Read( 188, 191 ); break;
      default: 
        rangeNo = 0;
        sdm630Read( 1, 10 ); 
        break;
    }
  }
  modbus.task();
}

void modbusInit() {
  sSerial.begin(9600, SWSERIAL_8N1);
  modbus.begin(&sSerial);
  modbus.master();
  
  registerLoop(modbusLoop);
}
