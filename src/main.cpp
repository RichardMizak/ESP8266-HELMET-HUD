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


float prev_accel_mps2 = 0;
float prev_time = 0;
float speed_mps = 0;
float prev_speed = 0;
float kmh = 0;

void kokot();
void display_left();
void display_rigth();

String NTPtime();

DHT dht(DHTPIN, DHTTYPE);
int read_temperature();

const char *ssid     = "TP-Link_A053";
const char *password = "kombajn1";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

Adafruit_MPU6050 mpu;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display1(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D3, D7);
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display2(U8G2_MIRROR, /* reset=*/ U8X8_PIN_NONE, D1, D2);
//U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C display1(U8G2_MIRROR, /* clock=*/ 1, /* data=*/ 2, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C display2(U8G2_MIRROR, /* clock=*/ D4, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  
  Serial.begin(115200);
   while (!Serial)
    delay(10);

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
  

}

void loop() {
  display_left();
  display_rigth();
  kokot();
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
    display2.print(read_temperature());
    display2.print(char(176));
    display2.print("C");
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
  sensors_event_t accel, g ,x;
  mpu.getEvent(&accel, &g, &x);
  float accel_mag = sqrt(pow(accel.acceleration.x, 2) + pow(accel.acceleration.y, 2) + pow(accel.acceleration.z, 2));
  float accel_mps2 = accel_mag / 9.81;
  float curr_time = millis() / 1000.0;
  float dt = curr_time - prev_time;
  speed_mps = accel_mps2 * dt ;
  kmh = (speed_mps * 3600) / 1000;
  prev_time = curr_time;
  return "sda";
}

void kokot() {

  if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
  }

  delay(10);
}