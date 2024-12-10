//MarOuen Modifyyyyyyyyyy
#ifndef STPM_h
#define STPM_h


#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include "STPM3X_DEFINE.h"
// #define DEBUG_DEEP


#define STPM3x_FRAME_LEN 5
#define CRC_8 (0x07)
#define SPI_LATCH // Latching with SPI
#define _V 0
#define _I 1

enum Gain { twoX=0x00, fourX=0x01, eightX=0x02, sixteenX=0x03};

class STPM {
  public:
    STPM(int resetPin, int csPin, int synPin, HardwareSerial &uart);
    STPM(int resetPin, int csPin, HardwareSerial &uart);
    bool init();
    void setCurrentGain(uint8_t channel, Gain gain);
    bool checkGain(uint8_t channel, uint8_t *buffer);
    
#if defined(ESP32) or defined(ESP8266) 
    void IRAM_ATTR latchReg();
#else
    void latchReg();
#endif
    float readVoltage(uint8_t channel);
    float readRMSVoltage(uint8_t channel);
    float readCurrent(uint8_t channel);
    float readActiveEnergy(uint8_t channel);
    float readReactiveEnergy(uint8_t channel);
    float readApparentEnergy(uint8_t channel);
    float readFundamentalEnergy(uint8_t channel);
    float readActivePower(uint8_t channel);
    float readFundamentalPower(uint8_t channel);
    float readReactivePower(uint8_t channel);
    float readApparentRMSPower(uint8_t channel);
    float readApparentVectorialPower(uint8_t channel);
    float readMomentaryFundamentalPower(uint8_t channel);
    float readMomentaryActivePower(uint8_t channel);
    float readTotalActiveEnergy() ;
    float readTotalFundamentalEnergy();
    float readTotalReactiveEnergy();
    float readTotalApparentEnergy(); 
    void autoLatch(bool enabled);
    void CRC(bool enabled);

  private:
    bool Init_STPM34();
#if defined(ESP32) or defined(ESP8266) 
    void IRAM_ATTR readFrame(uint8_t address, uint8_t *buffer);
    void IRAM_ATTR sendFrame(uint8_t readAdd, uint8_t writeAdd, uint8_t dataLSB, uint8_t dataMSB);
#else
    void readFrame(uint8_t address, uint8_t *buffer);
    void sendFrame(uint8_t readAdd, uint8_t writeAdd, uint8_t dataLSB, uint8_t dataMSB);
#endif
    void sendFrameCRC(uint8_t readAdd, uint8_t writeAdd, uint8_t dataLSB, uint8_t dataMSB);
    void printFrame(uint8_t *frame, uint8_t length);
    void printRegister(uint8_t *frame, const char* regName);
    uint8_t CalcCRC8(uint8_t *pBuf);
    void Crc8Calc (uint8_t u8Data);

    inline float calcVolt (int16_t value);
    inline float calcVolt (int32_t value);
    inline float calcCurrent (int16_t value);
    inline float calcCurrent (int32_t value);
    inline float calcEnergy (int32_t value);
    inline float calcPower (int32_t value);
    inline int32_t buffer0to32(uint8_t *buffer);
    inline int16_t buffer0to14(uint8_t *buffer);
    inline int32_t buffer0to28(uint8_t *buffer);
    inline void latch();
    float _calibration[3][2];
    int RESET_PIN;
    int CS_PIN;
    int SYN_PIN;
    uint8_t CRC_u8Checksum;
    uint8_t address;
    bool _autoLatch;
    bool _crcEnabled;
    Gain _gain1;
    Gain _gain2;
    uint8_t readBuffer[10];
    HardwareSerial* _uart;
};

#endif
