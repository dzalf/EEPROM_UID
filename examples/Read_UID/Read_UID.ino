/**
 * @file Read_UID.ino
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
 
   // Retrieve the UID
   retrieveUID();
 
   // Check for any errors
   if (eUID.hasError())
   {
     Serial.print("Error: ");
     Serial.println(eUID.getErrorMessage());
   }
 }
 
 void loop()
 {
 }
 
 void retrieveUID()
 {
   Serial.println(F("Retrieving Unique Device ID..."));
 
   // Retrieve the 32-bit UID cached during initialization
   uint32_t uid32 = eUID.getStoredUID();
   if (eUID.hasError())
   {
     Serial.print("Error while retrieving 32-bit UID: ");
     Serial.println(eUID.getErrorMessage());
     return;
   }
   Serial.printf("32-bit UID: 0x%08X\n", uid32);
 
   // Retrieve a 48-bit UID as a string
   char uid48[49]; // Ensure enough buffer size for 48-bit UID
   eUID.getUID(uid48, sizeof(uid48), UID_48bit);
   if (eUID.hasError())
   {
     Serial.print("Error while retrieving 48-bit UID: ");
     Serial.println(eUID.getErrorMessage());
     return;
   }
   Serial.print(F("48-bit UID: "));
   Serial.println(uid48);
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