// Apr 16, 2016 Firefly proto with trinket and two lines

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            5
#define PRPIN A0       //photoresistor analog pin

#define LIGHT_MIN 550
#define LIGHT_MAX 1010

#define HUE pixels.Color(255,210,0)
#define OFF pixels.Color(0,0,0)

#define MALE_DURATION 500L //male flash duration
#define FEMALE_DURATION 600L //female flash duration
#define FEMALE_DELAY 2800L //how long female waits to respond

#define DURFLUC 100

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(25, PIN, NEO_GRB + NEO_KHZ800);

int genders[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,11,10,9,8,7,6,5,4,3,2,1 };

//event variables

long onTimes[25];
long offTimes[25];

void setup() 
{
  // This is for Trinket 5V 16MHz
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  for(int i = 0; i < 25; i++)
  {
    onTimes[i] = -1;
    offTimes[i] = -1;
  }
  
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
  //Serial.begin(9600);
}



long activateMale = 0;
long t;

void loop() 
{
  
  while(analogRead(PRPIN) < LIGHT_MIN || analogRead(PRPIN) > LIGHT_MAX)
  {
    for(int i = 0; i < 25; i++)
    {
      pixels.setPixelColor(i, OFF);
      onTimes[i] = -1;
    }
    //Serial.println(analogRead(PRPIN));
    pixels.show();
    activateMale = millis() + 100;
    delay(100);
  }

  t = millis();
  if (millis() >= activateMale)
  {
    //activate male incremented by 10000
    activateMale += 5000L + random(-DURFLUC, DURFLUC);

    int maleIndex = random(13, 25);
    int femaleIndex = genders[maleIndex];
    onTimes[maleIndex] = t;
    offTimes[maleIndex] = t + MALE_DURATION;
    onTimes[femaleIndex] = t + FEMALE_DELAY;
    offTimes[femaleIndex] = onTimes[femaleIndex] + FEMALE_DURATION;
   }

  for(int i = 0; i < 25; i++) //run this for each light
  {
    //first check to see if there is an on event scheduled for this light, and if there is
    //check to see if it is time for it yet
    if(onTimes[i] != -1 && t >= onTimes[i])
    {
      pixels.setPixelColor(i, HUE);
      onTimes[i] = -1;
    }
    if(offTimes[i] != -1 && t >= offTimes[i])
    {
      pixels.setPixelColor(i, OFF);
      offTimes[i] = -1;
    }
  }
  
  pixels.show();
  delay(10);
  //Serial.println(freeRam());
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
