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
  mylora.sendAddrChannel();
    
  mylora.setData("haha uccu\n");// not enough space, only set 'haha uc', note that the last of char[] should be 0 or '\0', so this example isn't good
  mylora.sendData();
  Serial.write("You just send \"");
  Serial.write(mylora.getData());
  Serial.write("\"\n");

  char myString[] = "directly sending";
  mylora.sendString(myString);
  Serial.write("You just send \"");
  Serial.write(myString);
  Serial.write("\"\n");

  uint8_t myBytes[] = {49, 50, 51, 52, 53, 0};
  mylora.sendBytes(myBytes, sizeof(myBytes));
  Serial.write("You just send \"");
  Serial.write(myBytes, sizeof(myBytes));
  Serial.write("\"\n");
}

void loop() {

}
