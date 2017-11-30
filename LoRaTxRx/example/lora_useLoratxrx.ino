//Very Simple

#include <LoRaTxRx.h>

LoRaTxRx mylora;
String receivedString;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  uint8_t addrCh[] = {0, 0, 23};
  mylora.setAddrChannel(addrCh);
  mylora.sendAddrChannel();
    
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

  if(mylora.receivedFlag()){
    Serial.print(mylora.receiveDataToString());
  }
//  receivedString = mylora.receiveDataToString();
//  if(!receivedString.equals("")){
//    Serial.print(receivedString);
//  }

//  if(mylora.receivedFlag()){
//    myStr = mylora.receiveDataToString();
//    if(!myStr.equals("")){
//      Serial.println(myStr);
//    }
//  }

//  
//  if(){
//    Serial.println(receiveData);
//    receiveData[0] = 0;
//  }

  delay(100);
}
