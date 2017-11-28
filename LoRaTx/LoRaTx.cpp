#include "LoRaTx.h"
#include <stdio.h>
#include <string.h>

SoftwareSerial uartSerial(2, 4);// arduino's RX, TX, can be digital pin

LoRaTx::LoRaTx(void){
	//uartSerial = new SoftwareSerial(2, 4); 
	uartSerial.begin(9600);
	
}

void LoRaTx::sendString(const char* _str){
	uartSerial.write(_str);
}
void LoRaTx::sendBytes(const uint8_t* _byte, int _size){
	uartSerial.write(_byte, _size);
}

void LoRaTx::setAddrChannel(const uint8_t* _byte){
	for (int i = 0; i < 3; i++){
		addrCh[i] = _byte[i];
	}
}

void LoRaTx::getAddrChannel(uint8_t* _byte){
	for (int i = 0; i < 3; i++){
		_byte[i] = addrCh[i];
	}
}
void LoRaTx::setDefaultAddrChannel(){
	addrCh[0] = 0;
	addrCh[1] = 0;
	addrCh[2] = 23;
}
void LoRaTx::sendAddrChannel(){
	uartSerial.write(addrCh, sizeof(addrCh));
}

void LoRaTx::setData(const char * _str){
	//bufferData[0] = 0;
	//strncat(bufferData, _str, MAX_DATA_SIZE);

	strncpy(bufferData, _str, sizeof(bufferData));
	bufferData[sizeof(bufferData)] = '\0';
}
char * LoRaTx::getData(){
	return bufferData;
}
void LoRaTx::sendData(){
	uartSerial.write(bufferData);
}
