#pragma once
//
//    FILE: DS18B20_INT.h
//  AUTHOR: Rob.Tillaart@gmail.com
// VERSION: 0.3.0
//    DATE: 2017-07-25
// PUPROSE: Minimalistic library for DS18B20 temperature sensor
//          uses only integer math (no float to minimize footprint)
//     URL: https://github.com/RobTillaart/DS18B20_INT
//          https://github.com/RobTillaart/DS18B20_RT

//
//          BOTTOM VIEW
//
//                  PIN     MEANING
//           /---+
//          /  o |  1       GND
//          |  o |  2       DATA
//          \  o |  3       VCC
//           \---+
//


#define DS18B20_INT_LIB_VERSION       (F("0.3.0"))

#include "Arduino.h"
#include "OneWire.h"

//  Error Code
#define DEVICE_DISCONNECTED           -127


typedef uint8_t DeviceAddress[8];


class DS18B20_INT
{
public:
  explicit  DS18B20_INT(OneWire * ow);
  bool      begin(uint8_t retries = 3);
  bool      isConnected(uint8_t retries = 3);

  void      requestTemperatures(void);
  int16_t   getTempC(void);
  bool      isConversionComplete(void);
  bool      getAddress(uint8_t* buf);

  bool      setResolution(uint8_t resolution = 9);
  uint8_t   getResolution();  //  returns cached value

  int16_t   getTempCentiC(void);


private:
  DeviceAddress _deviceAddress;
  OneWire*      _oneWire;
  bool          _addressFound;

  uint8_t       _resolution;
  int16_t       _readRaw();
  void          _setResolution();
};


//  -- END OF FILE --

