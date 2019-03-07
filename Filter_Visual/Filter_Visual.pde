// Tutorial used to connect Processing to port: https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing

// Imports
import processing.serial.*;

// Global constants
private int PORT_NUMBER = 11; // Change the 0 to a 1 or 2 etc. to match your Arduino's port
private int SERIAL_RATE = 9600; // Using same rate as Arduino
private color BACKGROUND_COLOR = color(0);
private color AXIS_COLOR = color(255, 127, 0);
private color INPUT_COLOR = color(255);
private color INPUT_STEADY_COLOR = color(255, 0, 127);
private color LPF_COLOR = color(0, 127, 255);
private color MEDIAN_FILTER_COLOR = color(127, 255, 0);
private color LPF_MEDIAN_FILTER_COLOR = color(127, 0, 255);
private int WINDOW_WIDTH = 1024; // Remember to change size() function as well!
private int WINDOW_HEIGHT = 768;
private int ORIGIN_X = 80;
private int ORIGIN_Y = WINDOW_HEIGHT - 80;
private int INPUT_HEIGHT = 60;
private int INPUT_X = ORIGIN_X;
private int INPUT_Y = 80;
private int INPUT_STEADY_HEIGHT = 60;
private int INPUT_STEADY_X = ORIGIN_X;
private int INPUT_STEADY_Y = 160;
private int LPF_HEIGHT = 60; // Low Pass Filter
private int LPF_X = ORIGIN_X;
private int LPF_Y = 240;
private int MEDIAN_FILTER_HEIGHT = 60;
private int MEDIAN_FILTER_X = ORIGIN_X;
private int MEDIAN_FILTER_Y = 320;
private int LPF_MEDIAN_FILTER_HEIGHT = 60;
private int LPF_MEDIAN_FILTER_X = ORIGIN_X;
private int LPF_MEDIAN_FILTER_Y = 400;
private int PIXELS_PER_CM = 16;
private int TICK_HEIGHT = 10;
private float LPF_BETA = 0.05f;
private int MEDIAN_FILTER_WINDOW = 25;

// Global variables
private Serial inputPort;
private String inputString;
private float inputValue = 0;
private float lpfValue = 0;
private float median = 0;
private float lpfMedian = 0;
private float[] medianFilter = new float[MEDIAN_FILTER_WINDOW];
private int medianFilterIndex = 0;

// Setup step
void setup() {
  String portName = Serial.list()[PORT_NUMBER];
  inputPort = new Serial(this, portName, SERIAL_RATE);
  size(1024, 768); // CAN ONLY USE NUMBERS HERE!
  background(BACKGROUND_COLOR);
  for (int i = 0; i < medianFilter.length; i++) {
    medianFilter[i] = 0;
  }
}

float getMedian(float[] input) {
  //float[] sorted = new float[input.length];
  //for (int i = 0; i < input.length; i++) {
  //  float[] sorted[i] = input[i];
  //}
  float[] sorted = sort(input);
  sorted = sort(input);
  //println("unsorted:");
  //println(input);
  //println("sorted:");
  //println(sorted);
  //println(sorted[sorted.length / 2]);
  return sorted[sorted.length / 2];
  
}

float PDControl (
    float target,
    float targetPrevious,
    float position,
    float speed,
    float kp,
    float kd,
    float maxImpulse, // -1 for no limit
    float deltaTime)
{
  // Rreturns new Velocity
  float impulse = kp * (target - position) + kd * ((target - targetPrevious) - speed);
  if (impulse > maxImpulse)
    impulse = maxImpulse;
  else if (impulse < -maxImpulse)
    impulse = -maxImpulse;
  return speed + impulse * deltaTime;
}

// Draw step (and main loop?)
void draw() {

  clear(); // input is available with less frequency than draw event called.
  textSize(24); // For Null vs Zero input string
  noStroke();

  // Draw the raw input value without flickering
  fill(INPUT_STEADY_COLOR);
  rect(INPUT_STEADY_X, INPUT_STEADY_Y, PIXELS_PER_CM * inputValue, INPUT_STEADY_HEIGHT);

  // Draw the low pass filtered value.
  // Simple LPF. Comment from https://kiritchatterjee.wordpress.com/2014/11/10/a-simple-digital-low-pass-filter-in-c/
  // LPF: Y(n) = (1-ÃŸ)*Y(n-1) + (ÃŸ*X(n))) = Y(n-1) - (ÃŸ*(Y(n-1)-X(n)));
  // Interesting that LPF is just "P" of PID.
  // Outside of loop because if signal dies, assume previous signal.
  lpfValue = lpfValue + LPF_BETA * (inputValue - lpfValue);
  fill(LPF_COLOR);
  rect(LPF_X, LPF_Y, PIXELS_PER_CM * lpfValue, LPF_HEIGHT);
  //text(lpfValue, 100, 275);

  // Draw the median filtered value
  medianFilter[medianFilterIndex] = inputValue;
  medianFilterIndex++;
  if (medianFilterIndex >= medianFilter.length) {
    medianFilterIndex = 0;
  }
  median = getMedian(medianFilter);

  fill(MEDIAN_FILTER_COLOR);
  rect(MEDIAN_FILTER_X, MEDIAN_FILTER_Y, PIXELS_PER_CM * median, MEDIAN_FILTER_HEIGHT);

  // Draw a LPF on the median filter - protection from spikes, smooth traversal
  // Ultimately I believe a PID on a median filter would be even better, since this IS a controls problem.
  // Median filter gets "Where I'm looking" ignoring brief spikes, while PID gets there in a smooth manner on both ends.
  // However, due to time constraints and experimental (disorganized) nature of this code, sticking with proportional for now.
  // (proportional-derivitive method above copied from an implementation of that I made a while ago, could be "good enough.")
  lpfMedian = lpfMedian + LPF_BETA * (median - lpfMedian);
  fill(LPF_MEDIAN_FILTER_COLOR);
  rect(LPF_MEDIAN_FILTER_X, LPF_MEDIAN_FILTER_Y, PIXELS_PER_CM * lpfMedian, LPF_MEDIAN_FILTER_HEIGHT);

  // Draw the raw value if available.
  if (inputPort.available() > 0) {
    inputString = inputPort.readStringUntil('\n');
    fill(INPUT_COLOR);
    if (inputString != null) {
      inputValue = parseFloat(inputString);
      if (inputValue != 0) {
        rect(INPUT_X, INPUT_Y, PIXELS_PER_CM * inputValue, INPUT_HEIGHT);
      } else {
        //inputValue = prevInput; // can result in getting stuck in short range mode at long distance.
        // because 0 is out of range, basically same as 300 cm (max range of input)
        inputValue = 300;
        //text("ZERO", INPUT_X + 10, INPUT_Y + INPUT_HEIGHT / 2);
      }
    } else {
      text("NULL", INPUT_X - 70, INPUT_Y + INPUT_HEIGHT / 2);
    }
  }

  drawAxes();
}

void drawAxes() {
  stroke(AXIS_COLOR);
  fill(AXIS_COLOR);
  line(ORIGIN_X, ORIGIN_Y, WINDOW_WIDTH, ORIGIN_Y);
  line(ORIGIN_X, ORIGIN_Y, ORIGIN_X, 0);
  textSize(24);
  for (int i = 0; i <= 50; i += 5) {
    int xPos = PIXELS_PER_CM * i;
    text(i, ORIGIN_X + xPos, ORIGIN_Y + 40);
    float tickHeight = TICK_HEIGHT * 0.5f;
    line(ORIGIN_X + xPos, ORIGIN_Y - tickHeight, ORIGIN_X + xPos, ORIGIN_Y + tickHeight);
  }
}
