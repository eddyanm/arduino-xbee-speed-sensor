#include <Printers.h>
#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

ZBRxResponse rx = ZBRxResponse();

int packetCount = 0;

void setup() {
  // initialize Serial Monitor
  Serial.begin(115200);
  // initialize hardware UART serial port
  Serial1.begin(115200);
  // hook Xbee to UART serial port
  xbee.setSerial(Serial1);

  delay(500);
  // print message to Serial Monitor that program has started
  Serial.println("Initialized.");
}

void loop() {
  // setup .isAvailable() to work
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    packetCount++;
    xbee.getResponse(response);
 
    Serial.println();
    Serial.print("Packet #");
    Serial.println(packetCount, DEC);
/*
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
      Serial.println();
    }
  }
  else if (xbee.getResponse().isError()) {
    Serial.print("*** Error code:");
    Serial.println(xbee.getResponse().getErrorCode(), DEC);
  }
  else {
    //else case
    delay(1000);
    Serial.println("Waiting for frame.");
    */
  }
}
