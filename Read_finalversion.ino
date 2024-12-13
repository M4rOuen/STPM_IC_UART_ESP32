#include "STPM3X.h"

// Pins for STPM UART Connection
const int STPM_CS = 5;
const int STPM_SYN = 14;
const int STPM_RES = 12;

// Define the UART for communication (you can choose different UARTs like UART1 or UART2)
#define STPM_UART Serial2 // Using UART1, you can also use Serial2 if needed

// STPM Object
STPM stpm3x(STPM_RES, STPM_CS, STPM_SYN, STPM_UART);

unsigned long previousMillis = 0;  // Stores the last time we updated the readings
const long interval = 1000;         // Interval at which to update the readings (1000ms = 1 second)

void setup() {
  Serial.begin(115200);    // Debug serial output
  STPM_UART.begin(9600, SERIAL_8N1); // Set the baud rate for UART communication
  
  bool success = stpm3x.init();
  if (!success) Serial.println("STPM Init Failed");
}

void loop() {
  // Get the current time (in milliseconds since the program started)
  unsigned long currentMillis = millis();
  
  // Check if it's time to update the readings
  if (currentMillis - previousMillis >= interval) {
    // Save the last time you updated the readings
    previousMillis = currentMillis;

    // Read the values
    float voltage = stpm3x.readVoltage(1);
    float rmsvoltage = stpm3x.readRMSVoltage(1); 
    float current = stpm3x.readCurrent(1);
    float activeenergy = stpm3x.readActiveEnergy(1);
    float totalactiveenergy = stpm3x.readTotalActiveEnergy();
    float reactiveenergy = stpm3x.readReactiveEnergy(2);
    float totalreactiveenergy = stpm3x.readTotalReactiveEnergy();
    float readapparentenergy = stpm3x.readApparentEnergy(1);
    float readfundamentalenergy = stpm3x.readFundamentalEnergy(1);
    float totalfundamentalEnergy = stpm3x.readTotalFundamentalEnergy();
    float activepower = stpm3x.readActivePower(1);
    float fundamentalpower = stpm3x.readFundamentalPower(1);
    float reactivepower = stpm3x.readReactivePower(1);
    float apparentrmspower = stpm3x.readApparentRMSPower(1);
    float readApparentVectorialPower = stpm3x.readApparentVectorialPower(1);
    float momentaryactivepower = stpm3x.readMomentaryActivePower(1);
    float momentaryfundamentalpower = stpm3x.readMomentaryFundamentalPower(1);
    float totalapparentenergy = stpm3x.readTotalApparentEnergy();
    
    // Calculate Power Factor (PF)
    float powerFactor = 0.0;
    if (apparentrmspower != 0) {
      powerFactor = activepower / apparentrmspower;
    }

    Serial.printf("Voltage: %.2fV\n", voltage);
    Serial.printf("RMS Voltage: %.2fV\n", rmsvoltage);
    Serial.printf("Current: %.2fmA\n", current);
    Serial.printf("Active Energy: %.2fkWh\n", activeenergy);
    Serial.printf("Total Active Energy: %.2fkWh\n", totalactiveenergy);
    Serial.printf("Reactive Energy: %.2fJ\n", reactiveenergy);
    Serial.printf("Reactive Energy: %.2fJ\n", totalreactiveenergy);
    Serial.printf("Apparent Energy: %.2fJ\n", readapparentenergy);
    Serial.printf("Apparent Energy: %.2fJ\n", totalapparentenergy);
    Serial.printf("Fundamental Energy: %.2fJ\n", readfundamentalenergy);
    Serial.printf("Fundamental Energy: %.2fJ\n", totalfundamentalEnergy);
    Serial.printf("Active Power: %.2fW\n", activepower);
    Serial.printf("Fundamental Power: %.2fW\n", fundamentalpower);
    Serial.printf("Reactive Power: %.2fVAr\n", reactivepower);  
    Serial.printf("Apparent RMS Power: %.2fVA\n", apparentrmspower);  
    Serial.printf("Apparent Vectorial Power: %.2fVA\n", readApparentVectorialPower);  
    Serial.printf("Momentary Active Power: %.2fVA\n", momentaryactivepower);  
    Serial.printf("Momentary Fundamental Power: %.2fVA\n", momentaryfundamentalpower);
    Serial.printf("Power Factor: %.2f\n", powerFactor);
  }
}
