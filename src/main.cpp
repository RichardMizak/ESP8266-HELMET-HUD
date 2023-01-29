#include <Arduino.h>
#include <U8g2lib.h>
//#include "DHT.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//#define DHTPIN 2
//#define DHTTYPE DHT22

//DHT dht(DHTPIN, DHTTYPE);

void display_left();
void display_rigth();
const char* read_temperature();

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display1(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display2(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);

void setup() {
//dht.begin();
//u8g2_SetI2CAddress(&display1, 0x7A);
//u8g2_SetI2CAddress(&display2, 0x78);
display1.begin();
display2.begin();
display1.setFont(u8g2_font_fub30_tf);
display2.setFont(u8g2_font_fub30_tf);

}

void loop() {
  display_left();
  display_rigth();
  
}

void display_left(){
  display1.firstPage();
  do{
    display1.clearBuffer();
    display1.drawStr(0,32,"1111");
    display1.sendBuffer();
  }while (display1.nextPage());  
  delay(1000);
}

void display_rigth(String text){
   display2.firstPage();
  do{
    display2.clearBuffer();
    display2.drawStr(0,32, "OOOO");
    display2.sendBuffer();
  }while (display2.nextPage());  
  delay(1000);
}

