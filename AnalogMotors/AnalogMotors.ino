/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and grounds
  - LED connected from digital pin 9 to ground
*/

#define arr_len(x) (sizeof(x)/sizeof(int))

const int leftMotorPin = 11;
const int rightMotorPin = 9;
const int midMotorPin = 10;

int levels[] = {0,100,150,200,255};

void setup()
{
  Serial.begin(9600);
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
  pinMode(midMotorPin, OUTPUT);
}

void loop()
{

Serial.print("Size: ");
Serial.println(arr_len(levels));

for (int x = 0; x < arr_len(levels); x++){
  analogWrite(leftMotorPin,levels[x]);
  analogWrite(midMotorPin,levels[x]);
  analogWrite(rightMotorPin,levels[x]);
  delay(1000);
  analogWrite(leftMotorPin,0);
}

analogWrite(leftMotorPin,0);    // 0% duty cycle (off)
delay(2000);
}
