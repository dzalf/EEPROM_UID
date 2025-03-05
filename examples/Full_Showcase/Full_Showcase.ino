/**
 * @file eUID-full-showcase.ino
 * @author dzalf (github.com/dzalf)
 * @brief Example code to showcase the library's capabilities to retrieve
 *         UID from the 24AA025UID EEPROM.
 * @version 1.1
 * @date 2025-03-03
 *
 * @note Please ensure the EEPROM is properly connected via I2C.
 */

 #include "EEPROM_UID.h"
 #include "Wire.h"
 
 //* Uncomment the desired EEPROM chip configuration in EEPROM_UID.h
 
 // Instantiate the TwoWire object and EEPROM_UID library
 TwoWire *i2c = &Wire;
 EEPROM_UID eUID(i2c);
 
 // Function prototypes
 void printBanner();
 void retrieveUID();
 void readEEPROMData();
 void writeEEPROMData();
 void handleError();
 
 void setup()
 {
     Serial.begin(115200); // Start Serial communication
 
     delay(2000);
 
     Serial.println(__FILENAME__);
 
     printBanner();
 
     // Initialize I2C communication and EEPROM_UID library
     i2c->begin();
     eUID.begin();
 
     // Delay to ensure proper initialization
     delay(1000);
 
     // Showcase library functionality
     retrieveUID();
 
     // Retrieve and display the EEPROM size
     const char *size = eUID.getEEPROMSize();
     Serial.printf("EEPROM Size: %s\n", size);
 
     delay(100);
 
     // Read and write data from EEPROM
     readEEPROMData();
     delay(100);
     writeEEPROMData();
     delay(100);
     readEEPROMData();
 }
 
 void loop()
 {
     // Loop function is not used in this example
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
 
 void retrieveUID()
 {
     Serial.println(F("Retrieving UID..."));
 
     // Retrieve the cached 32-bit unique identifier
     uint32_t uid32 = eUID.getStoredUID();
     if (eUID.hasError())
     {
         handleError();
     }
     else
     {
         Serial.print(F("Cached Unique Device ID (32-bit): 0x"));
         Serial.println(uid32, HEX);
     }
 
     // Retrieve the 48-bit UID as a hexadecimal string
     char uid48[49]; // Ensure the buffer is large enough for 48-bit UID
     eUID.getUID(uid48, sizeof(uid48), UID_48bit);
 
     if (eUID.hasError())
     {
         handleError();
     }
     else
     {
         Serial.print(F("Unique Device ID (48-bit): "));
         Serial.println(uid48);
     }
 
     // Attempt to retrieve a 64-bit UID (based on chip capabilities)
     char uid64[65];
     eUID.getUID(uid64, sizeof(uid64), UID_64bit);
 
     if (eUID.hasError())
     {
         handleError(); // This error will occur if the chip does not support 64-bit UIDs
     }
     else
     {
         Serial.print(F("Unique Device ID (64-bit): "));
         Serial.println(uid64);
     }
 }
 
 void readEEPROMData()
 {
     Serial.println(F("Reading data from EEPROM..."));
 
     // Buffer to store the data read from the EEPROM
     uint8_t dataBuffer[16];
     uint8_t bytesRead = eUID.readData(dataBuffer, 0x00, sizeof(dataBuffer));
 
     if (eUID.hasError())
     {
         handleError();
     }
     else
     {
         Serial.print(F("Data read from EEPROM: "));
         for (uint8_t i = 0; i < bytesRead; i++)
         {
             Serial.printf("%02X ", dataBuffer[i]); // Display data as hexadecimal
         }
         Serial.println();
     }
 }
 
 void writeEEPROMData()
 {
     Serial.println(F("Writing data to EEPROM..."));
 
     // Example data to write to the EEPROM
     uint8_t dataBuffer[16] = {
         0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
         0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
 
     // Write data to the EEPROM starting at address 0x00
     uint8_t bytesWritten = eUID.writeData(dataBuffer, 0x00, sizeof(dataBuffer));
 
     if (eUID.hasError())
     {
         handleError();
     }
     else
     {
         Serial.print(F("Bytes successfully written: "));
         Serial.println(bytesWritten);
     }
 }
 
 void handleError()
 {
     Serial.print(F("Error encountered: "));
     Serial.println(eUID.getErrorMessage());
 }
 