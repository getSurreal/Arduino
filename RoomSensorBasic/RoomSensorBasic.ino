/*********************



 **********************/

// include the library code:
#include <SPI.h>
#include <Wire.h>
#include "SHT2x.h"
#include <MySensor.h>
#include <SimpleTimer.h>
#ifdef DEBUG
#include <MemoryFree.h>
#endif

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1

SHT2x sht21;

MySensor gw;
float last_temp;
float last_hum;
boolean metric = false; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
unsigned long SLEEP_TIME = 60000; // Sleep time between reads (in milliseconds)


void setup() {
	Serial.begin(115200);
	Wire.begin();

	gw.begin();
	gw.sendSketchInfo("RoomSensor", "0.1");
	// Register all sensors to gw (they will be created as child devices)
	gw.present(CHILD_ID_HUM, S_HUM);
	gw.present(CHILD_ID_TEMP, S_TEMP);

	metric = gw.getConfig().isMetric;

}

void loop() {
	getTempHum();
	gw.send(msgTemp.set(last_temp, 1));
	gw.send(msgHum.set(last_hum, 1));
#ifdef DEBUG
	Serial.print(F("Free Memory: "));
	Serial.println(freeMemory());
#endif
        showTempHum();
        delay(SLEEP_TIME);
//	gw.sleep(SLEEP_TIME); //sleep a bit
}


void getTempHum() {
	last_temp = sht21.getTemperatureC();
	last_hum = sht21.getHumidity();
}

void showTempHum() {

	Serial.print(F("Temp      "));
	Serial.print(last_temp);
	Serial.println(F(" F"));
	Serial.print(F("Humidity  "));
	Serial.print(last_hum);
	Serial.println(F(" %"));
}
