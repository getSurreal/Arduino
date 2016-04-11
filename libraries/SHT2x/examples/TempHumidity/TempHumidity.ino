#include "SHT2x.h"
#include "Wire.h"

SHT2x sht21;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("");
  Serial.println("SHT21 Example");
  Serial.println("");
}

void loop(){
  Serial.print("TemperatureC:  ");
  Serial.println(sht21.getTemperatureC());
  Serial.print("TemperatureF:  ");
  Serial.println(sht21.getTemperatureF());
  Serial.print("Humidity:      ");
  Serial.println(sht21.getHumidity());
  Serial.println("");
  delay(5000);
}