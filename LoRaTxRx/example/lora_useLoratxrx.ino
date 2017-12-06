//Very Simple

#include <LoRaTxRx.h>

LoRaTxRx myLora;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  uint8_t addrCh[] = {0, 0, 23};
  myLora.setAddrChannel(addrCh);
  myLora.sendAddrChannel();
    
  char myString[] = "directly sending";
  myLora.sendString(myString);
  Serial.write("You just send \"");
  Serial.write(myString);
  Serial.write("\"\n");

  uint8_t myBytes[] = {49, 50, 51, 52, 53, 0};
  myLora.sendBytes(myBytes, sizeof(myBytes));
  Serial.write("You just send \"");
  Serial.write(myBytes, sizeof(myBytes));
  Serial.write("\"\n");
}

void loop() {

  if(myLora.available() > 0){

    String receivedString = myLora.receiveDataToString();
    //*  handle it
    if(receivedString.equals("haha uccu")){
      //write your case here
      Serial.println("Jackpot!");
    }else{
      //write your case here
      Serial.println(myLora.receiveDataToString());
    }
    //*/
    
  }

  
//  if(myLora.available() > 15){ //if receive N characters (bytes), then
//    delay(100);//wating for remain message, but not a good method
//    byte *charIndex = myLora.receiveData();
//    Serial.write(charIndex, myLora.getReceiveDataLength);
//    myLora.flushData();
//  }
  
  delay(200);
}
