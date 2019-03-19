// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------
#include <ArduinoSort.h>
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

int MAX_DISTANCE =500; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. Can be changed.

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

//different feedback strengths
const int Motorlevels [] = {30,50,85};
float intensityFactor;
float defValue;

//battery pin variables
#define SENSOR_PIN  A5
const float stepVolt = 4.77 / 1024.0;
const int checkBatteryTime = 12000;

//timer Varibales
static unsigned long previousMillisBattery;
static unsigned long previousMillisFreq;

//Filter constants
const int MEDIAN_FILTER_WINDOW = 25;
const float LPF_ALPHA = 0.5f;

const int checkFreq = 1000;


int inputValue;
float medianLeft, medianMid, medianRight;
float lpfMedianLeft, lpfMedianMid, lpfMedianRight;
int nosig_count_left, nosig_count_mid, nosig_count_right;
float lpfMedian;
int medianFilterIndex;
float medianFilterLeft[MEDIAN_FILTER_WINDOW];
float medianFilterMid[MEDIAN_FILTER_WINDOW];
float medianFilterRight[MEDIAN_FILTER_WINDOW];

//Bluetooth
String data;

int recvVibrationValue;
int recvDetectionDistance;

//Main Methods for main
#define arr_len(x) (sizeof(x)/sizeof(int))

////*Methods*////
float getMedian(float input[], int inputSize){ // Calculate the median value of the input array
	float sorted[inputSize]; // sort the inputs
	sortArray(input, inputSize);

	return input[inputSize/2];
}

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

void motorSetting(int distance, int motorpin, float intensity, int count){
	if(!(distance==0) && count < 25){ //Checks if signal is recieved
    // Serial.print(distance);
    // Serial.print(",");

		if(distance< (MAX_DISTANCE/3)){
			analogWrite(motorpin, Motorlevels[2] * intensity);
		}
		else if (distance< (MAX_DISTANCE/3)*2 && distance> (MAX_DISTANCE/3)) {
			analogWrite(motorpin, Motorlevels[1] * intensity);
		}
		else if (distance< MAX_DISTANCE && distance> (MAX_DISTANCE/3)*2) {
			analogWrite(motorpin, Motorlevels[0] * intensity);
		}
		else{
			analogWrite(motorpin, 0);
		}
  }
	else{
    	//Serial.print("No Signal,");
			analogWrite(motorpin, 0 );
  }
}

void serialCheck(){
	//Bluetooth code
	if(Serial.available() > 0){      // Send data only when you receive data:
		 data = Serial.readString();        //Read the incoming data & store into data

     Serial.print("Data from Bluetooth: ");
		 Serial.println(data);
		 //Serial.println(toString(data));          //Print Value inside data in Serial monitor


		 if(data.indexOf("v") == 0) {              // Checks whether value of data is equal to 1
			 	data.remove(0, 1);
				recvVibrationValue = data.toInt();
				switch (recvVibrationValue) {
				  case 0:
				    intensityFactor = float(defValue/2);
				    break;
					case 1:
						intensityFactor = defValue;
						break;
					case 2:
						intensityFactor = float(defValue*1.5);
						break;
					case 3:
						intensityFactor = 0.0;
						break;
					default:
						intensityFactor = defValue;
				// if (recvVibrationValue == 0) {intensityFactor = 0.33;}
				// if (recvVibrationValue == 1) {intensityFactor = 0.66;}
				// if (recvVibrationValue == 2) {intensityFactor = 1.00;}
		 		}

		 		Serial.print("Vibration input: "); //If value is 1 then LED turns ON
		 		Serial.println(recvVibrationValue);
	 		}
		 if(data.indexOf("d") == 0) {         //  Checks whether value of data is equal to 0
			 	data.remove(0, 1);
			 	recvDetectionDistance = data.toInt();
				switch (recvDetectionDistance) {
				  case 0:
				    MAX_DISTANCE = 100;
				    break;
					case 1:
						MAX_DISTANCE = 150;
						break;
					case 2:
						MAX_DISTANCE = 200;
						break;
					case 3:
						MAX_DISTANCE = 250;
						break;
					case 4:
						MAX_DISTANCE = 300;
						break;
					case 5:
						MAX_DISTANCE = 350;
						break;
					case 6:
						MAX_DISTANCE = 400;
						break;
					case 7:
						MAX_DISTANCE = 450;
						break;
				  default:
				    MAX_DISTANCE = 400;
				    break;
				}
				//MAX_DISTANCE = recvDetectionDistance * 100;
				Serial.print("Max distance input: ");    //If value is 0 then LED turns OFF
				Serial.println(MAX_DISTANCE);
			}
		}
  }

void batteryCheck(){
	float batteryValue = batteryMonitorVoltage(SENSOR_PIN);
			//if (batteryValue > 3){
				// below 3, battery not connected

				String battString = "S" + (String) batteryValue;
				Serial.println(battString);
				//Serial.write((byte)batteryValue);
			//}
}
/////*Main code*/////
void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  pinMode(MOTOR_PIN_LEFT, OUTPUT);
  pinMode(MOTOR_PIN_MID, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT, OUTPUT);

	//Timer initialize
	previousMillisBattery = 0;
	previousMillisFreq = 0;

	//Bluetooth
	data = "";

	//Median variable initialize
	inputValue = 0;
	medianLeft = 0;
	medianMid = 0;
	medianRight = 0;
	lpfMedianLeft = 0;
	lpfMedianRight = 0;
	lpfMedianMid = 0;
	nosig_count_left = 0;
	nosig_count_mid = 0;
	nosig_count_right = 0;
	medianFilterIndex = 0;
	//max value of defValue is 0.66, don't increase
	defValue = 0.66;
	intensityFactor = defValue;
	MAX_DISTANCE = 400;

	// initialize medianFilter array
	for (int i = 0; i < MEDIAN_FILTER_WINDOW; i++) {
		medianFilterLeft[i] = 0;
		medianFilterMid[i] = 0;
		medianFilterRight[i] = 0;
	}
}

void loop(){

	serialCheck();

	//Timer Code
	if(timer(previousMillisBattery, checkBatteryTime)){
		batteryCheck();
  }



	//Ping Code
	delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  // Serial.print("LPF Median Ping: ");

  distance_left = US_LEFT.ping_cm();
	distance_mid = US_MID.ping_cm();
  distance_right = US_RIGHT.ping_cm();

	//Filter Code
	//inputValue = distance_left;
	if(distance_left != 0){
			medianFilterLeft[medianFilterIndex] = distance_left;
			nosig_count_left = 0;
	}
	else{
		nosig_count_left++;
	}


	if(distance_mid != 0){
		medianFilterMid[medianFilterIndex] = distance_mid;
		nosig_count_mid = 0; // at the first instance of regaining signal after a string of zeroes, the nosignal counter goes back to 0
	}
	else{
		nosig_count_mid++; // keep track of the number of consecutive nosignals (0s)
	}

	if(distance_right != 0) {
		medianFilterRight[medianFilterIndex] = distance_right;
		nosig_count_right = 0;
	}
	else{
		nosig_count_right++;
	}

	medianFilterIndex++;
	if (medianFilterIndex >= MEDIAN_FILTER_WINDOW) {
		medianFilterIndex = 0;
	}

	medianLeft = getMedian(medianFilterLeft, MEDIAN_FILTER_WINDOW);
	medianMid = getMedian(medianFilterMid, MEDIAN_FILTER_WINDOW);
	medianRight = getMedian(medianFilterRight, MEDIAN_FILTER_WINDOW);
	// Serial.print(distance_left);
	// Serial.print("Unsmoothed: ");
	// Serial.print(distance_left);
	// Serial.print(", Median: ");
	// Serial.print(median);

// -- LPF --
	// lpfValue = lpfValue + LPF_ALPHA * (inputValue - lpfValue);
	// Serial.print(", LPF: ");
	// Serial.print(lpfValue);

// -- LPF w/ Median --
	lpfMedianLeft = lpfMedianLeft + LPF_ALPHA * (medianLeft - lpfMedianLeft);
	lpfMedianMid = lpfMedianMid + LPF_ALPHA * (medianMid - lpfMedianMid);
	lpfMedianRight = lpfMedianRight + LPF_ALPHA * (medianRight - lpfMedianRight);

	// Serial.print(", LPFMedian: ");
	// Serial.println(lpfMedian);


// -- Testing Filter

	//Motor settings
  // Left motor

	if(timer(previousMillisFreq, checkFreq)){
		motorSetting(lpfMedianLeft, MOTOR_PIN_LEFT, 0, nosig_count_left);
		motorSetting(lpfMedianMid, MOTOR_PIN_MID, 0, nosig_count_mid);
		motorSetting(lpfMedianRight, MOTOR_PIN_RIGHT, 0, nosig_count_right);
	}

	else
	{
		motorSetting(lpfMedianLeft, MOTOR_PIN_LEFT, intensityFactor, nosig_count_left);
		motorSetting(lpfMedianMid, MOTOR_PIN_MID, intensityFactor, nosig_count_mid);
		motorSetting(lpfMedianRight, MOTOR_PIN_RIGHT, intensityFactor, nosig_count_right);
	}

	//just commented this
	Serial.print(lpfMedianLeft);
	Serial.print(",");
	Serial.print(lpfMedianMid);
	Serial.print(",");
	Serial.print(lpfMedianRight);


	Serial.print("	RAW: ");
	Serial.print(distance_left);
	Serial.print(",");
	Serial.print(distance_mid);
	Serial.print(",");
	Serial.print(distance_right);
	// Serial.print(medianRight);

	// just commented this
	Serial.println();
}
