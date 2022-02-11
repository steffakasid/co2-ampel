#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class NeoPixel: public Adafruit_NeoPixel
{
public:
    NeoPixel(uint16_t size, int16_t pin, neoPixelType type);
    void unset();
    void setColor(uint32_t c, uint8_t pixels, uint8_t wait);
    void colorWipe(uint32_t c, uint8_t wait);
    void rainbow(uint8_t wait);
    void rainbowCycle(uint8_t wait);
    void theaterChase(uint32_t c, uint8_t wait);
    void theaterChaseRainbow(uint8_t wait);

private:
    uint32_t Wheel(byte WheelPos);
};