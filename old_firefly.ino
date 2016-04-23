#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2
#define NUMPIXELS      7

#define MALE 1
#define FEMALE 0

#define Y565 pixels.Color(255,210,0)
#define OFF pixels.Color(0,0,0)

#define MALE_DURATION 500L //male flash duration
#define MALE_INTERVAL 5900L //male interval between flashes
#define FEMALE_DURATION 600L //female flash duration
#define FEMALE_DELAY 2800L //how long female waits to respond

#define DURFLUC 100

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

long fOn = 100000L; //arbitrarily long time
long fOff = 100000L;
long mOn = 0L;
long mOff = 100000L;

void setup() 
{
  
pixels.begin(); // This initializes the NeoPixel library.
delay(2000);

}

void loop() 
{
  if(millis() >= mOn) //male turns on
  {
    pixels.setPixelColor(MALE, Y565);
    mOff = millis() + MALE_DURATION + random(-DURFLUC,DURFLUC);
    mOn = millis() + MALE_INTERVAL;
    fOn = millis() + FEMALE_DELAY;
  }
  if(millis() >= fOn)
  {
    pixels.setPixelColor(FEMALE, Y565);
    fOff = millis() + FEMALE_DURATION + random(-DURFLUC,DURFLUC);
    fOn = millis() + 100000L;
  }
  if(millis() >= mOff)
  {
    pixels.setPixelColor(MALE, OFF);
  }
  if(millis() >= fOff)
  {
    pixels.setPixelColor(FEMALE, OFF);
    fOff = millis() + 100000L;
  }
  pixels.show();
}
