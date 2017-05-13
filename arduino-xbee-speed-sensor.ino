#include <XBee.h>
#include <FreqMeasure.h>

// create XBee object and payload
XBee xbee = XBee();

// reduced Motec Data Set 3/CRC32 format
// 3 header, 1 data length, 30 channels, 4 CRC32 bytes
const unsigned int payloadLen = 4 + 60 + 4; // 68
uint8_t payload[payloadLen]; 

// Coordinator address for receiving XBee and TX object
XBeeAddress64 addr64 = XBeeAddress64(0x0, 0x0);
ZBTxRequest tx = ZBTxRequest(addr64, payload, payloadLen);
//ZBTxStatusResponse txStatus = ZBTxStatusResponse();

unsigned long time = 0;
unsigned long lastTxTime = 0;
unsigned long lastSampleTime = 0;
unsigned txPeriod = 100;  // delay between TX in ms
unsigned samplePeriod = 2000;  // max delay between input samples in ms
//unsigned long packetCount = 0;

// speed sesnor pin
int sensorPin = 4;

// speed sensor values
float sensorFreq = 0;
float sensorCoeff = 2.65439627; // specific to NSX speed sensor

// init vehicle speeds variables for Tx
uint16_t groundSpeed = 0;  // in 0.1km/h
uint16_t driveSpeed = 0;  // in 0.1km/h

void setup() {
  // initialize Serial Monitor
  Serial.begin(115200);
  // initialize hardware UART serial port
  Serial1.begin(115200);
  // hook Xbee to UART serial port
  xbee.setSerial(Serial1);
  // assign Motec Data Set 3 format data into payload[]
  payload[0] = 0x82;
  payload[1] = 0x81;
  payload[2] = 0x80;
  payload[3] = 30;
  for (int i=4; i<=63; i++) {
    payload[i] = 0;
  }
  for (int i=64; i<=67; i++) {
    payload[i] = 0xff;
  }
  // init input pin for speed sensor
  pinMode(sensorPin, INPUT_PULLUP);
  // setup frequency measurement
  FreqMeasure.begin();  // uses pin 7-ICP1
}

void loop() {
  // loop time
  time = millis();

  // read sensor frequency
  if (FreqMeasure.available()) {
    sensorFreq = FreqMeasure.countToFrequency(FreqMeasure.read());
    lastSampleTime = time;
  } else {
    if (time-lastSampleTime >= samplePeriod)
      sensorFreq = 0;
  }

  //get speeds in 0.1km/h
  // NSX speed sesnor pulses 4 times per rev
  // diff gear to speed sensor gear ratio is 29:23
  // 245/40-17 tire has 526 rev per km traveled
  // 1.900052km/hr=1000*4*29/23 pulses/3600s
  // 1000*4*29*1000/23 pulses/3600s/period(ms) -> km/hr
  // coeff = 526*4*(29/23)/3600/10
  groundSpeed = sensorCoeff * sensorFreq * 10;
  driveSpeed = groundSpeed;
  
  // send data if time to send
  if (time-lastTxTime >= txPeriod) {
    // print debug info
    Serial.print("t=");
    Serial.print(time, DEC);
    Serial.print("ms f=");
    Serial.print(sensorFreq);
    Serial.print("Hz v=");
    Serial.print(sensorCoeff * sensorFreq);
    Serial.println("km/h");
    // load drive speed
    payload[61] = driveSpeed >> 8 & 0xff;
    payload[60] = driveSpeed & 0xff;
    // load ground speed
    payload[63] = groundSpeed >> 8 & 0xff;
    payload[62] = groundSpeed & 0xff;
    // send packet
    xbee.send(tx);
    // reset timer
    lastTxTime = time;
  }
}
