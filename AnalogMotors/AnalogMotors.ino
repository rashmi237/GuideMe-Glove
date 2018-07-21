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
const int leftMotorPin = 11;
const int rightMotorPin = 9;
const int midMotorPin = 10;

int levels [5] = {0,100,150,200,255};

void setup()
{
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
  pinMode(midMotorPin, OUTPUT);
}

void loop()
{


for (int x = 0; x < sizeof(levels); x++){
  analogWrite(leftMotorPin,level[x]);
  analogWrite(midMotorPin,level[x]);
  analogWrite(rightMotorPin,level[x]);
  delay(1000);
}

analogWrite(motorPin,0);    // 0% duty cycle (off)
delay(2000);
}
