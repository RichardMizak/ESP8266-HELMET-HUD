#include <Arduino.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define DHTPIN 12
#define DHTTYPE DHT22

void display_left();
void display_rigth();
String NTPtime();

DHT dht(DHTPIN, DHTTYPE);
int read_temperature();

const char *ssid     = "Wavex";
const char *password = "WaveX123";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

Adafruit_MPU6050 mpu;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display1(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display2(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);
//U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C display1(U8G2_MIRROR, /* clock=*/ 1, /* data=*/ 2, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C display2(U8G2_MIRROR, /* clock=*/ D4, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  //u8g2_SetI2CAddress(&display1, 0x7A);
  //u8g2_SetI2CAddress(&display2, 0x78);
  display1.begin();
  display2.begin();
  display1.setFont(u8g2_font_inr30_mf);
  display2.setFont(u8g2_font_inr30_mf);
  /*
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  */
}

void loop() {
  display_left();
  display_rigth();
  
}

void display_left()
{
  display2.firstPage();
  do{
    display2.clearBuffer();
    display2.setCursor(0, 32);
    display2.print(NTPtime());
    display2.sendBuffer();
  }while (display2.nextPage());  
  delay(2000);
}

void display_rigth()
{
   display1.firstPage();
  do{
    display1.clearBuffer();
    display1.setCursor(0, 32);
    display1.print(read_temperature());
    display1.print(char(176));
    display1.print("C");
    //display1.print(get_acceleration());
    display1.sendBuffer();
  }while (display1.nextPage());  
  delay(2000);
}

int read_temperature(){
  float temperature = dht.readTemperature();
  int temperature_int = (int)temperature;
  Serial.print(temperature_int);
  delay(2000);
  return temperature;
}

String NTPtime(){
  timeClient.setTimeOffset(3600);
  timeClient.update();
  String x_time = timeClient.getFormattedTime();
  delay(1000);
  return x_time;
}

String get_acceleration(){
  char* speed;
  if(mpu.getMotionInterruptStatus()) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float x = a.acceleration.x;
    float y = a.acceleration.y;
    float z = a.acceleration.z;
    sprintf(speed, "%f, %f, %f" ,x,y,z);
  }
  return speed;
}