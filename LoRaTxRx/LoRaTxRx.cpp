#include "LoRaTxRx.h"
#include <stdio.h>
#include <string.h>

SoftwareSerial uartSerial(2, 4);// arduino's RX, TX, can be digital pin

LoRaTxRx::LoRaTxRx(void){
	uartSerial.begin(9600);
	
	//default
	//maxReceivedSize = 30;
	//receivedMsg = (char*)malloc(sizeof(char)*31);

}
LoRaTxRx::LoRaTxRx(int _maxReceivedSize){
	uartSerial.begin(9600);
	
	//maxReceivedSize = _maxReceivedSize - 1;
	//receivedMsg = (char*)malloc(sizeof(char)*_maxReceivedSize);
}

void LoRaTxRx::sendString(const char* _str){
	uartSerial.write(_str);
}
void LoRaTxRx::sendBytes(const uint8_t* _byte, int _size){
	uartSerial.write(_byte, _size);
}

void LoRaTxRx::setAddrChannel(const uint8_t* _byte){
	for (int i = 0; i < 3; i++){
		addrCh[i] = _byte[i];
	}
}

void LoRaTxRx::getAddrChannel(uint8_t* _byte){
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
/*
char LoRaTxRx::receiveChar(){
	return uartSerial.read();
}
bool LoRaTxRx::receiveData(char * _str){

	while(uartSerial.available() && receiveLength < maxReceivedSize){
		receivedChar = uartSerial.read();
		
		receivedMsg[receiveLength++] = receivedChar;
		
	}
	
	if(receivedChar != '\0'){
		//last is not '\0' (Ending)
		return false;
	}
	
	strncpy(_str, receivedMsg, sizeof(receivedMsg));
	
	receiveLength = 0;
	receivedMsg[0] = '\0';
	
	return true;
}

String LoRaTxRx::receiveDataToString(){
	readString = "";
	while(uartSerial.available()){
		receivedChar = uartSerial.read();
		if (receivedChar == '\0'){
			break;
		}
		readString += receivedChar;
	}
	
	if(receivedChar != '\0'){
		return "";
	}
	
	return readString;
}
*/
bool LoRaTxRx::receivedFlag(){
	return uartSerial.available() > 0;
}
String LoRaTxRx::receiveDataToString(){
	
	//if(uartSerial.available() > 0){
		
		//readString = uartSerial.readString();
		
		return uartSerial.readString();
	//}
	
	//return "";
}
void LoRaTxRx::setData(const char * _str){
	//bufferData[0] = 0;
	//strncat(bufferData, _str, MAX_DATA_SIZE);

	strncpy(bufferData, _str, sizeof(bufferData));
	bufferData[sizeof(bufferData)] = '\0';
}
char * LoRaTxRx::getData(){
	return bufferData;
}
void LoRaTxRx::sendData(){
	uartSerial.write(bufferData);
}
