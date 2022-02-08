#include <Arduino.h>
#include "MHZ19.h"
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define RX_PIN 5 //MH-Z19 RX-PIN
#define TX_PIN 4 //MH-Z19 TX-PIN

#define NEO_PIXEL_RING_SIZE 12
#define NEO_PIXEL_PIN D4
#define NEO_BRIGHTNESS 5

#define LOWER_BOUND 400
#define GREEN_BOUND 1000
#define ORANGE_BOUND 1500
#define RED_BOUND 2200

#define MEASURE_DELAY 500

#define BAUDRATE 9600 //Terminal Baudrate

//Klassen Initialisierung
MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_PIXEL_RING_SIZE, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
unsigned long getDataTimer = 0;

uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void unset()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0); //turn every third pixel off
  }
}

void setColor(uint32_t c, uint8_t pixels, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    if (i < pixels) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    } else {
      strip.setPixelColor(i, 0);
    }
    
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
  for (int j = 0; j < 10; j++)
  { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, c); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0); //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
  for (int j = 0; j < 256; j++)
  { // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0); //turn every third pixel off
      }
    }
  }
}

void setup()
{

  // Initialize NeoPixelRing
  strip.begin();
  strip.setBrightness(NEO_BRIGHTNESS);
  rainbow(1);
  unset();
  strip.show();

  //Serielle Intialisierung
  Serial.begin(BAUDRATE);
  mySerial.begin(BAUDRATE);
  myMHZ19.begin(mySerial);
}
void loop()
{
  //Prüfung ob 2 Sekunden um sind.
  if (millis() - getDataTimer >= 2000)
  {
    int CO2;                //Variable darf nur Zahlen enthalten
    CO2 = myMHZ19.getCO2(); //Variable schreiben

    int stepsGreen = (GREEN_BOUND - LOWER_BOUND) / NEO_PIXEL_RING_SIZE;
    int stepsOrange = (ORANGE_BOUND - GREEN_BOUND) / NEO_PIXEL_RING_SIZE;
    int stepsRed = (RED_BOUND - ORANGE_BOUND) / NEO_PIXEL_RING_SIZE;

    if (CO2 < LOWER_BOUND)
    {
      theaterChase(strip.Color(0, 255, 0), 10);
    }
    else if (CO2 < GREEN_BOUND)
    {
      int numPixels = (CO2 - LOWER_BOUND) / stepsGreen;
      Serial.print("Number of NEO Pixels ");
      Serial.println(numPixels);
      setColor(strip.Color(0, 255, 0), numPixels, 50);
    }
    else if (CO2 < ORANGE_BOUND)
    {
      int numPixels = (CO2 - GREEN_BOUND) / stepsOrange;
      Serial.print("Number of NEO Pixels ");
      Serial.println(numPixels);
      setColor(strip.Color(242, 173, 8), numPixels, 50);
    }
    else if (CO2 < RED_BOUND)
    {
      int numPixels = (CO2 - ORANGE_BOUND) / stepsRed;
      Serial.print("Number of NEO Pixels ");
      Serial.println(numPixels);
      setColor(strip.Color(255, 0, 0), numPixels, 50);
    }
    else
    {
      theaterChase(strip.Color(255, 0, 0), 10);
    }

    delay(MEASURE_DELAY);
    //Ausgabe auf den Seriellen Monitor
    Serial.print("CO2 (ppm): ");
    Serial.println(CO2);
    getDataTimer = millis();
  }
}