#include <SPI.h>
#include <MySensor.h>
#include <Ping.h>

#define CHILD_ID 1
#define ECHO_PIN     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
// The maximum rated measuring range for the HC-SR04 is about 400-500cm.
#define MAX_DISTANCE 200 // Max distance we want to start indicating green (in cm)
#define PANIC_DISTANCE 60 // Mix distance we red warning indication should be active (in cm)
#define PARKED_DISTANCE 200 // Distance when "parked signal" should be sent to controller (in cm)
unsigned long SLEEP_TIME = 5000; // Sleep time between reads (in milliseconds)

Ping ping = Ping(ECHO_PIN);
MySensor gw;
MyMessage msg(CHILD_ID, V_TRIPPED);
int lastDist;
int oldParkedStatus=-1;
boolean metric = true; 

void setup()  
{ 
  gw.begin();

  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("Parking Sensor", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  gw.present(CHILD_ID, S_DOOR, "Parking Status");
  boolean metric = gw.getConfig().isMetric;
}

void loop()      
{     
  ping.fire();
  int dist = metric?ping.centimeters():ping.inches();
  Serial.print("Ping: ");
  Serial.print(dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(metric?" cm":" in");

  if (dist != lastDist) {
    lastDist = dist;
  }
  else {
    // Update parked status
    int parked = dist != 0 && dist<PARKED_DISTANCE;
    if (parked != oldParkedStatus) {
      if (parked)
        Serial.println("Car Parked");
      else
        Serial.println("Car Gone");
      gw.send(msg.set(parked)); 
      oldParkedStatus = parked;
    }
  }
  gw.sleep(SLEEP_TIME);
}




