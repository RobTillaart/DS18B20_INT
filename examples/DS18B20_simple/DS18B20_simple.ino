//
//    FILE: DS18B20_simple.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.0.1
// PURPOSE: equivalent of DallasTemperature library Simple
//
// HISTORY:
// 0.0.1 = 2021-06-16 initial version


#include <OneWire.h>
#include <DS18B20_INT.h>


#define ONE_WIRE_BUS 2

OneWire     oneWire(ONE_WIRE_BUS);
DS18B20_INT sensor(&oneWire);


void setup(void)
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("DS18B20_INT_LIB_VERSION: ");
  Serial.println(DS18B20_INT_LIB_VERSION);

  sensor.begin();
}


void loop(void)
{ 
  sensor.requestTemperatures();
  
  while (!sensor.isConversionComplete());  // wait until sensor is ready
  
  Serial.print("Temp: ");
  Serial.println(sensor.getTempC());
}

// -- END OF FILE --
