#include "NeoPixel.h"

NeoPixel::NeoPixel(uint16_t size, int16_t pin, neoPixelType type): Adafruit_NeoPixel(size, pin, type){}

void NeoPixel::unset()
{
    for (uint16_t i = 0; i < this->numPixels(); i++)
    {
        this->setPixelColor(i, 0);
    }
}

void NeoPixel::setColor(uint32_t c, uint8_t pixels, uint8_t wait)
{
    for (uint16_t i = 0; i < this->numPixels(); i++)
    {
        if (i < pixels)
        {
            this->setPixelColor(i, c);
            this->show();
            delay(wait);
        }
        else
        {
            this->setPixelColor(i, 0);
        }
    }
}

void NeoPixel::colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < this->numPixels(); i++)
    {
        this->setPixelColor(i, c);
        this->show();
        delay(wait);
    }
}

void NeoPixel::rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < this->numPixels(); i++)
        {
            this->setPixelColor(i, Wheel((i + j) & 255));
        }
        this->show();
        delay(wait);
    }
}

void NeoPixel::rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < this->numPixels(); i++)
        {
            this->setPixelColor(i, Wheel(((i * 256 / this->numPixels()) + j) & 255));
        }
        this->show();
        delay(wait);
    }
}

void NeoPixel::theaterChase(uint32_t c, uint8_t wait)
{
    for (int j = 0; j < 10; j++)
    { //do 10 cycles of chasing
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < this->numPixels(); i = i + 3)
            {
                this->setPixelColor(i + q, c); //turn every third pixel on
            }
            this->show();

            delay(wait);

            for (uint16_t i = 0; i < this->numPixels(); i = i + 3)
            {
                this->setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

void NeoPixel::theaterChaseRainbow(uint8_t wait)
{
    for (int j = 0; j < 256; j++)
    { // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < this->numPixels(); i = i + 3)
            {
                this->setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
            }
            this->show();

            delay(wait);

            for (uint16_t i = 0; i < this->numPixels(); i = i + 3)
            {
                this->setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

uint32_t NeoPixel::Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return this->Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return this->Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return this->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
