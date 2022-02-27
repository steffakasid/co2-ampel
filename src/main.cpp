#include <Arduino.h>
#include <MHZ19.h>
#include <SoftwareSerial.h>
#include <NeoPixel.h>

#include <U8g2lib.h> // U8glib library for the OLED you download below
#include <Wire.h>    // Set Wire library for I2C communication
#include <SPI.h>

#define CLK_PIN D1
#define VID_PIN D1

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE, CLK_PIN, VID_PIN);

#define RX_PIN D7 //MH-Z19 RX-PIN
#define TX_PIN D8 //MH-Z19 TX-PIN

#define NEO_PIXEL_RING_SIZE 12
#define NEO_PIXEL_PIN D3
#define NEO_BRIGHTNESS 5

#define LOWER_BOUND 400
#define GREEN_BOUND 1000
#define ORANGE_BOUND 1500
#define RED_BOUND 2200

#define MEASURE_DELAY 500

#define BAUDRATE 9600

MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN);
NeoPixel neoPixel = NeoPixel(NEO_PIXEL_RING_SIZE, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
unsigned long getDataTimer = 0;

void setup()
{
  neoPixel.begin();
  neoPixel.setBrightness(NEO_BRIGHTNESS);

  Serial.begin(BAUDRATE);
  mySerial.begin(BAUDRATE);
  myMHZ19.begin(mySerial);
  myMHZ19.autoCalibration(false);

  Serial.begin(9600);
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);    

  u8x8.inverse();
}

void loop()
{
  if (millis() - getDataTimer >= 2000)
  {              
    int CO2 = myMHZ19.getCO2();
    u8x8.println("CO2 (ppm) "+ CO2);

    int stepsGreen = (GREEN_BOUND - LOWER_BOUND) / NEO_PIXEL_RING_SIZE;
    int stepsOrange = (ORANGE_BOUND - GREEN_BOUND) / NEO_PIXEL_RING_SIZE;
    int stepsRed = (RED_BOUND - ORANGE_BOUND) / NEO_PIXEL_RING_SIZE;

    if (CO2 < LOWER_BOUND)
    {
      neoPixel.theaterChase(neoPixel.Color(0, 255, 0), 10);
    }
    else if (CO2 < GREEN_BOUND)
    {
      int numPixels = (CO2 - LOWER_BOUND) / stepsGreen;
      Serial.print("Number of NEO Pixels ");
      u8x8.print("Number of NEO Pixels ");
      Serial.println(numPixels);
      u8x8.println(numPixels);
      neoPixel.setColor(neoPixel.Color(0, 255, 0), numPixels, 50);
    }
    else if (CO2 < ORANGE_BOUND)
    {
      int numPixels = (CO2 - GREEN_BOUND) / stepsOrange;
      Serial.print("Number of NEO Pixels ");
      u8x8.print("Number of NEO Pixels ");
      Serial.println(numPixels);
      u8x8.println(numPixels);
      neoPixel.setColor(neoPixel.Color(255, 255, 0), numPixels, 50);
    }
    else if (CO2 < RED_BOUND)
    {
      int numPixels = (CO2 - ORANGE_BOUND) / stepsRed;
      Serial.print("Number of NEO Pixels ");
      u8x8.print("Number of NEO Pixels ");
      Serial.println(numPixels);
      u8x8.println(numPixels);
      neoPixel.setColor(neoPixel.Color(255, 0, 0), numPixels, 50);
    }
    else
    {
      neoPixel.theaterChase(neoPixel.Color(255, 0, 0), 10);
    }

    delay(MEASURE_DELAY);

    Serial.printf("CO2 (ppm): %d\n", CO2);
    getDataTimer = millis();
  }
}