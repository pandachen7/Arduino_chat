#include <LoRaTxRx.h>

//LoRa Vcc <-> 5V
//LoRa MD0, MD1, GND <-> GND
//LoRa Tx <-> arduino PIN2 (RX)
//LoRa RX <-> arduino PIN4 (TX)

LoRaTxRx myLora(51);//with receive buffer
char bufferSendStr[51];//send buffer

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //default LoRa address and channel
//  uint8_t addrCh[] = {0, 0, 23};
//  myLora.setAddrChannel(addrCh);
//  myLora.sendAddrChannel();

  //example 1
  char myString[] = "directly sending";
  myLora.sendString(myString);
  Serial.write("you just send \"");
  Serial.write(myString);
  Serial.write("\"\n");

  //example 2
  uint8_t myBytes[] = {49, 50, 51, 52, 53, 0};
  myLora.sendBytes(myBytes, sizeof(myBytes));
  Serial.write("you just send \"");
  Serial.write(myBytes, sizeof(myBytes));
  Serial.write("\"\n");
}

void loop() {
  //if you want to send...
  if(Serial.available() > 0){
    int index = 0;
    while(Serial.available() > 0 && index < 51){
      bufferSendStr[index++] = Serial.read();
    }
    bufferSendStr[index] = 0;//remember to end it
    myLora.sendString(bufferSendStr);
    Serial.write("you send ");
    Serial.write(bufferSendStr);
    Serial.write("\n");
  }
  
  //if you receive something...
  if(myLora.available() > 0){
    delay(40);
    String receivedString = myLora.receiveDataToString();
    //*  handle it
    if(receivedString.equals("haha uccu")){
      //write your case here
      Serial.println("Jackpot!");
    }else{
      //write your case here
      Serial.println("you received " + receivedString);
    }
    //*/
    
  }



// other function to receive
//  if(myLora.available() > 15){ //if receive N(N>=15) characters (bytes), then
//    byte *charIndex = myLora.receiveData();
//    Serial.write(charIndex, myLora.getReceiveDataLength());
//    myLora.flushData();
//  }
  
  delay(200);
}
