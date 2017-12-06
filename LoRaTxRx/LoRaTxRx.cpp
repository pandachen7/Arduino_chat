#include "LoRaTxRx.h"
#include <stdio.h>
#include <string.h>

SoftwareSerial uartSerial(2, 4);// arduino's RX, TX, can be digital pin

LoRaTxRx::LoRaTxRx(void){
	uartSerial.begin(9600);
	
	//default: 30
	maxReceivedSize = 30;
	receivedMsg = (byte*)malloc(sizeof(byte)*31);

}
LoRaTxRx::LoRaTxRx(int _maxReceivedSize){
	uartSerial.begin(9600);
	
	maxReceivedSize = _maxReceivedSize;
	receivedMsg = (byte*)malloc(sizeof(byte)*_maxReceivedSize + 1);
}
void LoRaTxRx::sendString(String _str){
	uartSerial.print(_str);
}
void LoRaTxRx::sendChars(const char* _str){
	uartSerial.write(_str);
}
void LoRaTxRx::sendBytes(const byte* _byte, int _size){
	uartSerial.write(_byte, _size);
}

void LoRaTxRx::setAddrChannel(const byte* _byte){
	for (int i = 0; i < 3; i++){
		addrCh[i] = _byte[i];
	}
}

void LoRaTxRx::getAddrChannel(byte* _byte){
	for (int i = 0; i < 3; i++){
		_byte[i] = addrCh[i];
	}
}
void LoRaTxRx::setDefaultAddrChannel(){
	addrCh[0] = 0;
	addrCh[1] = 0;
	addrCh[2] = 23;
}
void LoRaTxRx::sendAddrChannel(){
	uartSerial.write(addrCh, sizeof(addrCh));
}

byte * LoRaTxRx::receiveData(){//byte * _str){
	byte receivedOne;
	while(uartSerial.available() > 0 && receiveLength < maxReceivedSize){
		receivedOne = uartSerial.read();
		/*if (receivedOne == '\0'){
			break;
		}*/
		receivedMsg[receiveLength++] = receivedOne;
	}
	
	/*
	if(receivedOne != '\0'){
		//last is not '\0', must wait for remain message
		return 0;
	}*/
	
	//_str = receivedMsg;
	//strncpy(_str, receivedMsg, sizeof(receivedMsg));
	
	//receiveLength = 0;
	//receivedMsg[0] = '\0';
	
	return receivedMsg;
}
int LoRaTxRx::getReceiveDataLength(){
	return receiveLength;
}
byte * LoRaTxRx::getReceiveDataIndex(){
	return receivedMsg;
}
void LoRaTxRx::flushData(){
	while(uartSerial.available() > 0){
		uartSerial.read();
	}
	receiveLength = 0;
	receivedMsg[0] = '\0';
}
int LoRaTxRx::available(){
	return uartSerial.available();
}

String LoRaTxRx::receiveDataToString(){
	return uartSerial.readString();
}