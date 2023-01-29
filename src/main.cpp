#include <Arduino.h>
#include <U8g2lib.h>
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

DHT dht(DHTPIN, DHTTYPE);
int read_temperature();

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display1(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display2(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);

void setup() {
Serial.begin(115200);
dht.begin();
//u8g2_SetI2CAddress(&display1, 0x7A);
//u8g2_SetI2CAddress(&display2, 0x78);
display1.begin();
display2.begin();
display1.setFont(u8g2_font_inr30_mf);
display2.setFont(u8g2_font_inr30_mf);
}

void loop() {
  display_left();
  display_rigth();
  
}

void display_left()
{
  display1.firstPage();
  do{
    display1.clearBuffer();
    display1.setCursor(0, 32);
    display1.print("00:00");
    display1.sendBuffer();
  }while (display1.nextPage());  
  delay(2000);
}

void display_rigth()
{
   display2.firstPage();
  do{
    display2.clearBuffer();
    display2.setCursor(0, 32);
    display2.print(read_temperature());
    display2.print(char(176));
    display2.print("C");
    display2.sendBuffer();
  }while (display2.nextPage());  
  delay(2000);
}

int read_temperature(){
  float temperature = dht.readTemperature();
  int temperature_int = (int)temperature;
  Serial.print(temperature_int);
  delay(2000);
  return temperature;
}

