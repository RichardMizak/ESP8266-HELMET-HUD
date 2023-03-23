#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>

#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int button = D1;
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void display_left();
void display_rigth_speed();
void display_rigth_temp();
int read_temperature();
double get_speed();
void get_time();

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display1(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D3, D7);
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C display2(U8G2_MIRROR, /* clock=*/ D4, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  ss.begin(GPSBaud);
  pinMode(button, INPUT_PULLUP);
  display1.begin();
  display2.begin();
  display1.setFont(u8g2_font_inr30_mf);
  display2.setFont(u8g2_font_inr30_mf);  
}

void loop() {
   while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if (digitalRead(button) == LOW){
          display_left();
          display_rigth_speed();
        }else if (digitalRead(button) == HIGH){
          display_left();
          display_rigth_temp();
        }
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
    get_time();
    display2.sendBuffer();
  }while (display2.nextPage());  
  delay(2000);
}

void display_rigth_speed()
{
  display1.firstPage();
  do{
    display1.clearBuffer();
    display1.setCursor(0, 32);
    display1.print(get_speed());
    display1.sendBuffer();
  }while (display1.nextPage());  
  delay(2000);
}

void display_rigth_temp()
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
  return temperature;
}

void get_time(){
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) display2.print(F("0"));
    display2.print(gps.time.hour() + 1);
    display2.print(F(":"));
    if (gps.time.minute() < 10) display2.print(F("0"));
    display2.print(gps.time.minute());
  }
}

double get_speed(){
  double speed = gps.speed.kmph();
  return speed;
}