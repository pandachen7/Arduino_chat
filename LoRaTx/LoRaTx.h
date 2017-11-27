#ifndef LoRaTx_h
#define LoRaTx_h
#include <SoftwareSerial.h>

#define MAX_DATA_SIZE 30

class LoRaTx
{

  public:
	LoRaTx(void);
	void sendString(const char*);
	void sendBytes(const uint8_t*);
	
	void setAddrChannel(const uint8_t*);
	void getAddrChannel(uint8_t*);
	void setDefaultAddrChannel();
	void sendAddrCh();
	
	void setData(const char*);
	char* getData();
	void sendData();
	
  private:
  	char bufferData[MAX_DATA_SIZE + 1];
	uint8_t addrCh[3];//addr x 2, channel x 1

	
    

    //int st; 2 bytes on Uno
};

#endif