#ifndef LoRaTxRx_h
#define LoRaTxRx_h
#include <SoftwareSerial.h>

#define MAX_DATA_SIZE 30

class LoRaTxRx
{

  public:
	LoRaTxRx(void);
	LoRaTxRx(int);
	void sendString(const char*);
	void sendBytes(const uint8_t*, int);
	
	void setAddrChannel(const uint8_t*);
	void getAddrChannel(uint8_t*);
	void setDefaultAddrChannel();
	void sendAddrChannel();
	
	/*
	char receiveChar();
	bool receiveData(char*);
	
	*/
	bool receivedFlag();
	String receiveDataToString();
	
	void setData(const char*);
	char* getData();
	void sendData();
	
	
	
  private:
  	uint8_t addrCh[3];//addr x 2, channel x 1
	//int maxReceivedSize, receiveLength;
	//char *receivedMsg;
	//char receivedChar;
	String readString;
	
	char bufferData[MAX_DATA_SIZE + 1];

    //int st; 2 bytes on Uno
};

#endif