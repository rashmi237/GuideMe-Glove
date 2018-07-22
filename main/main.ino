// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

// US LEFT
#define TRIGGER_PIN_LEFT  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT   7  // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define MAX_DISTANCE_LEFT 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// US MID
#define TRIGGER_PIN_MID  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_MID     5  // Arduino pin tied to echo pin on the ultrasonic sensor.

// US RIGHT
#define TRIGGER_PIN_RIGHT  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_RIGHT     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define MAX_DISTANCE_RIGHT 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int MAX_DISTANCE = 450; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. Can be changed.

//Motor Pins
#define MOTOR_PIN_LEFT 11
#define MOTOR_PIN_MID  10
#define MOTOR_PIN_RIGHT 9

int levels [] = {85,170,255};

int distance_left;
int distance_mid;
int distance_right;

//battery pin variables
#define SENSOR_PIN  A5
float stepVolt = 4.77 / 1024.0;
static unsigned long previousMillis1;


NewPing US_LEFT(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE); // NewPing setup of pins and maximum distance - US1
NewPing US_RIGHT(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); // NewPing setup of pins and maximum distance - US2
NewPing US_MID(TRIGGER_PIN_MID, ECHO_PIN_MID, MAX_DISTANCE); // NewPing setup of pins and maximum distance - US3

float BatteryMonitorVoltage(int sensorPin){
	int sensorValue = analogRead(SENSOR_PIN);

	return sensorValue * stepVolt;
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
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  pinMode(MOTOR_PIN_LEFT, OUTPUT);
  pinMode(MOTOR_PIN_MID, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT, OUTPUT);

	previousMillis1 = 0;
}

void loop() {

	//Timer Code
	if(timer(previousMillis1, 6000)){
		float BatteryValue = BatteryMonitorVoltage(SENSOR_PIN);
		Serial.println(BatteryValue);
  }

	//Ping Code
	delay(100); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");

  distance_left = US_LEFT.ping_cm();
	distance_mid = US_MID.ping_cm();
  distance_right = US_RIGHT.ping_cm();


  // Left Sensor
  if(!(distance_left==501)){ //Checks if signal is recieved
    Serial.print(distance_left);
    Serial.print(",");

		if(distance_left< (MAX_DISTANCE/3)){
			analogWrite(MOTOR_PIN_LEFT,levels[2]);
		}
		else if (distance_left< (MAX_DISTANCE/3)*2 && distance_left> (MAX_DISTANCE/3)) {
			analogWrite(MOTOR_PIN_LEFT,levels[1]);
		}
		else if (distance_left< MAX_DISTANCE && distance_left> (MAX_DISTANCE/3)*2) {
			analogWrite(MOTOR_PIN_LEFT,levels[0]);
		}
		else{
			analogWrite(MOTOR_PIN_LEFT,0);
		}

  }
  else{
    Serial.print("No Signal,");
		analogWrite(MOTOR_PIN_LEFT,0);
  }


  // Middle Sensor
  if(!(distance_mid==501)){ //Checks if signal is recieved
		Serial.print(",");
		Serial.print(distance_mid);

		if(distance_mid< (MAX_DISTANCE/3)){
			analogWrite(MOTOR_PIN_MID,levels[2]);
		}
		else if (distance_mid< (MAX_DISTANCE/3)*2 && distance_mid> (MAX_DISTANCE/3)) {
			analogWrite(MOTOR_PIN_MID,levels[1]);
		}
		else if (distance_mid< MAX_DISTANCE && distance_mid> (MAX_DISTANCE/3)*2) {
			analogWrite(MOTOR_PIN_MID,levels[0]);
		}
		else{
			analogWrite(MOTOR_PIN_MID,0);
		}

  }
  else{
		Serial.print("No Signal,");
		analogWrite(MOTOR_PIN_MID,0);
	}


  // Right Sensor
  if(!(distance_right==501)){ //Checks if signal is recieved
    Serial.print(",");
    Serial.println(distance_right);

		if(distance_right< (MAX_DISTANCE/3)){
			analogWrite(MOTOR_PIN_RIGHT,levels[2]);
		}
		else if (distance_right < (MAX_DISTANCE/3)*2 && distance_right > (MAX_DISTANCE/3)) {
			analogWrite(MOTOR_PIN_RIGHT,levels[1]);
		}
		else if (distance_right < MAX_DISTANCE && distance_right > (MAX_DISTANCE/3)*2) {
			analogWrite(MOTOR_PIN_RIGHT,levels[0]);
		}
		else{
			analogWrite(MOTOR_PIN_RIGHT,0);
		}

  }
  else{
    Serial.println(" Right: No Signal");
		analogWrite(MOTOR_PIN_RIGHT,0);
  }
}
