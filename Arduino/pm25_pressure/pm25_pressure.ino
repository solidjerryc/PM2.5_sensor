#include <SFE_BMP180.h>
#include <DHT12.h>
#include <Wire.h>
#include <U8glib.h>
#include <SoftwareSerial.h>

//Software Serial port for PM2.5 sensor
SoftwareSerial mySerial(2, 3);
// IIC Screen 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
// You will need to create an SFE_BMP180 object, here called "pressure":
SFE_BMP180 pressure;
DHT12 dht12;
String t1,t2,t3,t4;

//float pm25=0.0;

void draw(void) {
  // 设定字体->指定输出位置->输出数据
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 16);
  u8g.print("T0:"); 
  u8g.setPrintPos(40, 16); 
  u8g.print(t1+"C");
  u8g.setPrintPos(0, 32);
  u8g.print("P:"); 
  u8g.setPrintPos(40, 32); 
  u8g.print(t2+"hPa");
  u8g.setPrintPos(0, 48);
  u8g.print("PM25:"); 
  u8g.setPrintPos(40, 48); 
  u8g.print(t3+"ug/m3");
  u8g.setPrintPos(0, 64);
  u8g.print("H:"); 
  u8g.setPrintPos(40, 64); 
  u8g.print(t4+"%");
}

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  if (pressure.begin())
  {  ;}
  else
  {
    while(1); // Pause forever.
  }
 
  //SET SCEEEN COLOR
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int num=10;//average no
  int sig=0;
  int i=0;
  int a[10]={0};
  char status;
  double T,P,H,pm25,pm10;
  double T_avg=0,P_avg=0,H_avg=0,pm25_avg=0,pm10_avg=0;
  while(sig<10)
  {
  while(mySerial.available()>0){
    /*********************
    * get PM2.5 from Serial port per second, while data recieved, Arduino reads pressure, temperature and humidity from sensor.
    *
    */
      for(i=0;i<9;i++){
        a[i]=a[i+1];
      }
      a[i]=(int)mySerial.read();
      delay(2);//must do this
      //Serial.println(a[i]);
      if(a[0]==170){
        //get PM2.5
        pm25=(float)(a[3]*256+a[2])/10;
        pm10=(float)(a[5]*256+a[4])/10;
        if(pm25<0||pm10<0)continue;
        //get temperature and pressure
        status = pressure.startTemperature();
        if (status != 0)
        {
          delay(status);
          status = pressure.getTemperature(T);
        }
        status = pressure.startPressure(3);
        if (status != 0)
        {
          delay(status);
          status = pressure.getPressure(P,T);
        }

        H=dht12.readHumidity();//get Humidity

        //sum
        T_avg+=T;
        P_avg+=P;
        H_avg+=H;
        pm25_avg+=pm25;
        pm10_avg+=pm10;
        sig++;
        //convert double to string
        t1=String(T);
        t2=String(P);
        t3=String(pm25);
        t4=String(H);
        
        //print result on the screen
        u8g.firstPage();  
        do {
          draw();
        } while( u8g.nextPage() );
      } 
    }
  }
  Serial.println(T_avg/num);
  Serial.println(P_avg/num);
  Serial.println((double)H_avg/num);
  Serial.println(pm25_avg/num);
  Serial.println(pm10_avg/num);
  Serial.println(' ');
}
