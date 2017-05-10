//#include <Printers.h>
#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

ZBRxResponse rx = ZBRxResponse();

unsigned long time = 0;
unsigned long lastTime = 0;
unsigned long packetCount = 0;

void setup() {
  // initialize Serial Monitor
  Serial.begin(115200);
  // initialize hardware UART serial port
  Serial1.begin(115200);
  // hook Xbee to UART serial port
  xbee.setSerial(Serial1);

  //delay(500);
  // print message to Serial Monitor that program has started
  //Serial.println("Initialized.");
}

void loop() {
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
}
