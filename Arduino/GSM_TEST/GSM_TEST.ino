#include<SoftwareSerial.h>

SoftwareSerial AT(2,3);
String returnStr="";

void blinkled(int led){
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
}

void tribleblinkled(int led){
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
}

bool A6Command(String Command){
  int i=0;
  AT.print(Command);
  for(int i=0;i<500;i++){
    if(AT.available()>0){
      returnStr=AT.readString();
      
      //blinkled(13);
      return true;
   }
   delay(20);
  }
}

void setup() {
  // put your setup code here, to run once:
/*AT+CGACT=1,1
AT+CIPSTART=TCP,39.108.108.117,8080
AT+CIPSEND
GET /PM2.5_war/insertdata?poiNo=001&pm25=21.5&pm10=35.7&temperature=12.43&pressure=1013.4&humidity=61.4 HTTP/1.1  \r\nHost: 39.108.108.117:8080\r\n\r\n
Host: 39.108.108.117:8080
*/
  //Serial.begin(115200);
  delay(5000);
  AT.begin(115200);
  delay(10000);
  Serial.write("AT\r\n");
  A6Command("AT+CGATT=1\r\n");
  delay(10000);
  A6Command("AT+CGACT=1,1\r\n");
  delay(10000);
  A6Command("AT+CIPSTART=TCP,39.108.108.117,8080\r\n");
  delay(1000);
  A6Command("AT+CIPSEND\r\n");
  delay(50);
  AT.print("GET /PM2.5_war/insertdata?poiNo=001&pm25=21.5&pm10=35.7&temperature=12.43&pressure=1013.4&humidity=61.4 HTTP/1.1\r\nHost: 39.108.108.117:8080\r\n\r\n\r\n");
  delay(1000);
  tribleblinkled(13);
}

void loop() {
  // put your main code here, to run repeatedly:

}
