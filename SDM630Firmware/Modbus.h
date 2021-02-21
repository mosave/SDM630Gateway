#ifndef aemodbus_h
#define aemodbus_h

// receivePin, transmitPin, inverse_logic, bufSize, isrBufSize
// connect RX to D1 (GPIO5), TX to D2 (GPIO4)
#define MODBUS_RX 5
#define MODBUS_TX 4
#define SDM630_ID 1

#define SDM630AliveTimeout ((unsigned long)10000)
#define SDM630_VALUE_SIZE 12
#define SDM630_VALUE_SIZE_L 48

struct SDM630 {
  char Alive[SDM630_VALUE_SIZE];
  char Frequency[SDM630_VALUE_SIZE];
  char Voltage[SDM630_VALUE_SIZE];
  char VoltageL[SDM630_VALUE_SIZE_L];
  char Current[SDM630_VALUE_SIZE];
  char CurrentL[SDM630_VALUE_SIZE_L];
  char Power[SDM630_VALUE_SIZE];
  char PowerL[SDM630_VALUE_SIZE_L];
  char PowerFactor[SDM630_VALUE_SIZE];
  char PowerFactorL[SDM630_VALUE_SIZE_L];
  char PhaseAngle[SDM630_VALUE_SIZE];
  char PhaseAngleL[SDM630_VALUE_SIZE_L];
  char VA[SDM630_VALUE_SIZE];
  char VAL[SDM630_VALUE_SIZE_L];
  char VAr[SDM630_VALUE_SIZE];
  char VArL[SDM630_VALUE_SIZE_L];
  char KWH[SDM630_VALUE_SIZE];
  char KWHL[SDM630_VALUE_SIZE_L];
  char KWHImport[SDM630_VALUE_SIZE];
  char KWHImportL[SDM630_VALUE_SIZE_L];
  char KWHExport[SDM630_VALUE_SIZE];
  char KWHExportL[SDM630_VALUE_SIZE_L];
};

extern SDM630 sdm630;


extern uint16_t sdm630Data[400];
extern unsigned long sdm630Updated;

void sdm630UpdateState();

void modbusInit();
#endif
