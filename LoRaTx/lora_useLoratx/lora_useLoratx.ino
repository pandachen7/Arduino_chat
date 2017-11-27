//Very Simple

#include <LoRaTx.h>
LoRaTx mylora;


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  uint8_t addrCh[] = {0, 0, 23};
  mylora.setAddrChannel(addrCh);

  mylora.setData("haha uccu");
  mylora.sendData();

  mylora.setData("x");
  mylora.sendData();

}

void loop() {

}
