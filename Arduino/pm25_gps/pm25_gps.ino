#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(2, 3);
SoftwareSerial mySerial(4, 5);

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  mySerial.begin(9600);//PM2.5 Sensor*/
}

void loop()
{
  int i=0;
  int a[10]={0};
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float flat=0.0, flon=0.0;
  double pm25,pm10;
  //ss.begin(9600);
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  //ss.end();
  //mySerial.begin(9600);
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while(mySerial.available()>0){
      for(i=0;i<9;i++){
        a[i]=a[i+1];
      }
      a[i]=(int)mySerial.read();
      delay(2);//must do this
      if(a[0]==170){
        //get PM2.5
        pm25=(float)(a[3]*256+a[2])/10;
        pm10=(float)(a[5]*256+a[4])/10;
        if(pm25<0||pm10<0)continue;
      }
    }
  }
  //mySerial.end();
  if (newData)
  {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    flat = flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
    flon = flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
  }

  String out=String(flat)+','+String(flon)+','+String(pm25)+','+String(pm10)+'\n';
  Serial.print(256646);
}
