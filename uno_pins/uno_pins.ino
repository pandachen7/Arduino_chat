
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println();
  Serial.println(SDA);
  Serial.println(SCL);
  Serial.println();
  Serial.println(A0);
  Serial.println(A1);
  Serial.println(A2);
  Serial.println(A3);
  Serial.println(A4);
  Serial.println(A5);  
}

void loop() {
  // put your main code here, to run repeatedly:

}
