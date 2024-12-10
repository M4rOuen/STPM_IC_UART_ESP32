//MarOuen Modifyyyyyyyyyy
#include "STPM3X.h"


DSP_CR100bits_t DSP_CR100bits;
DSP_CR101bits_t DSP_CR101bits;
DSP_CR200bits_t DSP_CR200bits;
DSP_CR201bits_t DSP_CR201bits;
DSP_CR400bits_t DSP_CR400bits;
US1_REG100bits_t US1_REG100bits;
DFE_CR101bits_t DFE_CR101bits;
DFE_CR201bits_t DFE_CR201bits;
DSP_CR301bits_t DSP_CR301bits;
DSP_CR500bits_t DSP_CR500bits;

STPM::STPM(int resetPin, int csPin, int synPin, HardwareSerial &uart) {
  RESET_PIN = resetPin;
  CS_PIN = csPin;
  SYN_PIN = synPin;
  _uart = &uart;
  _autoLatch = false;
  _crcEnabled = true;
  for (uint8_t i = 0; i < 3; i++) {
    _calibration[i][0] = 1.0;
    _calibration[i][1] = 1.0;
  }
}
STPM::STPM(int resetPin, int csPin, HardwareSerial &uart) {
  RESET_PIN = resetPin;
  CS_PIN = csPin;
  SYN_PIN = -1;
  _uart = &uart;
  _autoLatch = false;
  _crcEnabled = true;
  for (uint8_t i = 0; i < 3; i++) {
    _calibration[i][0] = 1.0;
    _calibration[i][1] = 1.0;
  }
}
bool STPM::init() {
  pinMode(CS_PIN, OUTPUT);
  if (SYN_PIN != -1) pinMode(SYN_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);
  digitalWrite(RESET_PIN, LOW);
  delay(35);
  digitalWrite(RESET_PIN, HIGH);
  delay(35);
  digitalWrite(CS_PIN, HIGH);
  delay(2);
  // Init sequence by togling 3 times syn pin
  if (SYN_PIN != -1) {
    for (size_t i = 0; i < 3; i++) {
      digitalWrite(SYN_PIN, LOW);
      delay(2);
      digitalWrite(SYN_PIN, HIGH);
      delay(2);
    }
  }
  delay(2);
  digitalWrite(CS_PIN, LOW);
  delay(5);
  digitalWrite(CS_PIN, HIGH);
  bool success = Init_STPM34();
  return success;
}
bool STPM::Init_STPM34() {
  #ifdef DEBUG_DEEP
	Serial.println(F("Start Init_STPM34"));
  #endif
  uint8_t readAdd, writeAdd, dataLSB, dataMSB;
  #ifdef DEBUG_DEEP
  Serial.println(F("Info:Set DSP Control Register 1 LSW"));
  #endif
  DSP_CR100bits.ENVREF1 = 1;       
  DSP_CR100bits.TC1 = 0x02;         
  readAdd = 0x00;
  writeAdd = 0x00;
  dataLSB = DSP_CR100bits.LSB;
  dataMSB = DSP_CR100bits.MSB;
  sendFrameCRC(0x00, 0x00, dataLSB, dataMSB); 
  #ifdef DEBUG_DEEP
  Serial.println(F("Info:Set DSP Control Register 1 MSW"));
  #endif
    DSP_CR101bits.BHPFV1 = 0;   
  DSP_CR101bits.BHPFC1 = 0;       
  DSP_CR101bits.BLPFV1 = 1;       
  DSP_CR101bits.BLPFC1 = 1;       
  DSP_CR101bits.LPW1 = 0x04;       
  readAdd = 0x00;
  writeAdd = 0x01;
  dataLSB = DSP_CR101bits.LSB;
  dataMSB = DSP_CR101bits.MSB;
  sendFrameCRC(readAdd, writeAdd, dataLSB, dataMSB); 
  
  #ifdef DEBUG_DEEP
  Serial.println(F("Info:Set DSP Control Register 2 LSW"));
  #endif
  DSP_CR200bits.ENVREF2 = 1;      
  DSP_CR200bits.TC2 = 0x02;        
  readAdd = 0x01;
  writeAdd = 0x02;
  dataLSB = DSP_CR200bits.LSB;
  dataMSB = DSP_CR200bits.MSB;
  sendFrameCRC(readAdd, writeAdd, dataLSB, dataMSB); 

  #ifdef DEBUG_DEEP
  Serial.println(F("Info:Set DSP Control Register 2 MSW"));
  #endif
  DSP_CR201bits.BHPFV2 = 0;
  DSP_CR201bits.BHPFC2 = 0;
  DSP_CR201bits.BLPFV2 = 1;       
  DSP_CR201bits.BLPFC2 = 1;      
  DSP_CR201bits.LPW2 = 0x04;       
  readAdd = 0x02;
  writeAdd = 0x03;
  dataLSB = DSP_CR201bits.LSB;
  dataMSB = DSP_CR201bits.MSB;
  sendFrameCRC(readAdd, writeAdd, dataLSB, dataMSB);
  setCurrentGain(1, sixteenX);
  setCurrentGain(2, sixteenX);
  readFrame(0x18, readBuffer);
  bool success = checkGain(1, readBuffer);
  
  #ifdef DEBUG_DEEP
  Serial.println(F("Info:GAIN (Bit 26/27):"));
  printRegister(readBuffer, "GainC1:");
  Serial.println(F("Info:GAIN (Bit 26/27):"));
  Serial.print(F("Info:SetGain1: "));
  Serial.println(success ? "Success" : "Fail");
  #endif
  readFrame(0x1A, readBuffer);
  success = success && checkGain(2, readBuffer);

  #ifdef DEBUG_DEEP
  printRegister(readBuffer, "GainC2:");
  Serial.print(F("Info:SetGain2: "));
  Serial.println(success ? "Success" : "Fail");
  #endif

  #ifdef DEBUG_DEEP
  Serial.println(F("Info:LPW: (Bit 24-27)"));
  readFrame(0x0, readBuffer);
  printRegister(readBuffer, "LPW1:");
  readFrame(0x01, readBuffer);
  printRegister(readBuffer, "LPW2:");
  #endif
  autoLatch(false);
  CRC(false);

  #ifdef DEBUG_DEEP
	Serial.println(F("End Init_STPM34"));
  #endif
  return success;
}

void STPM::CRC(bool enabled) {
  if (_crcEnabled == enabled) return;
  // Disable CRC
  if (!enabled) {
    #ifdef DEBUG_DEEP
    Serial.println(F("Info:Disable CRC"));
    #endif
    US1_REG100bits.CRC_EN=0;       
    sendFrameCRC(0x24,0x24,US1_REG100bits.LSB,US1_REG100bits.MSB);
  } else {
    #ifdef DEBUG_DEEP
    Serial.println(F("Info:Enable CRC"));
    #endif
    US1_REG100bits.CRC_EN=1;        
    sendFrame(0x24,0x24,US1_REG100bits.LSB,US1_REG100bits.MSB);
  }
  _crcEnabled = enabled;
}
void STPM::autoLatch(bool enabled) {
  #ifdef DEBUG_DEEP
  Serial.println(F("Info:Set DSP Control Register 3 LSW"));
  if (enabled) {
    Serial.println(F("Info:Automatic latching"));
  } else {
    Serial.println(F("Info:Manual latching"));
  }
  #endif
  _autoLatch = enabled;
  if (_autoLatch) {
    DSP_CR301bits.SWAuto_Latch = 1;     
    DSP_CR301bits.SW_Latch1 = 0;
    DSP_CR301bits.SW_Latch2 = 0;
  } else {
    DSP_CR301bits.SWAuto_Latch = 0;     
    DSP_CR301bits.SW_Latch1 = 1;
    DSP_CR301bits.SW_Latch2 = 1;
  }
  if (_crcEnabled) sendFrameCRC(0x05, 0x05, DSP_CR301bits.LSB, DSP_CR301bits.MSB);
  else sendFrame(0x05, 0x05, DSP_CR301bits.LSB, DSP_CR301bits.MSB);
}

bool STPM::checkGain(uint8_t channel, uint8_t *buffer) {
  if (channel != 1 && channel != 2) {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info:checkCurrentGain: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return false;
  }
  if (channel == 1) {
    if (DFE_CR101bits.LSB != buffer[2]) return false;
    if (DFE_CR101bits.MSB != buffer[3]) return false;
    if (DFE_CR101bits.GAIN1 != _gain1) return false;
  } else {
    if (DFE_CR201bits.LSB != buffer[2]) return false;
    if (DFE_CR201bits.MSB != buffer[3]) return false;
    if (DFE_CR201bits.GAIN1 != _gain2) return false;
  }
  return true;
}
void STPM::setCurrentGain(uint8_t channel, Gain gain) {
  if (channel != 1 && channel != 2) {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info:setCurrentGain: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return;
  }
  uint8_t readAdd, writeAdd, dataLSB, dataMSB;


  if (channel == 1) {
    readFrame(0x18, readBuffer);
    readAdd = 0x00;
    writeAdd = 0x19;
    DFE_CR101bits.LSB = readBuffer[2];
    DFE_CR101bits.MSB = readBuffer[3];
    DFE_CR101bits.GAIN1 = gain;
    _gain1 = gain;
    dataLSB = DFE_CR101bits.LSB;
    dataMSB = DFE_CR101bits.MSB;   
  } else {
    readFrame(0x1A, readBuffer);
    readAdd = 0x00;
    writeAdd = 0x1B;
    DFE_CR201bits.LSB = readBuffer[0];
    DFE_CR201bits.MSB = readBuffer[1];
    DFE_CR201bits.GAIN1 = gain;
    _gain2 = gain;
    dataLSB = DFE_CR201bits.LSB;
    dataMSB = DFE_CR201bits.MSB;
  }
  sendFrameCRC(readAdd, writeAdd, dataLSB, dataMSB);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*All functions by m4rOuen let's gooooooooooooooooooooooooooooooooooo*/
/********************************************************************/
/**********************************************************************/
/**********************************************************************/

float STPM::readVoltage(uint8_t channel) {
  
  if (channel == 1) { 
    address = V1_Data_Address;
  } else if (channel == 2) {
    address = V2_Data_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();
  
  readFrame(address, readBuffer);
  return (calcVolt((int32_t)buffer0to32(readBuffer)))*_calibration[channel][_V];
}

float STPM::readRMSVoltage(uint8_t channel) {
  
  if (channel == 1) { 
    address = C1_RMS_Data_Address;
  } else if (channel == 2) {
    address = C2_RMS_Data_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return calcVolt((int16_t)buffer0to14(readBuffer))*_calibration[channel][_V];
}

float STPM::readCurrent(uint8_t channel) {
  
  if (channel == 1) { 
    address = V1_Data_Address;
  } else if (channel == 2) {
    address = V2_Data_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();
  

  readFrame(address, readBuffer);
  return (calcCurrent((int32_t)buffer0to32(readBuffer)))*_calibration[channel][_I];
}


float STPM::readActiveEnergy(uint8_t channel) {
  if (!_autoLatch) latch();
  if (channel != 1 && channel != 2) {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info:readActiveEnergy: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  uint8_t address = PH1_Active_Energy_Address;
  if (channel == 2) address = PH2_Active_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}


float STPM::readReactiveEnergy(uint8_t channel) {
  if (!_autoLatch) latch();
  if (channel != 1 && channel != 2) {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info:readReactiveEnergy: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  uint8_t address = PH1_Reactive_Energy_Address;
  if (channel == 2) address = PH2_Reactive_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}


float STPM::readApparentEnergy(uint8_t channel) {
  if (!_autoLatch) latch();
  if (channel != 1 && channel != 2) {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info:readApparentEnergy: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  uint8_t address = PH1_Apparent_Energy_Address;
  if (channel == 2) address = PH2_Apparent_RMS_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}


float STPM::readFundamentalEnergy(uint8_t channel) {
  if (!_autoLatch) latch();
  if (channel != 1 && channel != 2) {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info:readFundamentalEnergy: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  uint8_t address = PH1_Fundamental_Energy_Address;
  if (channel == 2) address = PH2_Fundamental_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}


float STPM::readActivePower(uint8_t channel) {
  if (channel == 1) { 
    address = PH1_Active_Power_Address;
  } else if (channel == 2) {
    address = PH2_Active_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return calcPower(buffer0to28(readBuffer))*_calibration[channel][_V]*_calibration[channel][_I];
}


float STPM::readFundamentalPower(uint8_t channel) {
  
  if (channel == 1) { 
    address = PH1_Fundamental_Power_Address;
  } else if (channel == 2) {
    address = PH2_Fundamental_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return (calcPower(buffer0to28(readBuffer)))*_calibration[channel][_V]*_calibration[channel][_I];
}


float STPM::readReactivePower(uint8_t channel) {
 
  if (channel == 1) { 
    address = PH1_Reactive_Power_Address;
  } else if (channel == 2) {
    address = PH2_Reactive_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return (calcPower(buffer0to28(readBuffer)))*_calibration[channel][_V]*_calibration[channel][_I];
}

float STPM::readApparentRMSPower(uint8_t channel) {
  
  if (channel == 1) { 
    address = PH1_Apparent_RMS_Power_Address;
  } else if (channel == 2) {
    address = PH2_Apparent_RMS_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return (calcPower(buffer0to28(readBuffer)))*_calibration[channel][_V]*_calibration[channel][_I];
}

float STPM::readApparentVectorialPower(uint8_t channel) {
  
  if (channel == 1) { 
    address = PH1_Apparent_Vectorial_Power_Address;
  } else if (channel == 2) {
    address = PH2_Apparent_Vectorial_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return (calcPower(buffer0to28(readBuffer)))*_calibration[channel][_V]*_calibration[channel][_I];
}


float STPM::readMomentaryActivePower(uint8_t channel) {

  if (channel == 1) { 
    address = PH1_Momentary_Active_Power_Address;
  } else if (channel == 2) {
    address = PH2_Momentary_Active_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return (calcPower(buffer0to28(readBuffer)))*_calibration[channel][_V]*_calibration[channel][_I];
}

float STPM::readMomentaryFundamentalPower(uint8_t channel) {
  
  if (channel == 1) { 
    address = PH1_Momentary_Fundamental_Power_Address;
  } else if (channel == 2) {
    address = PH2_Momentary_Fundamental_Power_Address;
  } else {
    #ifdef DEBUG_DEEP
    Serial.print(F("Info: Channel "));
    Serial.print(channel);
    Serial.println(F(" out of range"));
    #endif
    return -1;
  }
  if (!_autoLatch) latch();

  readFrame(address, readBuffer);
  return (calcPower(buffer0to28(readBuffer)))*_calibration[channel][_V]*_calibration[channel][_I];
}

float STPM::readTotalActiveEnergy() {
  if (!_autoLatch) latch();
  uint8_t address = Tot_Active_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}

float STPM::readTotalFundamentalEnergy() {
  if (!_autoLatch) latch();
  uint8_t address = Tot_Fundamental_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}

float STPM::readTotalReactiveEnergy() {
  if (!_autoLatch) latch();
  uint8_t address = Tot_Reactive_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}

float STPM::readTotalApparentEnergy() {
  if (!_autoLatch) latch();
  uint8_t address = Tot_Apparent_Energy_Address;
  readFrame(address, readBuffer);
  return (calcEnergy(buffer0to32(readBuffer)));
}
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*By m4RrrroooooooooooUuuuuuuuuuuuuuuueeeeeeeeeeeeeennnnnnnnnnnnnnnnn*/
/********************************************************************/
/**********************************************************************/
/**********************************************************************/
void STPM::latchReg() {
	latch();
}

inline void STPM::latch() {
#ifdef SPI_LATCH
  DSP_CR301bits.SW_Latch1 = 1;    
  DSP_CR301bits.SW_Latch2 = 1;      
  sendFrame(0x05, 0x05, DSP_CR301bits.LSB, DSP_CR301bits.MSB);// Send latch frame for SPI
#else
// For UART, send a latch signal over GPIO (or another method)
  digitalWrite(SYN_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(SYN_PIN, HIGH);
  delayMicroseconds(4);
#endif
}

inline float STPM::calcVolt (int32_t value) {
  return ((float)value/*-14837*/)*0.000138681; 
}
inline float STPM::calcVolt (int16_t value) {
  return ((float)value/*-56*/)* 0.0354840440;
}


inline float STPM::calcCurrent (int32_t value) {
  return (float)value * 0.003349213; //1000; // 0.0041875 old value from Benny
}
inline float STPM::calcCurrent (int16_t value) {
  return (float)value * 0.2143; //1000; // 0.26794 old value from Benny
}

inline float STPM::calcEnergy (int32_t value) {
  return (float)value * 0.00000000886162;
}

inline int32_t STPM::buffer0to32(uint8_t *buffer) {
  return (((buffer[3] << 24) | (buffer[2] << 16)) | (buffer[1] << 8)) | buffer[0];
}

inline int16_t STPM::buffer0to14(uint8_t *buffer) {
  return (buffer[1]&0x7f << 8) | buffer[0];

}

inline int32_t STPM::buffer0to28(uint8_t *buffer) {
  return (((buffer[3] << 24) | (buffer[2] << 16)) | (buffer[1] << 8)) | buffer[0];
}

inline float STPM::calcPower (int32_t value) {
  return value * 0.0001217; 
}

void STPM::readFrame(uint8_t address, uint8_t *buffer) {
  digitalWrite(CS_PIN, LOW);
  _uart->write(address);   
  _uart->write(0xff);      
  _uart->write(0xff);      
  _uart->write(0xff);      
  delay(5);                
  for (uint8_t i = 0; i < 4; i++) {
    buffer[i] = _uart->read();    
    while (_uart->available() == 0) {  
      delay(1);                     
    }
  }
  digitalWrite(CS_PIN, HIGH);
}



void STPM::sendFrame(uint8_t readAddr, uint8_t writeAddr, uint8_t dataLSB, uint8_t dataMSB) {
  digitalWrite(CS_PIN, LOW); 
  _uart->write(readAddr);  
  _uart->write(writeAddr);
  _uart->write(dataLSB);
  _uart->write(dataMSB);
  digitalWrite(CS_PIN, HIGH);
}



void STPM::sendFrameCRC(uint8_t readAdd, uint8_t writeAdd, uint8_t dataLSB, uint8_t dataMSB) {
  uint8_t frame[STPM3x_FRAME_LEN];
  frame[0] = readAdd;
  frame[1] = writeAdd;
  frame[2] = dataLSB;
  frame[3] = dataMSB;
  frame[4] = CalcCRC8(frame);  // Calculate the CRC
  #ifdef DEBUG_DEEP
  printRegister(frame, "Sending:");
  #endif
  for (uint8_t i = 0; i < STPM3x_FRAME_LEN; i++) {
    _uart->write(frame[i]);   // Send each byte of the frame
  }
}


void STPM::printFrame(uint8_t *frame, uint8_t length) {
  char buffer[4];
  Serial.print(F("Info:"));
  for (uint8_t i = 0; i < length; i++) {
    snprintf(buffer, 4, "%02x", frame[i]);
    Serial.print(F("|"));
    Serial.print(buffer);
  }
  Serial.println(F("|"));
}

void STPM::printRegister(uint8_t *frame, const char* regName) {
  Serial.print(F("Info:"));
  Serial.println(regName);
  Serial.print(F("Info:"));
  Serial.print(F("|"));
  for (int8_t i = 31; i >= 0; i--) {
    Serial.print(i);
    Serial.print(F("|"));
    if (i <= 10) Serial.print(" ");
  }
  Serial.println("");
  Serial.print(F("Info:"));
  Serial.print(F("| "));
  for (int8_t i = 3; i >= 0; i--) {
    Serial.print((frame[i] & 0x80) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x40) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x20) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x10) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x08) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x04) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x02) > 0);Serial.print(F("| "));
    Serial.print((frame[i] & 0x01) > 0);Serial.print(F("| "));
  }
  Serial.println("");
}

uint8_t STPM::CalcCRC8(uint8_t *pBuf) {
  uint8_t i;
  CRC_u8Checksum = 0x00;
  for (i = 0; i < STPM3x_FRAME_LEN - 1; i++) {
    Crc8Calc(pBuf[i]);
  }
  return CRC_u8Checksum;
}

void STPM::Crc8Calc(uint8_t u8Data) {
  uint8_t loc_u8Idx;
  uint8_t loc_u8Temp;
  loc_u8Idx = 0;
  while (loc_u8Idx < 8) {
    loc_u8Temp = u8Data ^ CRC_u8Checksum;
    CRC_u8Checksum <<= 1;
    if (loc_u8Temp & 0x80) {
      CRC_u8Checksum ^= CRC_8;
    }
    u8Data <<= 1;
    loc_u8Idx++;
  }
}
