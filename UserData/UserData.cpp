#include "UserData.h"
#include <stdio.h>
#include <string.h>

UserData::UserData(void){
	
}

void UserData::setMyName(){
	userName[0] = 0;
	strcat(userName, "anonymous");
}

void UserData::setMyName(const char * _str){
	userName[0] = 0;
	strcat(userName, _str);
}

char * UserData::sayMyName(){
	return userName;
	
}
