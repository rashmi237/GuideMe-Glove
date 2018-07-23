#include <ArduinoSort.h>


int array[10] = {5, 7, 21, 1, 23, 14, 34, 11, 90, 12};

void setup(){


Serial.begin(9600);
}

void loop(){

sortArray(array,10);

  for (int i = 0; i < 10; i++) {
    Serial.println(array[i]);
  }



}
