/*********************



 **********************/

// include the library code:
#include <SPI.h>
#include <Wire.h>
#include "SHT2x.h"
#include <MySensor.h>  


SHT2x sht21;

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1


MySensor gw;
float last_temp;
float last_humid;
boolean metric = false; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
unsigned long SLEEP_TIME = 60000; // Sleep time between reads (in milliseconds)


void setup() {
  Wire.begin();
      gw.begin();
      gw.sendSketchInfo("RoomSensor", "0.1");
        // Register all sensors to gw (they will be created as child devices)
  gw.present(CHILD_ID_HUM, S_HUM);
  gw.present(CHILD_ID_TEMP, S_TEMP);
  
  metric = gw.getConfig().isMetric;
  
	// Debugging output
	Serial.begin(115200);
}

void loop() {
getTempHumid();
gw.send(msgTemp.set(last_temp, 1));
gw.send(msgHum.set(last_humid, 1));


//delay(5000);	
  gw.sleep(SLEEP_TIME); //sleep a bit
}


void getTempHumid() {
	last_temp = sht21.getTemperatureC();
	last_humid = sht21.getHumidity();
}


