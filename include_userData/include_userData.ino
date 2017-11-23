#include <UserData.h>

UserData uData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  uData.setMyName();
  Serial.write("My name: \n");
  //Serial.write(uData.userName); //access denied
  Serial.println(uData.sayMyName());

  uData.setMyName("Walter");
  Serial.write("My name: \n");
  Serial.println(uData.sayMyName());
}

void loop() {
  // put your main code here, to run repeatedly:

}
