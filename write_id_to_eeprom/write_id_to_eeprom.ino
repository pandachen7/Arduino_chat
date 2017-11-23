#include <EEPROM.h>


int a = 0;
int value;

static int boardIdBaseAddress = 0; // or whatever you want
static int randomSeedValue = 127;

unsigned long setupEeprom()
{
  byte b1 = EEPROM.read(boardIdBaseAddress);
  byte b2 = EEPROM.read(boardIdBaseAddress + 1);
  byte b3 = EEPROM.read(boardIdBaseAddress + 2);
  byte b4 = EEPROM.read(boardIdBaseAddress + 3);
//  write or update
  if ((b1 == 0xff) && (b2 == 0xff) && (b3 == 0xff) && (b4 == 0xff))
  {
    b1 = random(256);
    b2 = random(256);
    b3 = random(256);
    b4 = random(256);
    EEPROM.write(boardIdBaseAddress, b1); 
    EEPROM.write(boardIdBaseAddress + 1, b2); 
    EEPROM.write(boardIdBaseAddress + 2, b3); 
    EEPROM.write(boardIdBaseAddress + 3, b4); 
  }
    b1 = random(256);
    b2 = random(256);
    b3 = random(256);
    b4 = random(256);
    EEPROM.write(boardIdBaseAddress, b1); 
    EEPROM.write(boardIdBaseAddress + 1, b2); 
    EEPROM.write(boardIdBaseAddress + 2, b3); 
    EEPROM.write(boardIdBaseAddress + 3, b4); 
}

void setup()
{
  randomSeed(analogRead(randomSeedValue));
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  setupEeprom();
}

void loop()
{
  value = EEPROM.read(a);

  Serial.print(a);
  Serial.print("\t");
  Serial.print(value);
  Serial.print(" = ");
  Serial.print(String(value, HEX));
  Serial.println();

  a = a + 1;

  if (a == 4){
    a = 0;
    delay(50000);
  }

  delay(500);
}

