#include "Arduino.h"
#include "SensorLib.h"

SensorLib sensor;

void callback(SensorLibData data) {
  Serial.print("type = ");
  Serial.println(data.type);

  Serial.print("Temperature = ");
  Serial.print(data.temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");

  Serial.print(data.pressure);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(data.altitude);
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(data.hum);
  Serial.println(" %");

  Serial.println();
}

void setup() {
  Serial.begin(9600);
  sensor.setCallback(callback);
  sensor.setTimeOut(1000);
}

void loop() {
  sensor.loop();
}
