#include "SensorLib.h"
#include <SHT21.h>

Adafruit_BME280 bme;

SHT21 sht;

SensorLib::SensorLib() {
  Wire.begin();
  setCallback(NULL);
}

SensorLib &SensorLib::setCallback(SENSOR_CALLBACK) {
  this->callback = callback;
  return *this;
}

void SensorLib::loop() {

  if (millis() - millisTime > _setTimeOut) {
    int device = scanI2C();
    millisTime = millis();
    if (device == BME280_i2c_1 || device == BME280_i2c_2) {
      Serial.println("BME280_i2c");
      SensorLibData data = read_BME280();
      callback(data);
    } else if (device == SHT21_i2c) {
      Serial.println("SHT21_i2c");
      SensorLibData data = read_SHT21();
      callback(data);
    } else {
      _initSensor = true;
      _isReadyReadSensor = false;
    }
  }
}

int SensorLib::scanI2C() {
  byte error, address;
  int nDevices;
  byte addrDevices;
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      nDevices++;
      addrDevices = address;
    }
  }
  if (nDevices == 0) {
    return none;
  } else {
    return addrDevices;
  }
}

int SensorLib::scanOneWire() {}

SensorLibData SensorLib::read_BME280() {
  SensorLibData data;
  if (_initSensor) {
    Wire.begin();
    unsigned status;
    status = bme.begin();
    if (!status) {
      _initSensor = true;
      _isReadyReadSensor = false;
    } else {
      _initSensor = false;
      _isReadyReadSensor = true;
    }
  }

  if (_isReadyReadSensor) {
    float temp = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F;
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    float humidity = bme.readHumidity();
    data.type = BME280_SENSOR;
    data.temp = temp;
    data.pressure = pressure;
    data.altitude = altitude;
    data.hum = humidity;
    return data;
  }
  return data;
}
SensorLibData SensorLib::read_SHT21() {
  SensorLibData data;
  if (_initSensor) {
    Wire.begin();
    sht.begin();
    _initSensor = false;
    _isReadyReadSensor = true;
  }

  if (_isReadyReadSensor) {

    float temp = sht.getTemperature();
    float humidity = sht.getHumidity();
    data.type = SHT21_SENSOR;
    data.temp = temp;
    data.pressure = 0;
    data.altitude = 0;
    data.hum = humidity;
    return data;
  }
  return data;
}

void SensorLib::setTimeOut(unsigned int setTimeOut) {
  _setTimeOut = setTimeOut;
}