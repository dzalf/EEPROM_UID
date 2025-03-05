/**
 * @file eUID-full-showcase.cpp
 * @author dzalf (github.com/dzalf)
 * @brief Example code to showcase the libraries capabilities to retrieve 
 *         UID from the 24AA025UID EEPROM
 * @version 1.0
 * @date 2025-03-03
 *
 * @note Please ensure the EEPROM is properly connected via I2C.
 */


#include "EEPROM_UID.h"
#include "Wire.h"

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

    // Retrieve the 32-bit unique identifier
    uint32_t uid = eUID.getUID();
    if (eUID.hasError())
    {
        handleError();
    }
    else
    {
        Serial.print(F("Unique Device ID (32-bit): 0x"));
        Serial.println(uid, HEX);
    }

    // Retrieve the 128-bit UID as a hexadecimal string
    char uid128[129]; // Ensure the buffer is large enough for 128-bit UID
    eUID.getUID(uid128, sizeof(uid128), UID_128bit);

    if (eUID.hasError())
    {
        handleError();
    }
    else
    {
        Serial.print(F("Unique Device ID (128-bit): "));
        Serial.println(uid128);
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
