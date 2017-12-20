#include <LoRaTxRx.h>
/*
資料來源
https://www.youtube.com/watch?v=vTSLijvnVxg
輸出要接在 pin 2
*/
int i = 0;      // used if calculations are made

int ax = 0; // hundreds .... sadat
int bx = 0; // tens ... kymmenet
int cx = 0; // ones ... ykköset
int dx = 0;
int ex = 190;
int top = 0;
int mov = 0;
int ix = 0;
int kx = 0;
int red = 20;
int green = 70;
int blue = 0;

float dd = 0.0; 
float ee = 0.0;
float ff = 0.0;
float gg = 0.0;


float a = 0.0;
float b = 0.0;
float c = 0.0;

float d = 0.0;  // using floats one can have readings with 
float e = 0.0;  // decimal point such as 65.3 km/h
                // with floats a decimal point should be 

////////////LoRa
const byte MD0_pin = 8, MD1_pin = 9;
LoRaTxRx myLora(51);//with receive buffer
char bufferSendStr[51];//send buffer
////////////LoRa ^
unsigned long detectionTimes; //from 0 to 4,294,967,295 (2^32 - 1).

float mm = 51308.0; // With 10.525Ghz HB100 and desired km/h
// Value is = 51308.0 .... for Mph value is 31888.0
// Formula for value is 1000000 / Fd without V for exmple 1000000/31.36 = 31888
/* 
Frequency  Fd (V in Km/hr)    Fd (V in mph)
9.35 GHz        17.31V          27.85V
9.9 GHz         18.33V          29.49V
10.525 GHz      19.49V          31.36V
10.587 GHz      19.60V          31.54V
10.687 GHz      19.79V          31.84V
24.125 GHz      44.68V          71.89V

*/
char * floatToString(char * outstr, float value, int places, int minwidth=0, bool rightjustify=false) {
    // this is used to write a float value to string, outstr.  oustr is also the return value.
    int digit;
    float tens = 0.1;
    int tenscount = 0;
    int i;
    float tempfloat = value;
    int c = 0;
    int charcount = 1;
    int extra = 0;
    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
    // if this rounding step isn't here, the value  54.321 prints as 54.3209

    // calculate rounding term d:   0.5/pow(10,places)  
    float d = 0.5;
    if (value < 0)
        d *= -1.0;
    // divide by ten for each decimal place
    for (i = 0; i < places; i++)
        d/= 10.0;    
    // this small addition, combined with truncation will round our values properly 
    tempfloat +=  d;

    // first get value tens to be the large power of ten less than value    
    if (value < 0)
        tempfloat *= -1.0;
    while ((tens * 10.0) <= tempfloat) {
        tens *= 10.0;
        tenscount += 1;
    }

    if (tenscount > 0)
        charcount += tenscount;
    else
        charcount += 1;

    if (value < 0)
        charcount += 1;
    charcount += 1 + places;

    minwidth += 1; // both count the null final character
    if (minwidth > charcount){        
        extra = minwidth - charcount;
        charcount = minwidth;
    }

    if (extra > 0 and rightjustify) {
        for (int i = 0; i< extra; i++) {
            outstr[c++] = ' ';
        }
    }

    // write out the negative if needed
    if (value < 0)
        outstr[c++] = '-';

    if (tenscount == 0) 
        outstr[c++] = '0';

    for (i=0; i< tenscount; i++) {
        digit = (int) (tempfloat/tens);
        itoa(digit, &outstr[c++], 10);
        tempfloat = tempfloat - ((float)digit * tens);
        tens /= 10.0;
    }

    // if no places after decimal, stop now and return

    // otherwise, write the point and continue on
    if (places > 0)
    outstr[c++] = '.';


    // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
    for (i = 0; i < places; i++) {
        tempfloat *= 10.0; 
        digit = (int) tempfloat;
        itoa(digit, &outstr[c++], 10);
        // once written, subtract off that digit
        tempfloat = tempfloat - (float) digit; 
    }
    if (extra > 0 and not rightjustify) {
        for (int i = 0; i< extra; i++) {
            outstr[c++] = ' ';
        }
    }


    outstr[c++] = '\0';
    return outstr;
}

void setup()
{
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);

  ////////////LoRa
  pinMode(MD0_pin, OUTPUT);
  pinMode(MD1_pin, OUTPUT);
  digitalWrite(MD0_pin, LOW);
  digitalWrite(MD1_pin, LOW);
  ////////////LoRa ^
  detectionTimes = 0;
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println ("Connection OK");
}
void loop()

{   
 //利用脈衝來讀取HB100訊號
 while (dd < 4.0){
     a = pulseIn(2, HIGH, 50000); // loop here in while-loop
    b = pulseIn(2, LOW, 50000);  //  if no movenment detected
    dd = mm/(a+b);  
    } // go out of while-loop when dd = > 4
  //read input 4 times with 50mS delay between reads
    a = pulseIn(2, HIGH, 50000); // wait 50000uS for pulse (0,05 sec)
    b = pulseIn(2, LOW, 50000);  // to complete (over 10Hz needed)
    dd = mm/(a+b);              // as 0.05 LOW + 0.05 HIGH = 0.1 sec
  delay (50);
    a = pulseIn(2, HIGH, 50000);
    b = pulseIn(2, LOW, 50000);
    ee = mm/(a+b);
  delay (50);
    a = pulseIn(2, HIGH, 50000);
    b = pulseIn(2, LOW, 50000);
    ff = mm/(a+b);
  delay (50);
    a = pulseIn(2, HIGH, 50000);
    b = pulseIn(2, LOW, 50000);
    gg = mm/(a+b);

// Make strange misread values like 4158344 to zeros ( 0 )
if (dd > 255.0) {
  dd = 0.0;
}
if (ee > 255.0) {
  ee = 0.0;
}
if (ff > 255.0) {
  ff = 0.0;
}
if (gg > 255.0) {
  gg = 0.0;
}

 //**********"BubbleSort" the readings dd highest gg lowest
     for (i = 0; i<4; i++) {
          if (dd > ee) {
        d = dd;
        e = ee;
        ee = d;
        dd = e;
      }
          if (ee > ff) {
        d = ee;
        e = ff;
        ee = e;
        ff = d;
      }
          if (ff > gg) {
        d = ff;
        e = gg;
        ff = e;
        gg = d;
      }
     }

 c = (ee + ff)/2;
 // ignore highes and lowest value dd and gg ****************
   //  and continue with average of ee and ff   
    
   if (c > 999.0) {  //prints ovf instead of 0 without this
    c = 0.0; // not needed when using low limit in next if-sentence
      }
            
 if (((ff - ee) > 4.0) || ((ee - ff) > 4.0)) {
   c = 0.0;   // filter strange things
 }         // if ff differs from ee more than 4
      
      
  if ((c > 4.0) && c < (255.0))  { // If reading is over 4 and less than 
    detectionTimes++;
    sprintf(bufferSendStr, "%lu,\t", detectionTimes);

    Serial.print(bufferSendStr);
    Serial.print(c, 2);              //255 then print it to serial
    Serial.print(" km/h \r\n");

    ////////////LoRa
    myLora.sendString(bufferSendStr);
    // send the same message to others
    floatToString(bufferSendStr, c , 2);
    myLora.sendString(bufferSendStr);
    myLora.sendString(" km/h \r\n");
    ////////////LoRa ^
    
  }
  delay(20);
}


