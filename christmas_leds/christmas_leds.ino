#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define PIN 6
#define NUMPIXELS 182
#define DELAYVAL 500
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

unsigned long scenarioTimeInSeconds = 60;
unsigned long scenarioTimeInMicros = scenarioTimeInSeconds * 1000000;

uint32_t rainbow[7] = { 0xFFFFFFFF, 0xFFFF0000, 0xFFFF7F00, 0xFFFFFF00, 0xFF00FF00, 0xFF0000FF, 0xFF8F00FF };
uint32_t christmas[6] = { 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00 };
uint32_t seahawks[3] = { 0xFF002244, 0xFF69BE1C, 0xFFA5ACAF };

void loop() {
  arrayShift(rainbow, 7);
  arrayShift(christmas, 6);
  arrayShift(seahawks, 2);
}

void arrayShift(uint32_t arr[], int numColors) {
  beginScenario(arr, numColors);
  
  unsigned long endTime = micros() + scenarioTimeInMicros;
  uint32_t startIndex = 0;
  
  while(endTime > micros())
  {
    pixels.clear();
  
    for(int j=0; j<NUMPIXELS; j++)
    {
      pixels.setPixelColor((startIndex + j) % NUMPIXELS, arr[j % numColors]);
    }
    pixels.show();
    
    startIndex++;
    delay(DELAYVAL);
  }

  //endScenario();
}


void beginScenario(uint32_t arr[], int numColors)
{
  for(int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, arr[i % numColors]);
    pixels.show();
  }
}

void growColors()
{
  unsigned long endTime = micros() + scenarioTimeInMicros;
  uint32_t power = 0;
  
  while(endTime > micros())
  {
    //pixels.clear();
  
    for(uint32_t i=0; i<NUMPIXELS; i++)
    {
      uint32_t color = power << ((i % 4) * 8);
      if(i % 4 == 0)
      {
        color = 0xFFFFFFFF;
      }
      pixels.setPixelColor(i, color);
      pixels.show();
      //delay(DELAYVAL);
    }

    power = (power + 1) % 256;
  }
}

void randomizePixels()
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, 0xFF000000 | random());
  }
}

void endScenario()
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i % NUMPIXELS, 0);
    pixels.show();
    delay(10);
  }
}
