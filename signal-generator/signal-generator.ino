/* Pro Micro Test Code
   by: Nathan Seidle
   modified by: Jim Lindblom
   SparkFun Electronics
   date: September 16, 2013
   license: Public Domain - please use this code however you'd like.
   It's provided as a learning tool.

   This code is provided to show how to control the SparkFun
   ProMicro's TX and RX LEDs within a sketch. It also serves
   to explain the difference between Serial.print() and
   Serial1.print().
*/

int RXLED = 17;  // The RX LED has a defined Arduino pin

unsigned long time = 0;
unsigned long lastTime = 0;
unsigned long lastChangeTime = 0;
unsigned long changePeriod = 500000; // 0.5 sec
float outputFreqHigh = 150;  // 500 Hz
float outputFreqLow = 0.4; // 0.4 Hz
float outputChangeRate = 0.1;  // 0.1 Hz
float outputFreq = outputFreqLow;

int outputPin = 7;
boolean outputState = 0;

void setup()
{
  pinMode(outputPin, OUTPUT);
  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
}

void loop()
{
  time = micros();

  if (time-lastChangeTime >= changePeriod) {
    if (outputFreq >= outputFreqHigh)
      outputFreq = outputFreqLow;
    outputFreq = outputFreq + outputChangeRate;
    lastChangeTime = time;
  }
  if (time-lastTime >= 1000000/outputFreq) {
    outputState = !outputState;
    digitalWrite(outputPin, outputState);
    digitalWrite(RXLED, outputState);
    lastTime = time;
  }
}

