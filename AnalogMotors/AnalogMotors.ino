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
const int motorPin = 3;

void setup()
{
  pinMode(motorPin, OUTPUT);
}

void loop()
{

int levels
for (int x = 0; x<255; x++){
  analogWrite(motorPin,x);
  delay(50);
}

analogWrite(motorPin,0);    // 0% duty cycle (off)
delay(2000);
}
