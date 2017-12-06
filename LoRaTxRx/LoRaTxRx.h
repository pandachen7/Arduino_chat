#ifndef LoRaTxRx_h
#define LoRaTxRx_h
#include <SoftwareSerial.h>

typedef unsigned char byte;

class LoRaTxRx
{

  public:
	LoRaTxRx(void);
	LoRaTxRx(int);
	void sendString(const String);
	void sendChars(const char*);
	void sendBytes(const byte*, int);
	
	void setAddrChannel(const byte*);
	void getAddrChannel(byte*);
	void setDefaultAddrChannel();
	void sendAddrChannel();
	
	byte* receiveData();//byte*);
	int getReceiveDataLength();
	byte* getReceiveDataIndex();
	void flushData();
	int available();
	
	String receiveDataToString();
		
  private:
  	byte addrCh[3];//addr x 2, channel x 1
	int maxReceivedSize, receiveLength;
	byte *receivedMsg;
	String readString;

};

#endif