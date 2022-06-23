//
//    FILE: DS18B20_INT.cpp
//  AUTHOR: Rob.Tillaart@gmail.com
// VERSION: 0.1.8
//    DATE: 2017-07-25
// PUPROSE: library for DS18B20 temperature sensor - integer only.
//     URL: https://github.com/RobTillaart/DS18B20_INT
//          https://github.com/RobTillaart/DS18B20_RT
//
//  HISTORY:
//  0.1.0   2017-07-25  initial version
//  0.1.1   2019-
//  0.1.2   2020-08-05  refactor / sync with DS18B20
//  0.1.3   2020-12-20  add Arduino-CI + unit test
//  0.1.4   2021-05-26  add OneWire.reset() to begin()
//  0.1.5   2021-06-16  add retries parameter to begin()
//  0.1.6   2021-10-03  add dependency + fix build-CI
//  0.1.7   2021-12-17  update library.json, license, minor edits
//  0.1.8   2022-06-23  fix #10 getTempCentiC() (thanks negroKiordi)


#include "DS18B20_INT.h"

// OneWire commands
#define STARTCONVO              0x44
#define READSCRATCH             0xBE
#define WRITESCRATCH            0x4E


// Device resolution
#define TEMP_9_BIT              0x1F    //  9 bit
#define TEMP_10_BIT             0x3F    // 10 bit
#define TEMP_11_BIT             0x5F    // 11 bit
#define TEMP_12_BIT             0x7F    // 12 bit


DS18B20_INT::DS18B20_INT(OneWire* ow)
{
  _oneWire      = ow;
  _addressFound = false;
  _resolution   = TEMP_9_BIT;
}


bool DS18B20_INT::begin(uint8_t retries)
{
  _addressFound = false;
  for (uint8_t rtr = retries; (rtr > 0) && (_addressFound == false); rtr--)
  {
    _oneWire->reset();
    _oneWire->reset_search();
    _deviceAddress[0] = 0x00;
    _oneWire->search(_deviceAddress);
    _addressFound = _deviceAddress[0] != 0x00 &&
                _oneWire->crc8(_deviceAddress, 7) == _deviceAddress[7];
  }

  if (_addressFound)
  {
    _oneWire->reset();
    _oneWire->select(_deviceAddress);
    _oneWire->write(WRITESCRATCH);
    // two dummy values for LOW & HIGH ALARM
    _oneWire->write(0);
    _oneWire->write(100);
    _oneWire->write(_resolution);     // lowest as we do only integer math.
    _oneWire->reset();
  }
  return _addressFound;
}


void DS18B20_INT::requestTemperatures(void)
{
  _oneWire->reset();
  _oneWire->skip();
  _oneWire->write(STARTCONVO, 0);
}


bool DS18B20_INT::isConversionComplete(void)
{
  return (_oneWire->read_bit() == 1);
}


int16_t DS18B20_INT::getTempC(void)
{
  int16_t rawTemperature = _readRaw();
  rawTemperature >>= 4;
  if (rawTemperature < -55) return DEVICE_DISCONNECTED;
  return rawTemperature;
}


bool  DS18B20_INT::getAddress(uint8_t* buf)
{
  if (_addressFound)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      buf[i] = _deviceAddress[i];
    }
  }
  return _addressFound;
}


void DS18B20_INT::setResolution(uint8_t bits)
{
  uint8_t newRes = 0;
  switch (bits)
  {
    case 12: newRes = TEMP_12_BIT;  break;
    case 11: newRes = TEMP_11_BIT;  break;
    case 10: newRes = TEMP_10_BIT;  break;
    default: newRes = TEMP_9_BIT;   break;
  }
  if (newRes != _resolution)
  {
    _resolution = newRes;
    begin();
  }
}


uint8_t DS18B20_INT::getResolution()
{
  switch (_resolution)
  {
    case TEMP_12_BIT: return 12;
    case TEMP_11_BIT: return 11;
    case TEMP_10_BIT: return 10;
    case TEMP_9_BIT:  return 9;
  }
  return 0;
}


int16_t DS18B20_INT::getTempCentiC(void)
{
  int16_t rawTemperature = _readRaw();
  //  rawTemperature = rawTemperature * 100 / 16;
  rawTemperature *= 25;
  rawTemperature >>= 2;
  //  use at own risk.
  //  if (rawTemperature < -5500) return DEVICE_DISCONNECTED * 100;
  return rawTemperature;
}


//////////////////////////////////////////////////
//
//  PRIVATE
//
int16_t DS18B20_INT::_readRaw(void)
{
  _oneWire->reset();
  _oneWire->select(_deviceAddress);
  _oneWire->write(READSCRATCH);
  int16_t rawTemperature = ((int16_t)_oneWire->read()) << 8;
  rawTemperature |= _oneWire->read();
  _oneWire->reset();
  return rawTemperature;
}




//  -- END OF FILE --

