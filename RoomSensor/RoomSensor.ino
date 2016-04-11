/*********************



 **********************/

// include the library code:
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include "SHT2x.h"
#include <SimpleTimer.h>
#include <Time.h>
#include <MemoryFree.h>
#include <MySensor.h>  

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
SHT2x sht21;
SimpleTimer timer;

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

MySensor gw;
float last_temp;
float last_humid;
boolean metric = false; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

int pir_pin = 15;
int pir_state = HIGH;
int chk_motion_id;
int display_off_id;
int get_temp_humid_id;
int show_temp_humid_id;
int show_clock_id;

void setup() {
	gw.begin();
	gw.sendSketchInfo("RoomSensor", "0.1");
	// Register all sensors to gw (they will be created as child devices)
	gw.present(CHILD_ID_HUM, S_HUM);
	gw.present(CHILD_ID_TEMP, S_TEMP);

	metric = gw.getConfig().isMetric;

	// Debugging output
	//Serial.begin(115200);
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);
	pinMode(pir_pin, INPUT);
	
	chk_motion_id = timer.setInterval(100, checkMotion);
	display_off_id = timer.setInterval(30000, displayOff);
	get_temp_humid_id = timer.setInterval(60000, getTempHumid);
	show_temp_humid_id = timer.setInterval(1000, showTempHumid);
	show_clock_id = timer.setInterval(1000, digitalClockDisplay);
	timer.disable(show_clock_id);
	setTime(1357041600);
}

void loop() {
	timer.run();
	uint8_t buttons = lcd.readButtons();

	if (buttons) {

		if (buttons & BUTTON_UP) {
			lcd.clear();
			timer.enable(show_temp_humid_id);
			timer.disable(show_clock_id);
		}
		if (buttons & BUTTON_DOWN) {
			lcd.clear();
			timer.disable(show_temp_humid_id);
			timer.enable(show_clock_id);
		}
		if (buttons & BUTTON_LEFT) {

		}
		if (buttons & BUTTON_RIGHT) {

		}
		if (buttons & BUTTON_SELECT) {

		}
	}
	//gw.sleep(SLEEP_TIME); //sleep a bit
}

void checkMotion() {
	pir_state = digitalRead(pir_pin);
	if (pir_state == HIGH) {
		timer.restartTimer(display_off_id);
		lcd.setBacklight(BLUE);
		//Serial.println("motion");
	}
}

void getTempHumid() {
	Serial.print(F("Free Memory:"));
	Serial.println(freeMemory());
	last_temp = sht21.getTemperatureC();
	last_humid = sht21.getHumidity();
	if(last_temp < -20 or last_temp > 200 or last_humid < 0 or last_humid > 100) return;
	gw.send(msgTemp.set(last_temp, 1));
	gw.send(msgHum.set(last_humid, 1));
}

void displayOff() {
	lcd.setBacklight(OFF);
	lcd.clear();
}

void showTempHumid() {
	lcd.setCursor(0,0);
	lcd.print(F("Temp      "));
	lcd.print(last_temp,1);
	lcd.print(F(" F"));
	lcd.setCursor(0,1);
	lcd.print(F("Humidity  "));
	lcd.print(last_humid,1);
	lcd.print(F(" %"));
}

void digitalClockDisplay() {
	lcd.setCursor(0, 0);
	printDigits(day());
	lcd.print(F("/"));
	printDigits(month());
	lcd.print(F("/"));
	lcd.print(year());

	lcd.setCursor(0, 1);
	lcd.print(hour());
	lcd.print(F(":"));
	printDigits(minute());
	lcd.print(F(":"));
	printDigits(second());
}

void printDigits(int digits) {
	// utility function for digital clock display: prints leading 0	
	if (digits < 10)
		lcd.print(F("0"));
	lcd.print(digits);
}
