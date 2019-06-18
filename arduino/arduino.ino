#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define PIN 6
#define NUMPIXELS 150

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
//#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//  clock_prescale_set(clock_div_1);
//#endif
  Serial.begin(9600);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

int acc = 0;
int incomingByte = 0;

int colorR = 255;
int colorG = 0;
int colorB = 0;
bool isFlashing = true;

void loop() {
  acc += 1;
  runningLine(acc, 10);
  
  if (isFlashing) { flash(); }
}

void flash() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
  delay(500);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
  delay(500);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
  isFlashing = false;
}

void runningLine(int offset, int lngth) {
  int limitedOffset = offset % NUMPIXELS;
  int limitedOffset2 = (offset + NUMPIXELS / 3) % NUMPIXELS;
  int limitedOffset3 = (offset + NUMPIXELS * 2 / 3) % NUMPIXELS;
  for (int i = 0; i < NUMPIXELS; i++) {
    if ((i > limitedOffset && i < limitedOffset + lngth) | (i > limitedOffset2 && i < limitedOffset2 + lngth) | (i > limitedOffset3 && i < limitedOffset3 + lngth)) {
      pixels.setPixelColor(i, pixels.Color(colorR, colorG, colorB));
      pixels.show();
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }
}
