#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 6, NEO_GRB + NEO_KHZ800);

float pixelStrength = 0.1f;  // 1 - full strenght 0 - off
float delayMultiplier = 0.5f; // global delay multiplier to make effects faster or slower

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 

  strip.setBrightness(255);         // one time brightness setup (0 - 255)
  strip.begin();                    // start strip
  strip.show();                     // Initialize all pixels to 'off'
}

void loop() {
  colorWipe(127, 127, 127, 100);  // White
  colorWipe(0, 255, 0, 100);      // Green
  colorWipe(255, 0, 0, 100);      // Red

  theaterChase(127, 127, 127, 100); // White
  theaterChase(0, 127, 0, 100);     // Green
  theaterChase(127, 0, 0, 100);     // Red

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  DimColor(r , g, b);
  wait *= delayMultiplier;
  
  for (uint16_t p = 0; p < strip.numPixels(); p++) {
    strip.setPixelColor(p, r, g, b);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  wait *= delayMultiplier;
  
  for (uint16_t w = 0; w < 256; w++) {
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, Wheel((p+w)));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  wait *= delayMultiplier;
  
  for(uint16_t c=0; c < 256*5; c++) { // 5 cycles of all colors on wheel
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, Wheel(p*256/strip.numPixels() + c));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  DimColor(r , g, b);
  wait *= delayMultiplier;
  
  for (uint16_t i = 0; i < 10; i++) {  //do 10 cycles of chasing
    for (uint16_t c = 0; c < 3; c++) {
      for (uint16_t p = 0; p < strip.numPixels(); p += 3) {
        strip.setPixelColor(p + c, r, g, b);    //turn every third pixel on
      }
      strip.show();
      delay(wait);

      for (uint16_t p = 0; p < strip.numPixels(); p += 3) {
        strip.setPixelColor(p+c, 0);    //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  wait *= delayMultiplier;
  
  for (int w = 0; w < 256; w++) {     // cycle all 256 colors in the wheel
    for (uint16_t c = 0; c < 3; c++) {
      for (uint16_t p = 0; p < strip.numPixels(); p += 3) {
        strip.setPixelColor(w + c, Wheel((p + w) % 255));    //turn every third pixel on
      }
      strip.show();
      delay(wait);

      for (uint16_t p = 0; p < strip.numPixels(); p += 3) {
        strip.setPixelColor(p + c, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
inline uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  uint8_t r = WheelPos < 85 ? 255 - WheelPos * 3 : WheelPos < 170 ? 0 : WheelPos * 3;
  uint8_t g = WheelPos < 85 ? 0 : WheelPos < 170 ? WheelPos * 3 : 255 - WheelPos * 3;
  uint8_t b = WheelPos < 85 ? WheelPos * 3 : WheelPos < 170 ? 255 - WheelPos * 3 : 0;

  return DimColor(strip.Color(r, g, b));
}

inline uint32_t DimColor(const uint32_t &color)
{
  return strip.Color(Red(color) * pixelStrength, Green(color) * pixelStrength, Blue(color) * pixelStrength);
}

inline void DimColor(uint8_t &r, uint8_t &g, uint8_t &b)
{
  r *= pixelStrength;
  g *= pixelStrength;
  b *= pixelStrength;
}

// Returns the Red component of a 32-bit color
inline uint8_t Red(const uint32_t &color)
{
  return (color >> 16) & 0xFF;
}
 
// Returns the Green component of a 32-bit color
inline uint8_t Green(const uint32_t &color)
{
  return (color >> 8) & 0xFF;
}
 
// Returns the Blue component of a 32-bit color
inline uint8_t Blue(const uint32_t &color)
{
  return color & 0xFF;
}
