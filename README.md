# EEPROM_UID Library

The `EEPROM_UID` library provides an interface to interact with the [24AA02xUID EEPROM](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/20005202A.pdf). This library supports reading and writing data, retrieving unique identifiers (UIDs) of various lengths, and handling errors during interactions with the EEPROM.

## Features

- Retrieve fixed and dynamic-length unique device IDs (UIDs) (32-bit, 48-bit, 64-bit, 128-bit, and 256-bit).
- Automatic caching of 32-bit UID during initialization.
- Validate UID length requests based on the specific chip configuration.
- Read and write data to/from the EEPROM.
- Handle errors with descriptive error messages.
- Optimized for the 24AA025UID series with a maximum storage size of 2 Kbits.

---

## Installation

To use the `EEPROM_UID` library, copy the `EEPROM_UID` folder into your `lib` directory in your PlatformIO project.

For `PlatformIO` users, configure the following `build_flags` in your `platformio.ini`:

```ini
build_flags =
    -Ilib/EEPROM_UID/include
```

## Usage

Include the Library
To use the library in your sketch, include the following headers:

```cpp
#include <EEPROM_UID.h>
#include <Wire.h>
```

Initialize the Library
Create an instance of the EEPROM_UID class and initialize it in your setup function:

```cpp
TwoWire *i2c = &Wire;
EEPROM_UID eUID(i2c);

void setup() {
  Serial.begin(115200);
  i2c->begin(); // Initialize I2C communication
  eUID.begin(); // Initialize the EEPROM and fetch the 32-bit UID
}
```

## Retrieve the UID

Retrieve a 32-bit UID (Default)
The 32-bit UID is cached during initialization and can be retrieved directly:

```cpp
uint32_t uid = eUID.getStoredUID();
Serial.print("Cached 32-bit UID: 0x");
Serial.println(uid, HEX);
```

## Retrieve a UID of Custom Length

Retrieve a UID of any length (e.g., 128-bit) as a hexadecimal string:

```cpp
char uid128[129]; // Buffer large enough for 128-bit UID (32 bytes * 2 characters + null terminator)
eUID.getUID(uid128, sizeof(uid128), UID_128bit);
Serial.print("128-bit UID: ");
Serial.println(uid128);
```

## Read Data

Read data from a specific EEPROM address into a buffer:

```cpp
uint8_t dataBuffer[16]; // Buffer to store the read data
uint8_t bytesRead = eUID.readData(dataBuffer, 0x00, sizeof(dataBuffer));

if (!eUID.hasError()) {

  Serial.print("Data read from EEPROM: ");

  for (uint8_t i = 0; i < bytesRead; i++) {
    Serial.printf("%02X ", dataBuffer[i]); // Print each byte as hexadecimal
  }

  Serial.println();

} else {

  Serial.print("Error: ");
  Serial.println(eUID.getErrorMessage());

}
```

## Write Data

Write data to a specific EEPROM address:

```cpp
uint8_t dataBuffer[16] = {
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};
uint8_t bytesWritten = eUID.writeData(dataBuffer, 0x00, sizeof(dataBuffer));

if (!eUID.hasError()) {

  Serial.print("Bytes written to EEPROM: ");
  Serial.println(bytesWritten);

} else {

  Serial.print("Error: ");
  Serial.println(eUID.getErrorMessage());

}
```

## Error Handling

Errors during EEPROM operations (e.g., failed I2C communication) are flagged, and descriptive error messages can be retrieved:

```cpp
if (eUID.hasError()) {

  Serial.print("Error: ");
  Serial.println(eUID.getErrorMessage());

}
```

## Retrieve EEPROM Size

Get the storage size of the EEPROM as a string:

```cpp
const char *size = eUID.getEEPROMSize();
Serial.print("EEPROM Size: ");
Serial.println(size);
``

## Example Sketch

The following demonstrates the library''s full functionality:

```cpp
#include <EEPROM_UID.h>
#include <Wire.h>

TwoWire *i2c = &Wire;
EEPROM_UID eUID(i2c);

void setup() {
  Serial.begin(115200);
  i2c->begin();
  eUID.begin();

  // Retrieve and display the cached 32-bit UID
  uint32_t uid = eUID.getStoredUID();
  Serial.print("32-bit UID: 0x");
  Serial.println(uid, HEX);

  // Retrieve and display a 128-bit UID
  char uid128[129];
  eUID.getUID(uid128, sizeof(uid128), UID_128bit);
  Serial.print("128-bit UID: ");
  Serial.println(uid128);

  // Read data from EEPROM
  uint8_t dataBuffer[16];
  uint8_t bytesRead = eUID.readData(dataBuffer, 0x00, sizeof(dataBuffer));
  Serial.print("Data read from EEPROM: ");
  for (uint8_t i = 0; i < bytesRead; i++) {
    Serial.printf("%02X ", dataBuffer[i]);
  }
  Serial.println();

  // Write data to EEPROM
  uint8_t writeBuffer[16] = {
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
    0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0xFF
  };
  eUID.writeData(writeBuffer, 0x00, sizeof(writeBuffer));
  Serial.println("Data written to EEPROM.");
}

void loop() {
  // Empty loop for this example
}
```

## License

This library is licensed under the MIT License.

## Author

Created by dzalf, this library is loosely based on the 24AA025UID library by Stephen Minakian from [Healthsmart](https://github.com/Healthsmart/24AA025UID_library).

## #TODOs

[x] Test compatibility with other EEPROM UID sizes and models. (_under development_)

[ ] Validate edge cases

[ ] Extend functionality to support multi-page write operations.

[ ] Enhance documentation for advanced use cases.

[ ] Buy beer
