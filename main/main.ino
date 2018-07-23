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

int MAX_DISTANCE = 450; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. Can be changed.

//Ultrasonic sensor declaration
NewPing US_LEFT(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE); // NewPing setup of pins and maximum distance - US1
NewPing US_RIGHT(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); // NewPing setup of pins and maximum distance - US2
NewPing US_MID(TRIGGER_PIN_MID, ECHO_PIN_MID, MAX_DISTANCE); // NewPing setup of pins and maximum distance - US3

int distance_left;
int distance_mid;
int distance_right;

//Motor Pins
#define MOTOR_PIN_LEFT 11
#define MOTOR_PIN_MID  10
#define MOTOR_PIN_RIGHT 9

const int Motorlevels [] = {85,170,255};
float intensityFactor;

//battery pin variables
#define SENSOR_PIN  A5
const float stepVolt = 4.77 / 1024.0;
const int checkBatteryTime = 6000

//timer Varibales
static unsigned long previousMillis1;

//Methods
float batteryMonitorVoltage(int sensorPin){
	int sensorValue = analogRead(sensorPin);

	return sensorValue * stepVolt;
}

//https://arduino.stackexchange.com/questions/12915/timer-function-without-the-use-of-a-library
bool timer(unsigned long &last_time, unsigned long period) {
  unsigned long now = millis(); //Global time since starting
  if (now - last_time >= period) {
    last_time = now;
    return true;
  }
  return false;
}

void motorSetting(int distance, int motorpin, int intensity){

	if(!(distance==501)){ //Checks if signal is recieved
    Serial.print(distance);
    Serial.print(",");

		if(distance< (MAX_DISTANCE/3)){
			analogWrite(motorpin,Motorlevels[2]);
		}
		else if (distance< (MAX_DISTANCE/3)*2 && distance> (MAX_DISTANCE/3)) {
			analogWrite(motorpin,Motorlevels[1]);
		}
		else if (distance< MAX_DISTANCE && distance> (MAX_DISTANCE/3)*2) {
			analogWrite(motorpin,Motorlevels[0]);
		}
		else{
			analogWrite(motorpin,0);
		}
  }
	else{
    	Serial.print("No Signal,");
			analogWrite(motorpin,0);
  }
}

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  pinMode(MOTOR_PIN_LEFT, OUTPUT);
  pinMode(MOTOR_PIN_MID, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT, OUTPUT);

	previousMillis1 = 0;
}

void loop()

	//Bluetooth
	if(Serial.available() > 0)      // Send data only when you receive data:
	{
		 data = Serial.read();        //Read the incoming data & store into data
		 Serial.println(data);          //Print Value inside data in Serial monitor

		 if(data == 1){              // Checks whether value of data is equal to 1
				Serial.println("ON if"); //If value is 1 then LED turns ON
		 }
		 if(data == 0)         //  Checks whether value of data is equal to 0
				Serial.println("OFF if")    //If value is 0 then LED turns OFF
	}

	//Timer Code
	if(timer(previousMillis1, checkBatteryTime)){
		float BatteryValue = batteryMonitorVoltage(SENSOR_PIN);
		Serial.println(BatteryValue);
  }

	//Ping Code
	delay(100); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");

  distance_left = US_LEFT.ping_cm();
	distance_mid = US_MID.ping_cm();
  distance_right = US_RIGHT.ping_cm();


  // Left Sensor
	motorSetting(distance_left, MOTOR_PIN_LEFT,1234);

  // Middle Sensor
  motorSetting(distance_right,MOTOR_PIN_MID,1234);

  // Right Sensor
  motorSetting(distance_right,MOTOR_PIN_RIGHT,1234);
	Serial.println();
}
