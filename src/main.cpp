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
 Serial.begin(115200); // Initializes the serial communication with baud rate 115200.
 dht.begin(); // Initializes the DHT22 sensor.
 ss.begin(GPSBaud); // Initializes the software serial communication with the GPS module at a baud rate of 9600.
 pinMode(button, INPUT_PULLUP); // Configures the push button as an input with a pull-up resistor.
 display1.begin(); // Initializes the left display using hardware I2C interface.
 display2.begin(); // Initializes the right display using software I2C interface.
 display1.setFlipMode(1); // Sets the flip mode for the left display.
 display1.setFont(u8g2_font_inr30_mf); // Sets the font for the left display.
 display2.setFont(u8g2_font_inr30_mf); // Sets the font for the right display.
}

void loop() {
  while (ss.available() > 0) // Checks if there is any data available in the software serial buffer.
    if (gps.encode(ss.read())) // Decodes the data from the GPS module.
      if (digitalRead(button) == LOW){ // Checks if the push button is pressed.
          display_left(); // Calls the function to display the left content on the right display.
          display_rigth_speed(); // Calls the function to display the speed on the right display.
        }else if (digitalRead(button) == HIGH){ // Checks if the push button is not pressed.
          display_left(); // Calls the function to display the left content on the right display.
          display_rigth_temp(); // Calls the function to display the temperature on the right display.
        }

  if (millis() > 5000 && gps.charsProcessed() < 10) // Checks if GPS module is not detected.
  {
    Serial.println(F("No GPS detected: check wiring.")); // Prints an error message to the serial monitor.
    while(true); // Enters an infinite loop to stop the program execution.
  }
}
 

// Display the current time on the left display
void display_left()
{
  display2.firstPage();
  do{
    display2.clearBuffer();
    display2.setCursor(0, 32);
    get_time(); // Get the current time from the GPS module
    display2.sendBuffer();
  }while (display2.nextPage());  
  delay(2000);
}

// Display the current speed on the right display
void display_rigth_speed()
{
  display1.firstPage();
  do{
    display1.clearBuffer();
    display1.setCursor(0, 32);
    display1.print(get_speed()); // Get the current speed from the GPS module and print it
    display1.sendBuffer();
  }while (display1.nextPage());  
  delay(2000);
}

// Display the current temperature on the right display
void display_rigth_temp()
{
  display1.firstPage();
  do{
    display1.clearBuffer();
    display1.setCursor(0, 32);
    display1.print(read_temperature()); // Read the temperature from the DHT11 sensor and print it
    display1.print(char(176)); // Print the degree symbol
    display1.print("C"); // Print the unit
    display1.sendBuffer();
  }while (display1.nextPage());  
  delay(2000);
}

// Read the temperature from the DHT11 sensor
int read_temperature(){
  float temperature = dht.readTemperature();
  return temperature;
}

// Get the current time from the GPS module
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

// Get the current speed from the GPS module
double get_speed(){
  double speed = gps.speed.kmph();
  return speed;
}