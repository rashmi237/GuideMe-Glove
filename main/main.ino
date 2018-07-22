// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

// US LEFT
#define TRIGGER_PIN_LEFT  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT   7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_LEFT 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// US RIGHT
#define TRIGGER_PIN_RIGHT  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_RIGHT     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_RIGHT 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// US MID
#define TRIGGER_PIN_MID  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_MID     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_MID 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//Motor Pins
#define MOTOR_PIN_LEFT 11;
#define MOTOR_PIN_RIGHT 9;
#define MOTOR_PIN_MID  10;

int levels [5] = {0,100,150,200,255};

int distance_left;
int distance_mid;
int distance_right;

<<<<<<< HEAD
=======
//battery pin variables
float stepVolt = 5.0 / 1024.0;
int sensorPin = A5;    // select the input pin for the potentiometer
//int sensorValue = 0;  // variable to store the value coming from the sensor


>>>>>>> 1b354333dd6f44bc8d52a55cde469974ad213da2
NewPing US_LEFT(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE_LEFT); // NewPing setup of pins and maximum distance - US1
NewPing US_RIGHT(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE_RIGHT); // NewPing setup of pins and maximum distance - US2
NewPing US_MID(TRIGGER_PIN_MID, ECHO_PIN_MID, MAX_DISTANCE_MID); // NewPing setup of pins and maximum distance - US3

int BatteryMonitor(int sensorPin){
	// read the value from the sensor:
	int sensorValue = analogRead(sensorPin);

	return sensorValue;
}

//https://arduino.stackexchange.com/questions/12915/timer-function-without-the-use-of-a-library
bool timer(unsigned long &last_time, unsigned long period) {
  unsigned long now = millis();
  if (now - last_time >= period) {
    last_time = now;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(MOTOR_PIN_LEFT, OUTPUT);
  pinMode(MOTOR_PIN_MID, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT, OUTPUT);
}

void loop() {
  static unsigned long previousMillis1;
  
  if(timer(previousMillis1, 6000)){
	int sensorValue = BatteryMonitor(sensorPin);
	Serial.println(sensorValue);
	Serial.println(sensorValue * stepVolt);
  } 
  delay(500); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");

  distance_left = US_LEFT.ping_cm();
  distance_right = US_RIGHT.ping_cm();
  distance_mid = US_MID.ping_cm();

  // left

  if(!(distance_left==501)){ //Checks if singal is recieved
    Serial.print(distance_left);
    Serial.print(",");
  }
  else{
    Serial.print("No Signal,");
  }

  // mid
  if(!(distance_mid==501)){ //Checks if singal is recieved
    Serial.print(distance_mid);
  }
  else{
    Serial.print(" Middle: No Signal");
  }

  // RIGHT
  if(!(distance_right==501)){ //Checks if singal is recieved
    Serial.print(" Right: ");
    Serial.println(distance_right);
  }
  else{
    Serial.println(" Right: No Signal");
  }

}
