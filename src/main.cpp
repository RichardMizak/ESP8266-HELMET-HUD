#include <Arduino.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
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

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
double get_speed();

SoftwareSerial ss(RXPin, TXPin);

const char *ssid     = "Wavex";
const char *password = "WaveX123";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display1(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C display2(U8G2_MIRROR, /* clock=*/ D4, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  ss.begin(GPSBaud);
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

  
}

void loop() {
   while (ss.available() > 0)
    if (gps.encode(ss.read()))
       display_left();
       display_rigth();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  
  
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

double get_speed(){
  double speed = gps.speed.kmph();
  return speed;
}