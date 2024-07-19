#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_CCS811.h"

#define OLED_DC 9
#define OLED_CS 10
#define OLED_CLK 13
#define OLED_MOSI 11
#define OLED_RESET 8

int measurePin = A1;
int ledPower = 12;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ OLED_CLK, /* data=*/ OLED_MOSI, /* cs=*/ OLED_CS, /* dc=*/ OLED_DC, /* reset=*/ OLED_RESET);
Adafruit_CCS811 ccs;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  u8g2.begin();

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
  }

  while(!ccs.available());
}

void loop() {
  
  ccs.readData();

  voMeasured = analogRead(measurePin);
  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  char co2buff[20]; // Create a buffer to hold the string representation of the CO2 value
  char tvocbuff[20]; // Create a buffer to hold the string representation of the CO2 value
    

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(25,13,"AirView");
    
    u8g2.setFont(u8g2_font_ncenB08_tr);

    size_t co2_value = ccs.geteCO2(); // Get the CO2 value
    sprintf(co2buff, "%zu", co2_value); // Convert the size_t value to a string
    size_t tvoc_value = ccs.getTVOC(); // Get the CO2 value
    sprintf(tvocbuff, "%zu", tvoc_value); // Convert the size_t value to a string

    u8g2.setCursor(5, 30);
    u8g2.print("PPM:");
    u8g2.setCursor(42, 30);
    u8g2.print(ccs.geteCO2());
    u8g2.setCursor(5, 45);
    u8g2.print("TVOC:");
    u8g2.setCursor(42, 45);
    u8g2.print(ccs.getTVOC());
    u8g2.setCursor(5, 60);
    u8g2.print("DUST:");
    u8g2.setCursor(42, 60);
    u8g2.print(dustDensity);
  } while ( u8g2.nextPage() );
    delay(100);
}
