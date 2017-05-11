//#include <Printers.h>
#include <XBee.h>

// create XBee object and payload
XBee xbee = XBee();

// reduced Motec Data Set 3/CRC32 format
// 3 header, 1 data length, 30 channels, 4 CRC32 bytes
const unsigned int payloadLen = 4 + 60 + 4;
uint8_t payload[payloadLen]; 

// RX object
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

// address for receiving XBee and TX object
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403217f3);
ZBTxRequest tx = ZBTxRequest(addr64, payload, payloadLen);
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

unsigned long time = 0;
unsigned long lastTime = 0;
unsigned txPeriod = 100;  // delay between TX in ms
unsigned long packetCount = 0;

// speed sesnor pin
int sensorPin = 7;

// speed sensor values
unsigned long sensorPeriod = 0; // in microseconds
unsigned long sensorCoeff = 265439627; // specific to NSX speed sensor

// init vehicle speeds in 0.1km/h from speed sensor
uint16_t groundSpeed = 0;
uint16_t driveSpeed = 0;

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
}

void loop() {
  // sample loop time
  time = millis();

  // read sensor period
  sensorPeriod = pulseIn(sensorPin, LOW) * 2;  // in microseconds
  //sensorPeriod = pulseIn(sensorPin, HIGH);
  
  //get speeds in 0.1km/h
  // NSX speed sesnor pulses 4 times per rev
  // diff gear to speed sensor gear ratio is 29:23
  // 245/40-17 tire has 526 rev per km traveled
  // 1.900052km/hr=1000*4*29/23 pulses/3600s
  // 1000*4*29*1000/23 pulses/3600s/period(ms) -> km/hr
  // coeff = 526*4*(29/23)/3600/10
  //sensorPeriod = 200000000;
  groundSpeed = sensorPeriod == 0 ? 0 : sensorCoeff / sensorPeriod;
  driveSpeed = groundSpeed;
  
  // send data if time to send
  if (time-lastTime >= txPeriod) {
    // print debug info
    Serial.print("t=");
    Serial.print(time, DEC);
    Serial.print("ms T=");
    Serial.print(sensorPeriod, DEC);
    Serial.print("us v=");
    Serial.print(groundSpeed, DEC);
    Serial.println("*0.1km/h");
    // load drive speed
    payload[61] = driveSpeed >> 8 & 0xff;
    payload[60] = driveSpeed & 0xff;
    // load ground speed
    payload[63] = groundSpeed >> 8 & 0xff;
    payload[62] = groundSpeed & 0xff;
    // send packet
    xbee.send(tx);
    // reset timer
    lastTime=time;
  }
  /*
  // count loops
  time = millis();
  
  // setup .isAvailable() to work
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    packetCount++;
    xbee.getResponse(response);
 
    Serial.print("P#");
    Serial.print(packetCount, DEC);
    Serial.print(" T=");
    Serial.print(time, DEC);
    Serial.print("ms dT=");
    Serial.print(time-lastTime, DEC);
    Serial.println("ms");
    lastTime = time;

    if (response.getApiId() == ZB_RX_RESPONSE) {
      response.getZBRxResponse(rx);
      Serial.print("Frame Type is 0x");
      Serial.println(response.getApiId(), HEX);
      Serial.print("Frame is ");
      for (int i=0; i < rx.getDataLength(); i++) {
        if (iscntrl(rx.getData()[i]))
          Serial.write(' ');
        else
          Serial.write(rx.getData()[i]);
      }
      Serial.print(" Checksum is ");
      Serial.print(rx.getChecksum(), HEX);
      Serial.println();
    }
  }
  else if (xbee.getResponse().isError()) {
    xbee.getResponse(response);
    Serial.print("*** Error code:");
    Serial.println(response.getErrorCode(), DEC);
  }
  else {
    //else case
  }
  */
}
