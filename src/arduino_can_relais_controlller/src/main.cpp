#include <Arduino.h>

#include "BMP280.h"
#define P0 1013.25
BMP280 bmp;


void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // get temperature from sensor
  Serial.print("Current temp: ");

  Serial.println(" C");

  // shutdown the sensor and wait a while


  Serial.println();
}
