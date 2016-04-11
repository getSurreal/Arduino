/*********************



 **********************/

// include the library code:
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include "SHT2x.h"
#include <MySensor.h>
#include <SimpleTimer.h>
#ifdef DEBUG
#include <MemoryFree.h>
#endif

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
#define OFF 0x0

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
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
	lcd.begin(16, 2);

	gw.begin();
	gw.sendSketchInfo("RoomSensor", "0.1");
	// Register all sensors to gw (they will be created as child devices)
	gw.present(CHILD_ID_HUM, S_HUM);
	gw.present(CHILD_ID_TEMP, S_TEMP);

	metric = gw.getConfig().isMetric;

	lcd.setBacklight(WHITE);
}

void loop() {
	getTempHum();
	gw.send(msgTemp.set(last_temp, 1));
	gw.send(msgHum.set(last_hum, 1));
#ifdef DEBUG
	Serial.print(F("Free Memory: "));
	Serial.println(freeMemory());
#endif

	lcd.clear();
	showTempHum();
	uint8_t buttons = lcd.readButtons();

	if (buttons) {

		if (buttons & BUTTON_UP) {


		}
		if (buttons & BUTTON_DOWN) {
			lcd.clear();

		}
		if (buttons & BUTTON_LEFT) {

		}
		if (buttons & BUTTON_RIGHT) {

		}
		if (buttons & BUTTON_SELECT) {

		}
	}
	gw.sleep(SLEEP_TIME); //sleep a bit
}


void getTempHum() {
	last_temp = sht21.getTemperatureC();
	last_hum = sht21.getHumidity();
}

void showTempHum() {

	lcd.setCursor(0,0);
	lcd.print(F("Temp      "));
	lcd.print(last_temp,1);
	lcd.print(F(" F"));
	lcd.setCursor(0,1);
	lcd.print(F("Humidity  "));
	lcd.print(last_hum,1);
	lcd.print(F(" %"));
}
