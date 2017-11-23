#ifndef UserData_h
#define UserData_h

class UserData
{

  public:
	UserData();
	void setMyName(void);
	void setMyName(const char*);
	char * sayMyName(void);
	
  private:
  	char registerId[9];
    char userName[21];

    //int st; 2 bytes on Uno
};

#endif