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
// The TX LED was not so lucky, we'll need to use pre-defined
// macros (TXLED1, TXLED0) to control that.
// (We could use the same macros for the RX LED too -- RXLED1,
//  and RXLED0.)

//int inByte = 0;

unsigned long time = 0;
unsigned long lastTime = 0;
unsigned outputPeriod = 300;

int outputPin = 7;
boolean outputState = 0;

void setup()
{
  pinMode(outputPin, OUTPUT);
  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
 // TX LED is set as an output behind the scenes

 //Serial.begin(115200); //This pipes to the serial monitor
 //Serial1.begin(115200); //This is the UART, pipes to sensors attached to board
}

void loop()
{
  time = millis();

  if (time-lastTime >= outputPeriod) {
    outputState = !outputState;
    digitalWrite(outputPin, outputState);
    digitalWrite(RXLED, outputState);
    lastTime = time;
  }
 /* Serial.println("Hello world");  // Print "Hello World" to the Serial Monitor
 Serial1.println("Hello!");  // Print "Hello!" over hardware UART

 while(Serial1.available() > 0) {
  inByte = Serial1.read();
  Serial.write(inByte);
 }
 Serial.write('\n');
 digitalWrite(RXLED, LOW);   // set the LED on
 TXLED0; //TX LED is not tied to a normally controlled pin
 delay(1000);              // wait for a second
 digitalWrite(RXLED, HIGH);    // set the LED off
 TXLED1;
 delay(1000);              // wait for a second
  */
}

