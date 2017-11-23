//This is for Arduino Uno

#include <SoftwareSerial.h>
//#include <Timer.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>

//#define AS32TTL_MD1_PIN 9
#define MAX_INPUT_SIZE 30//Do not exceed 255
#define NUM_ARGUMENTS 1
#define LENGTH_ARGUMENTS 10

//boolean bDebug = true, bViewHex = false;

File myFile;
//Timer timer;

//unsigned int currentTime = 0;
//unsigned int countCallBack = 0;
//int auxPin = A0;
//int sensorValue = 0;
byte receiveLength = 0;
char cmdStr[MAX_INPUT_SIZE + 1];
char receivedMsg[MAX_INPUT_SIZE + 1];
char sendMeta[40];
//byte addrChannel[] = {0x0, 0x0, 0x23};//default addr = 0, channel = 23


SoftwareSerial uartSerial(2, 4); // arduino's RX, TX, can be digital pin
DS3231  rtc(SDA, SCL);

class UserData
{
  //
  //size: 20 bytes
  public:
    char registerId[9];
    char userName[11];
    //int st; 2 bytes on Uno
    
  private:
};

UserData userdata;

char c2h(char c)
{  
  return "0123456789ABCDEF"[0x0F & (unsigned char)c];
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //sd part///////////////
  if (!SD.begin(10)) {
    Serial.write("Initializing SD card failed!\n");
    return;
  }
  loadUserData();
  ////////////////////////

  //rtc part//////////////
  rtc.begin();
//  rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(9, 0, 0);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(29, 11, 2017);   // Set the date to January 1st, 2014
  ////////////////////////

  //UART part/////////////
  uartSerial.begin(9600);
  ////////////////////////
  
  //load my id
  int value;
  for(int i = 0 ; i < 4 ; i++){
    value = EEPROM.read(i);

    //Serial.println(value,HEX);

    userdata.registerId[i * 2] = c2h(value>>4);
    userdata.registerId[i * 2 + 1] = c2h(value);
  }
  userdata.registerId[8] = 0;
  //memcpy( userdata.userName, userdata.registerId, 9*sizeof(char) );

  Serial.write("Your name is: ");
  sayMyName();
  Serial.write("\nWellcome to Chat rooM!!\n");

  //timer.every(5000, callback);//call 'callback' per N miniseconds
}

void loop() {
//  currentTime = micros();
  
  //timer.update();
  if(uartSerial.available()){
    receiveFromUart();
  }
  
  if (Serial.available()) {
    sendToUart();
  }

//  sensorValue = analogRead(auxPin);
//  Serial.println(sensorValue);
  
//  currentTime = micros() - currentTime;
//  Serial.println(currentTime, DEC);

  delay(100);
}
void loadUserData(){
  myFile = SD.open("user.txt");
  if (myFile) {
    //Serial.println("user.txt:");

    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }

    byte inputSize = myFile.readBytes(userdata.userName, 10);
    userdata.userName[inputSize] = '\0';//end the C string
  
    // close the file:
    myFile.close();
  } 
}
void saveUserData(){
  //myFile = SD.open("user.txt", FILE_WRITE);
  myFile = SD.open("user.txt", O_READ | O_WRITE | O_TRUNC);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(userdata.userName);
    myFile.close();
    Serial.write("\nYour name now is ");
    sayMyName();
    Serial.write("\n");
  }else{
    Serial.write("error opening user.txt\n");
  }
}
void sayMyName(){
  Serial.write(userdata.userName);
  Serial.write(" (");
  Serial.write(userdata.registerId);
  Serial.write(")");
}
//void sendTime(){
//  uartSerial.print(t.hour, DEC);
//  uartSerial.write(":");
//  uartSerial.print(t.min, DEC);
//  uartSerial.write(":");
//  uartSerial.print(t.sec, DEC);
//  uartSerial.write("\t");
//}
//void sendMyName(){
//  uartSerial.write(userdata.userName);
//  uartSerial.write(" (");
//  uartSerial.write(userdata.registerId);
//  uartSerial.write(") says: ");
//}
void receiveFromUart(){
  
  while(uartSerial.available() && receiveLength < MAX_INPUT_SIZE){
    receivedMsg[receiveLength++] = uartSerial.read();
  }
  
  if(receivedMsg[receiveLength] != '\0'){
    //last is not '\0' (Ending)
    return;
  }
  
//  if(bViewHex){
//    Serial.write("\nReceived string in hex\n");
//    for(int i = 0 ; i < receiveLength ; i++){
//      Serial.write("0x");
//      Serial.print(receivedMsg[i], HEX);
//      Serial.write("\n");
//    }
//  }
  
//  Serial.write("\nget: ");
  if(receivedMsg[0] == 5){
    //ignore...
  }else if(receivedMsg[0] == 6){
    //sync time
    rtc.setTime(receivedMsg[1] - 33, receivedMsg[2] - 33, receivedMsg[3] - 33);//33 = 0, 92 = 59
    
    Time t = rtc.getTime();
    Serial.write("Now is ");
    Serial.print(t.hour, DEC);
    Serial.print(" hour(s), ");
    Serial.print(t.min, DEC);
    Serial.print(" minute(s) and ");
    Serial.print(t.sec, DEC);
    Serial.println(" second(s).");
  }else{
    //message from others
    Serial.write(receivedMsg);
  }

  receiveLength = 0;
  receivedMsg[0] = '\0';
}
void sendToUart(){

  char argv[NUM_ARGUMENTS][LENGTH_ARGUMENTS+1], tmpChar;
  
  int iInput = 0, iArgu = 0, iStr = 1, iChar = 0;
  byte inputSize = Serial.readBytes(cmdStr, MAX_INPUT_SIZE);
  cmdStr[inputSize] = '\0';//end the C string
//  if(bViewHex){
//    Serial.write("Input string in hex\n");
//    for(int i = 0 ; i < inputSize ; i++){
//      Serial.write("0x");
//      Serial.print(cmdStr[i], HEX);
//      Serial.write("\n");
//    }
//    return;
//  }
  //special case
  if(cmdStr[0] == '>'){
    char sendCmd[MAX_INPUT_SIZE];
//    if(addrChannel[0] != 0 || addrChannel[1] != 0){//specific address
//      uartSerial.write(addrChannel, sizeof(addrChannel));
//    }
    memcpy( sendCmd, &cmdStr[1], inputSize - 1 );
    sendCmd[inputSize - 1] = '\0';
    sendMeta[0] = '\0';
    
    Time t = rtc.getTime();
    char tmpStr[5];
    //9 bytes
    sprintf(tmpStr, "%d", t.hour);
    strcat(sendMeta, tmpStr);
    strcat(sendMeta, ":");
    sprintf(tmpStr, "%d", t.min);
    strcat(sendMeta, tmpStr);
    strcat(sendMeta, ":");
    sprintf(tmpStr, "%d", t.sec);
    strcat(sendMeta, tmpStr);
    strcat(sendMeta, "\n");

    //Max 28 bytes
    strcat(sendMeta, userdata.userName);
    strcat(sendMeta, " (");
    strcat(sendMeta, userdata.registerId);
    strcat(sendMeta, ") says: ");

    uartSerial.write(sendMeta);
    uartSerial.write(sendCmd);
    
    Serial.write(sendMeta);
    Serial.write(sendCmd);
//    sayMyName();
//    Serial.write(" says: ");
//    Serial.write(sendCmd);
    
//      if(addrChannel[0] != 0 || addrChannel[1] != 0){//specific address
//        Serial.write("' to 0x");
//        Serial.print(addrChannel[0], HEX);
//        Serial.write(", 0x");
//        Serial.print(addrChannel[1], HEX);
//      }else{
//        Serial.write("' (broadcast)");
//      }
    Serial.write("\n");
  
    return;
  }
  //init
  for (int i = 0 ; i < NUM_ARGUMENTS ; i++){
    argv[i][0] = 0; 
  }
  
  //split cmdStr to arguments
  while(cmdStr[iStr] != '\0'){
    tmpChar = cmdStr[iStr++];
    if(iArgu >= NUM_ARGUMENTS) {Serial.write("too many arguments\n");return;}
    if(tmpChar == ' ' || tmpChar == ',' || iChar >= LENGTH_ARGUMENTS){
      argv[iArgu][iChar] = 0;
      iChar = 0;
      iArgu ++;
      continue;
    }
    
    argv[iArgu][iChar++] = tmpChar;
  }
  argv[iArgu][iChar] = 0;
  unsigned int uiAddr;//for temp
  switch(cmdStr[0]){
    case 'h':
      if(strcmp(argv[0], "elp") == 0){//CMD:'help'
        Serial.write("Command list\n");
        Serial.write("temperature:\tShow temperature\n");
        Serial.write("u:\t\tChange your user name, e.g. upanda for name 'panda'\n");
        Serial.write("now:\t\tShow current time\n");
        Serial.write("sync:\t\tsynchronize your time\n");
        Serial.write(">:\t\tSend message to every one, e.g. >test\n");
      }else{
        showUndefined(cmdStr);
      }
      break;
    case 't'://for test that you can see arguments you input
//      Serial.write("arguments are\n");
//      Serial.write(argv[0]);
//      Serial.write("\n");
//      Serial.write(argv[1]);
//      Serial.write("\n");
//      Serial.write(argv[2]);
//      Serial.write("\n");
      if(strcmp(argv[0], "emperature") == 0){//CMD:'temperature'
        Serial.write("\nNow the temperature is ");
        Serial.print(rtc.getTemp());
        Serial.write("\n");
      }else{
        showUndefined(cmdStr);
//        if(strcmp(userdata.registerId, "A436ABA3") == 0){
//          Serial.write("test OK\n");
//        }else{
//          Serial.write("test NG\n");
//        }
      }
      
      
      break;
    case 'u'://e.g. 'upanda', that means your user name will be 'panda'
      memcpy(userdata.userName, argv[0], 10);
      userdata.userName[10] = 0;
      saveUserData();
      break;
    case 'n':
      if(strcmp(argv[0], "ow") == 0)//CMD:'now'
      {
        Time t = rtc.getTime();
        Serial.write("Now is ");
        Serial.print(t.hour, DEC);
        Serial.print(" hour(s), ");
        Serial.print(t.min, DEC);
        Serial.print(" minute(s) and ");
        Serial.print(t.sec, DEC);
        Serial.println(" second(s).");
      }else{
        showUndefined(cmdStr);
      }
      break;
    case 's':
      if(strcmp(argv[0], "ync") == 0)//CMD:'sync'
      {
        byte req[] = {5, 0};
        uartSerial.write(req, sizeof(req));
        Serial.write("synchronizing...\n");

        
      }else{
        showUndefined(cmdStr);
      }
      break;
//    case 's'://set address and channel, default channel is 23
//      if(!isNumeric(argv[0]) || !isNumeric(argv[1])){
//        Serial.write("argument 1 or 2 is not a number!\n");
//        break;
//      }
//      uiAddr = atoi(argv[0]) % 65536;//Channel should not greater than 65535
//      addrChannel[0] = int(uiAddr / 256);//high addr
//      addrChannel[1] = uiAddr % 256;//low addr
//      addrChannel[2] = atoi(argv[1]) % 32;//Channel should not greater than 31
//
//      if (bDebug){
//        Serial.write("now addr is ");
//        Serial.write(uiAddr);
//        Serial.write(" (0x");
//        Serial.print(addrChannel[0], HEX);
//        Serial.write(", 0x");
//        Serial.print(addrChannel[1], HEX);
//        Serial.write(")\nand channel is ");
//        Serial.write(addrChannel[2]);
//        Serial.write("\n");
//      }
//
//      break;
    default:
      showUndefined(cmdStr);
      break;
  }
}
void showUndefined(const char * _str){
  Serial.write("Undefined command '");
  Serial.write(_str);
  Serial.write("'\n");
}
void callback(){
//  uartSerial.print(countCallBack++);
//  uartSerial.println(". sending...");
//
//  if (countCallBack > 65530){
//    countCallBack = 0;
//  }

}


////////////////////////////////////////////
//useful functions
boolean isNumeric(String str) {
    unsigned int stringLength = str.length();
 
    if (stringLength == 0) {
        return false;
    }
// Serial.print("str=");
// Serial.println(str);
// Serial.print("stringLength=");
// Serial.println(stringLength);
    boolean seenDecimal = false;
 
    for(unsigned int i = 0; i < stringLength; ++i) {
        if (isDigit(str.charAt(i))) {
            continue;
        }
 
        if (str.charAt(i) == '.') {
            if (seenDecimal) {
                return false;
            }
            seenDecimal = true;
            continue;
        }
        return false;
    }
    return true;
}
