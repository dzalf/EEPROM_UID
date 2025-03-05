/**
 * @file EEPROM_UID.h
 * @author dzalf (github.com/dzalf)
 * @brief Example code to retrieve the UID from the family of 24AA02xUID/24AA02xUID EEPROMs
 * @version 1.1.0
 * @date 2025-03-03
 *
 * Note: This ibrary is loosely based on the 24AA025E48 EEPROM library by Stephen Minakian
 *      (from [Healthsmart](https://github.com/Healthsmart/24AA025UID_library))
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef EEPROM_UID_H
#define EEPROM_UID_H

#include <Arduino.h>
#include <Wire.h>

#define LIBRARY_VERSION "0.1"

#define DEFAULT_ADDRESS 0x50

//* EEPROM chip definitions

// *** SELECT ONE ***

//* UID length extensible chips (up to 256 bits):
// #define EEPROM_24AA02UID
// #define EEPROM_24AA025UID

//* EUI-48 chips:
// #define EEPROM_24AA02E48
#define EEPROM_24AA025E48

//* EUI-64 chips:
// #define EEPROM_24AA02E64
// #define EEPROM_24AA025E64

//* EEPROM size in bytes
#define EEPROM_SIZE_BYTES 256

//* Page size in bytes
#if defined(EEPROM_24AA025UID) || defined(EEPROM_24AA025E48) || defined(EEPROM_24AA025E64)
#define MAX_WRITE_PAGE 16
#elif defined(EEPROM_24AA02UID) || defined(EEPROM_24AA02E48) || defined(EEPROM_24AA02E64)
#define MAX_WRITE_PAGE 8
#else
#error "EEPROM not defined"
#endif

//* Fixed or variable length UID/EID
#if defined(EEPROM_24AA02UID) || defined(EEPROM_24AA025UID)
#define EXTENSIBLE_LENGTH true
#elif defined(EEPROM_24AA02E48) || defined(EEPROM_24AA025E48) || defined(EEPROM_24AA02E64) || defined(EEPROM_24AA025E64)
#define EXTENSIBLE_LENGTH false
#if defined(EEPROM_24AA02E48) || defined(EEPROM_24AA025E48)
#define FIXED_LENGTH 48
#elif defined(EEPROM_24AA02E64) || defined(EEPROM_24AA025E64)
#define FIXED_LENGTH 64
#endif
#endif

//* Register Addresses
#define START_ADDRESS_32bit 0xFC
#define START_ADDRESS_48bit 0xFA
#define START_ADDRESS_64bit 0xF8
#define START_ADDRESS_128bit 0xF0
#define START_ADDRESS_256bit 0xE0

/**
 * @enum UIDLength
 * @brief Enumerator for unique identifier lengths
 */
enum UIDLength
{
  UID_32bit,
  UID_48bit,
  UID_64bit,
  UID_128bit,
  UID_256bit
};

/**
 * @class EEPROM_UID
 * @brief Class to handle EEPROM with UID functionality
 */
class EEPROM_UID
{
public:
  /**
   * @brief Constructor with address and wire object
   * @param address I2C address of the EEPROM
   * @param wire Pointer to TwoWire object
   */
  EEPROM_UID(uint8_t address, TwoWire *wire);

  /**
   * @brief Default constructor
   */
  EEPROM_UID();

  /**
   * @brief Constructor with wire object
   * @param wire Pointer to TwoWire object
   */
  EEPROM_UID(TwoWire *wire);

  /**
   * @brief Initialize the EEPROM
   */
  void begin();

  /**
   * @brief Read data from EEPROM
   * @param dataBuffer Pointer to data buffer
   * @param startAddress Starting address to read from
   * @param bytes Number of bytes to read
   * @return Number of bytes read
   */
  uint8_t readData(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes);

  /**
   * @brief Write data to EEPROM
   * @param dataBuffer Pointer to data buffer
   * @param startAddress Starting address to write to
   * @param bytes Number of bytes to write
   * @return Number of bytes written
   */
  uint8_t writeData(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes);

  /**
   * @brief Retrieve the unique identifier as a uint32_t value (32-bit default)
   * @param length UIDLength enum specifying the UID size (default is UID_32bit)
   * @return uint32_t value containing the UID
   */
  uint32_t getUID(UIDLength length = UID_32bit);

  /**
   * @brief Retrieve the unique identifier as a C-style string
   * @param uidBuffer Pointer to the character array to store the UID
   * @param bufferSize Size of the provided character array
   * @param length UIDLength enum specifying the UID size (default is UID_32bit)
   */
  void getUID(char *uidBuffer, size_t bufferSize, UIDLength length = UID_32bit);

  /**
   * @brief Validate UID length for the EEPROM chip
   * @param length The requested UID length (enum UIDLength)
   * @return true if the length is valid, false otherwise
   */
  bool isValidLength(UIDLength length);

  /**
   * @brief Get the Last UID object
   * @return const char*
   */
  inline const char *getLastUID() { return _lastUID; }

  /**
   * @brief Get the stored 32-bit UID fetched during initialization
   * @return Stored 32-bit UID as a uint32_t value
   */
  uint32_t getStoredUID() const;

  /**
   * @brief Check if there is an error
   * @return true if there is an error, false otherwise
   */
  bool hasError();

  /**
   * @brief Get error message
   * @return Error message string
   */
  const char *getErrorMessage();

  /**
   * @brief Get the size of the EEPROM in KB or MB
   * @return Size of the EEPROM as a string
   */
  const char *getEEPROMSize();

private:
  TwoWire *_wire;       ///< Pointer to TwoWire object
  uint8_t _wireAddress; ///< I2C address of the EEPROM
  uint32_t _storedUID;  // Cached 32-bit UID
  char _lastUID[65];    // Cached last-read UID in hex (max 256-bit + null)

  /**
   * @brief Read a byte from EEPROM
   * @param dataAddress Address to read from
   * @return Byte read from EEPROM
   */
  uint8_t readByte(uint8_t dataAddress);

  /**
   * @brief Read consecutive bytes from EEPROM
   * @param dataBuffer Pointer to data buffer
   * @param startAddress Starting address to read from
   * @param bytes Number of bytes to read
   * @return Number of bytes read
   */
  uint8_t readConsecutive(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes);

  /**
   * @brief Write a byte to EEPROM
   * @param dataAddress Address to write to
   * @param data Byte to write
   * @return 1 on success, 0 on failure
   */
  uint8_t writeByte(uint8_t dataAddress, uint8_t data);

  /**
   * @brief Write a page to EEPROM
   * @param dataBuffer Pointer to data buffer
   * @param startAddress Starting address to write to
   * @param bytes Number of bytes to write
   * @return Number of bytes written
   */
  uint8_t writePage(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes);

  /**
   * @brief Read UID from EEPROM
   * @return UID value
   */
  uint32_t readUID();

  bool _error;               ///< Error flag
  const char *_errorMessage; ///< Error message

  /**
   * @brief Set error message
   * @param errorMessage Error message string
   */
  void setError(const char *errorMessage);
};

#endif
