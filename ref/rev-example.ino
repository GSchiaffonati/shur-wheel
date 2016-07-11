#include <Wire.h>
#include <FastLED.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
//#include <Adafruit_Sensor.h>
//#include "Adafruit_TSL2591.h"

#define NUM_LEDS    12
#define SHIFT       4500 
#define BRIGHTNESS  50

CRGB leds[NUM_LEDS]; //Memory block for storing LED data. 

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
//Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

void setup() {

 //tsl.setGain(TSL2591_GAIN_MED);     
 //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);
  
 Serial.begin(9600);
  
 //LED Display Setup
 alpha4.begin(0x70);
 alpha4.clear();
 alpha4.writeDisplay();

 //SHift light setup 
 FastLED.addLeds<APA102,BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(BRIGHTNESS);
}


void loop() {
  int RPM = (analogRead(A0))*9;
  SHIFT_LIGHTS(RPM);
  DISPLAY_RPM(RPM);
}


//Function to take value of Shift and scale to display
void SHIFT_LIGHTS(int RS) {
  int TRIM = constrain(RS, 0, SHIFT);
  int numLedsToLight = map(TRIM, 0, SHIFT, 0, NUM_LEDS);
  FastLED.clear();
  for(int led = 0; led < numLedsToLight; led++) { 
     leds[led] = CRGB::Red;
  }
  FastLED.show();
}


//Function to take RPM and output to alphanumeric display
void DISPLAY_RPM(int RR) {

  int d0,d1,d2,d3;
  
  d0=RR/1000;
  d1=RR%1000/100;
  d2=RR%100/10;
  d3=RR%10;
    
  alpha4.writeDigitAscii(0, d0 + '0');
  alpha4.writeDigitAscii(1, d1 + '0');
  alpha4.writeDigitAscii(2, d2 + '0');
  alpha4.writeDigitAscii(3, d3 + '0');

  alpha4.writeDisplay();

  delay(1);
}