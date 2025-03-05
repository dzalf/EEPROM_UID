/**
 * @file Read_UID.cpp
 * @author dzalf (github.com/dzalf)
 * @brief Example code to retrieve the UID from the 24AA025UID EEPROM
 * @version 0.1
 * @date 2025-03-03
 *
 * Note: Example code and library loosely based on the 24AA025E48 EEPROM library by Stephen Minakian
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "EEPROM_UID.h"
#include "Wire.h"

//* The address is 0x50 when A0-A2 = GND

TwoWire *i2c = &Wire;

uint32_t deviceID;

EEPROM_UID eUID(i2c);

void retrieveUID();
void printBanner();

void setup()
{

  Serial.begin(115200);

  delay(2000);

  Serial.println(__FILENAME__);

  printBanner();

  i2c->begin();

  eUID.begin();

  delay(2000);

  retrieveUID();

  if (eUID.hasError()) {
    Serial.print("Error: ");
    Serial.println(eUID.getErrorMessage());
  }
}

void loop()
{
}

void retrieveUID()
{

  uint8_t data[6];

  Serial.println(F("Method 1. 48-bits: Manual reading of Device ID (using readData)..."));
  eUID.readData(data, 0xfa, 6); // Reads the UID Manually

  Serial.print("Unique Device ID: 0x");

  for (uint8_t pos = 0; pos < 6; pos++)
  {
    Serial.print(String(data[pos], HEX));
  }

  Serial.print("\n");

  // Serial.println(String(eUID.getUID(), HEX));  // Reads the UID obtained from eUID.begin();

  Serial.println(F("Method 2. 32-bits: Automatic read from begin()..."));

  deviceID = eUID.getUID();

  Serial.printf("Unique Device ID: 0x%08X\n", deviceID);
}

void printBanner()
{
  Serial.println(F("======================================"));
  Serial.println(F("   EEPROM UID CHIP TESTING UTILITY   "));
  Serial.println(F("======================================"));
  Serial.println(F("   Detecting and Reading EEPROM UID  "));
  Serial.println(F("   Please ensure the chip is wired   "));
  Serial.println(F("   correctly before proceeding...    "));
  Serial.println(F("======================================"));
  Serial.println();
}