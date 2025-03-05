/**
 * @file EEPROM_UID.cpp
 * @author dzalf (github.com/dzalf)
 * @brief
 * @version 0.1
 * @date 2025-03-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "EEPROM_UID.h"

/* Public Functions */

//* Constructors

EEPROM_UID::EEPROM_UID(uint8_t address, TwoWire *wire)
    : _wireAddress(address), _wire(wire), _error(false), _errorMessage(""), _storedUID(0)
{
  // Initialize _storedUID to 0 and clear the lastUID buffer
  _lastUID[0] = '\0';
}

EEPROM_UID::EEPROM_UID()
    : EEPROM_UID(DEFAULT_ADDRESS, &Wire) {}

EEPROM_UID::EEPROM_UID(TwoWire *wire)
    : EEPROM_UID(DEFAULT_ADDRESS, wire) {}

void EEPROM_UID::begin()
{
  _wire->begin();
  _storedUID = getUID(UID_32bit); // Fetch 32-bit UID during initialization
}

uint32_t EEPROM_UID::getStoredUID() const
{
  // Return the cached 32-bit UID
  return _storedUID;
}

uint8_t EEPROM_UID::readData(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes)
{
  uint8_t readBytes = readConsecutive(dataBuffer, startAddress, bytes > 32 ? 32 : bytes);
  if (readBytes == 0)
  {
    setError("Failed to read data");
    return 0;
  }
  if (bytes > 32)
  {
    return readBytes + readData(dataBuffer + 32, startAddress + 32, bytes - 32);
  }
  return readBytes;
}

uint8_t EEPROM_UID::writeData(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes)
{
  uint8_t bytesOfPageRemaining = 0x20 - (startAddress % 0x20);
  bytesOfPageRemaining = bytesOfPageRemaining % 16;

  if (bytes > bytesOfPageRemaining && bytesOfPageRemaining != 0)
  {
    if (writePage(dataBuffer, startAddress, bytesOfPageRemaining) != 0)
    {
      setError("Failed to write data");
      return 0;
    }
    dataBuffer += bytesOfPageRemaining;
    startAddress += bytesOfPageRemaining;
    bytes -= bytesOfPageRemaining;
  }

  uint8_t writtenBytes = writePage(dataBuffer, startAddress, bytes > MAX_WRITE_PAGE ? MAX_WRITE_PAGE : bytes);
  if (writtenBytes == 0)
  {
    setError("Failed to write data");
    return 0;
  }
  if (bytes > MAX_WRITE_PAGE)
  {
    return writtenBytes + writeData(dataBuffer + MAX_WRITE_PAGE, startAddress + MAX_WRITE_PAGE, bytes - MAX_WRITE_PAGE);
  }
  return writtenBytes;
}

uint32_t EEPROM_UID::getUID(UIDLength length)
{
  uint8_t startAddress;
  uint8_t numBytes;

  // Set start address and bytes based on length
  switch (length)
  {
  case UID_32bit:
    startAddress = START_ADDRESS_32bit;
    numBytes = 4;
    break;
  case UID_48bit:
    startAddress = START_ADDRESS_48bit;
    numBytes = 6;
    break;
  case UID_64bit:
    startAddress = START_ADDRESS_64bit;
    numBytes = 8;
    break;
  case UID_128bit:
    startAddress = START_ADDRESS_128bit;
    numBytes = 16;
    break;
  case UID_256bit:
    startAddress = START_ADDRESS_256bit;
    numBytes = 32;
    break;
  default:
    return 0; // Invalid length
  }

  uint8_t dataBuffer[4]; // For uint32_t, we only need 4 bytes
  if (readConsecutive(dataBuffer, startAddress, numBytes) < numBytes)
  {
    return 0; // Failed to read full UID
  }

  // Combine bytes into a uint32_t (only works for UID_32bit)
  uint32_t uid = 0;
  for (uint8_t i = 0; i < 4 && i < numBytes; i++)
  {
    uid = (uid << 8) | dataBuffer[i];
  }

  return uid;
}

void EEPROM_UID::getUID(char *uidBuffer, size_t bufferSize, UIDLength length)
{

  uint8_t startAddress;
  uint8_t numBytes;

  // Set start address and bytes based on length
  switch (length)
  {
  case UID_32bit:
    startAddress = START_ADDRESS_32bit;
    numBytes = 4;
    break;
  case UID_48bit:
    startAddress = START_ADDRESS_48bit;
    numBytes = 6;
    break;
  case UID_64bit:
    startAddress = START_ADDRESS_64bit;
    numBytes = 8;
    break;
  case UID_128bit:
    startAddress = START_ADDRESS_128bit;
    numBytes = 16;
    break;
  case UID_256bit:
    startAddress = START_ADDRESS_256bit;
    numBytes = 32;
    break;
  default:
    uidBuffer[0] = '\0'; // Invalid length
    return;
  }

  if (bufferSize < numBytes * 2 + 1)
  {
    // Insufficient buffer size
    uidBuffer[0] = '\0';
    return;
  }

  uint8_t dataBuffer[32];
  if (readConsecutive(dataBuffer, startAddress, numBytes) < numBytes)
  {
    uidBuffer[0] = '\0'; // Failed to read full UID
    return;
  }

  // Convert each byte to hexadecimal and store it in uidBuffer
  for (uint8_t i = 0; i < numBytes; i++)
  {
    snprintf(&uidBuffer[i * 2], 3, "%02X", dataBuffer[i]);
  }
  uidBuffer[numBytes * 2] = '\0'; // Null-terminate
}

bool EEPROM_UID::hasError()
{
  return _error;
}

const char *EEPROM_UID::getErrorMessage()
{
  return _errorMessage;
}

const char *EEPROM_UID::getEEPROMSize()
{
  static char sizeStr[10];
  snprintf(sizeStr, sizeof(sizeStr), "%d bytes", EEPROM_SIZE_BYTES);
  return sizeStr;
}

void EEPROM_UID::setError(const char *errorMessage)
{
  _error = true;
  _errorMessage = errorMessage;
}

/* Private Functions */

uint8_t EEPROM_UID::readByte(uint8_t dataAddress)
{
  _wire->beginTransmission(_wireAddress);
  _wire->write(dataAddress);

  if (_wire->endTransmission(false) != 0)
  {
    setError("Failed to end transmission");
    return 0;
  }

  if (_wire->requestFrom((int)_wireAddress, (int)1) != 1)
  {
    setError("Failed to request data");
    return 0;
  }

  return _wire->read();
}

uint8_t EEPROM_UID::readConsecutive(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes)
{

  _wire->beginTransmission(_wireAddress);
  _wire->write(startAddress);

  if (_wire->endTransmission(false) != 0)
  {
    setError("Failed to end transmission");
    return 0;
  }

  if (_wire->requestFrom((int)_wireAddress, (int)bytes) != bytes)
  {
    setError("Failed to request data");
    return 0;
  }

  for (uint8_t pos = 0; pos < bytes; pos++)
  {
    dataBuffer[pos] = _wire->read();
  }

  return bytes;
}

uint8_t EEPROM_UID::writeByte(uint8_t dataAddress, uint8_t data)
{

  _wire->beginTransmission(_wireAddress);
  _wire->write(dataAddress);
  _wire->write(data);

  if (_wire->endTransmission() != 0)
  {
    setError("Failed to end transmission");
    return 0;
  }

  return 1; // Success
}

uint8_t EEPROM_UID::writePage(uint8_t *dataBuffer, uint8_t startAddress, uint8_t bytes)
{
  _wire->beginTransmission(_wireAddress);
  _wire->write(startAddress);
  _wire->write(dataBuffer, bytes);

  uint8_t status = _wire->endTransmission();
  if (status != 0)
  {
    setError("Failed to end transmission");
    return 0;
  }
  delay(5); // This is the only intentional piece of blocking code

  return bytes; // Success
}

uint32_t EEPROM_UID::readUID()
{

  return getUID(UID_32bit);
}
