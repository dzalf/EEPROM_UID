
# EEPROM_UID Library

The `EEPROM_UID` library provides an interface to interact with the [24AA02xUID](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/20005202A.pdf) EEPROM, allowing you to read and write data, as well as retrieve the unique device ID (UID) from the EEPROM. The device has a max storage size of 2 Kbits

## Features

- Read data from the EEPROM
- Write data to the EEPROM
- Retrieve the unique device ID (UID) from the EEPROM
- Error handling with descriptive error messages

## Installation

To use the `EEPROM_UID` library in your project, copy the `EEPROM_UID` folder to your `lib` directory in your PlatformIO project.

For `PlatformIO` users, use the following `build_falgs` configuration:

```ini
build_flags = 
    -I/lib/EEPROM_UID/include
```

## Usage

### Include the Library

Include the `EEPROM_UID` library in your sketch:

```cpp
#include <EEPROM_UID.h>
#include <Wire.h>
```

### Initialize the Library

Create an instance of the EEPROM_UID class and initialize it:

```cpp
TwoWire *i2c = &Wire;
EEPROM_UID eUID(i2c);
```

```cpp
void setup() {
  Serial.begin(115200);
  i2c->begin();
  eUID.begin();
}
```

### Retrieve the UID

Retrieve the unique device ID (UID) from the EEPROM:

```cpp
uint32_t uid = eUID.getUID();
Serial.print("Unique Device ID: 0x");
Serial.println(uid, HEX);
```

### Read Data

Read data from the EEPROM:

```cpp
uint8_t dataBuffer[16];
uint8_t bytesRead = eUID.readData(dataBuffer, 0x00, sizeof(dataBuffer));
for (uint8_t i = 0; i < bytesRead; i++) {
  Serial.print(dataBuffer[i], HEX);
  Serial.print(" ");
}
Serial.println();
```

### Write Data

Write data to the EEPROM:

```cpp
uint8_t dataBuffer[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
uint8_t bytesWritten = eUID.writeData(dataBuffer, 0x00, sizeof(dataBuffer));
Serial.print("Bytes Written: ");
Serial.println(bytesWritten);
```

### Error Handling

Check for errors and retrieve error messages:

```cpp
if (eUID.hasError()) {
  Serial.print("Error: ");
  Serial.println(eUID.getErrorMessage());
}
```

### Example

An example sketch demonstrating the usage of the `EEPROM_UID` library can be found in the `examples/Read UID/Read_UID.cpp` file.

### License

This library is licensed under the MIT License. See the LICENSE file for more details.

### Author

This library was created by [dzalf](github.com/dzalf) and is _very loosely_ based on the 24AA025E48 EEPROM library by Stephen Minakian from [Healthsmart](https://github.com/Healthsmart/24AA025UID_library).

### #TODO

- [ ] Test library on more devices.
- [ ] Buy beer
