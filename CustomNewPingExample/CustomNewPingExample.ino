// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

// US LEFT
#define TRIGGER_PIN_LEFT  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT   7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_LEFT 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


// US RIGHT
#define TRIGGER_PIN_RIGHT  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_RIGHT     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_RIGHT 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// US MID
#define TRIGGER_PIN_MID  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_MID     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_MID 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int distance;

NewPing US_LEFT(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE_LEFT); // NewPing setup of pins and maximum distance - US1
NewPing US_RIGHT(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE_RIGHT); // NewPing setup of pins and maximum distance - US2
NewPing US_MID(TRIGGER_PIN_MID, ECHO_PIN_MID, MAX_DISTANCE_MID); // NewPing setup of pins and maximum distance - US3

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");

  distance = sonar.ping_cm();

  if(!(distance==500)){ //Checks if singal is recieved
    Serial.print(distance);
    Serial.println("cm");

    distance = sonar.ping_cm();
  }
  else{
    Serial.println("No Signal");
  }

}
