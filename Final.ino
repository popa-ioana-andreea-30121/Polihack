#include <Stepper.h>

const int stepsPerRevolution = 200;
Stepper stepper1(stepsPerRevolution, 6, 7, 8, 9);
Stepper stepper2(stepsPerRevolution, 10, 11, 12, 13);
Stepper stepper3(stepsPerRevolution, 2, 3, 4, 5);

const int micPin = A0;
const int numSamples = 128;
const int sampleRate = 1000;

// Adjust this threshold value based on your environment and testing
const int threshold = 100;

void setup() {
  stepper1.setSpeed(200);
  stepper2.setSpeed(300);
  stepper3.setSpeed(400);
  Serial.begin(9600);
}

void loop() {
  int micValues[numSamples];

  // Collect samples
  for (int i = 0; i < numSamples; i++) {
    micValues[i] = analogRead(micPin);
    delayMicroseconds(100);
  }

  // Find the peak value
  int peakIndex = findPeakIndex(micValues, numSamples);
  int peakFrequency = (peakIndex * sampleRate) / numSamples;

  Serial.print("Peak frequency: ");
  Serial.println(peakFrequency);

  // Compare peak frequency to trigger motors with a threshold
  if ((peakFrequency > 20) && (peakFrequency < 200) && (micValues[peakIndex] > threshold)) {
    stepper1.step(200);
    stepper2.step(0);  // Stop other motors
    stepper3.step(0);
  } else if ((peakFrequency >= 200) && (peakFrequency < 400) && (micValues[peakIndex] > threshold)) {
    stepper1.step(200);
    stepper2.step(300);
    stepper3.step(0);
  } else if ((peakFrequency >= 400) && (peakFrequency < 600) && (micValues[peakIndex] > threshold)) {
    stepper1.step(200);
    stepper2.step(300);
    stepper3.step(400);
  } else {
    stepper1.step(0);  // Stop all motors
    stepper2.step(0);
    stepper3.step(0);
  }
}

// Function to find the index of the maximum value in an array
int findPeakIndex(int array[], int size) {
  int peakIndex = 0;
  for (int i = 1; i < size; i++) {
    if (array[i] > array[peakIndex]) {
      peakIndex = i;
    }
  }
  return peakIndex;
}
