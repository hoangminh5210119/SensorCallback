#ifndef SensorLib_h
#define SensorLib_h

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#define DHTPIN D2
#define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);
#define SEALEVELPRESSURE_HPA (1013.25)

enum type_sensor {
    BME280_SENSOR,
    SHT21_SENSOR
};

enum sensor {
    none,
    SI7021_i2c,
    SHT30_i2c = 68,//0x44
    SHT21_i2c = 64,//0x40
    HTU21_i2c,
    SHT10_i2c,
    BME280_i2c_1 = 118, //0x76
    BME280_i2c_2 = 119 //0x77
};

struct SensorLibData {
  type_sensor type;
  float temp;
  float hum;
  float pressure;
  float altitude;
};

#define SENSOR_CALLBACK std::function<void(SensorLibData)> callback

class SensorLib {
private:
  bool isSensor = false;
  bool _initSensor = true;
  bool _isReadyReadSensor = false;
  unsigned int _setTimeOut = 3000;
  unsigned long millisTime = millis();
  int scanI2C();
  int scanOneWire();
  SENSOR_CALLBACK;
  SensorLibData read_BME280();
  SensorLibData read_SHT21();

public:
  SensorLib();
  void loop();
  SensorLib &setCallback(SENSOR_CALLBACK);
  void setTimeOut(unsigned int setTimeOut);
};

#endif