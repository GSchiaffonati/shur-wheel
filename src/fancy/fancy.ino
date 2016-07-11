#include <FastLED.h>

#include <Adafruit_LEDBackpack.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

FASTLED_USING_NAMESPACE

#define LED_TYPE APA102
#define COLOR_ORDER BGR
#define NUM_LEDS 15
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 120

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

int Button1 = A0;
int Button2 = A1;

int SegA = 13;
int SegB = 12;
int SegC = 11;
int SegD = 10;
int SegE = 9;
int SegF = 6;
int SegG = 5;

void setup(){
	delay(3000); //3 second delay for recovery
	Serial.begin(9600);
	FastLED.addLeds<LED_TYPE, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  alpha4.begin(0x70);  // pass in the address for alphanumeric
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(SegA, OUTPUT);
  pinMode(SegB, OUTPUT);
  pinMode(SegC, OUTPUT);
  pinMode(SegD, OUTPUT);
  pinMode(SegE, OUTPUT);
  pinMode(SegF, OUTPUT);
  pinMode(SegG, OUTPUT);

  int i;
  for(i=0;i<NUM_LEDS;i++){
    leds[i].red = 0;//set to off
    leds[i].green = 0;
    leds[i].blue = 0;
    FastLED.show();
    }


}

void loop(){
  int ledState = 1;
  Serial.println("main start");
  //FastLED.show();
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
  alphaDis();
  segR();
  ledState = digitalRead(Button1);
  if(ledState == LOW){
    Serial.println("button press");
    led();
  }
  else{
    Serial.println("no press");
  }
}

void alphaDis(){
  alpha4.writeDigitAscii(0, ' ');
  alpha4.writeDigitAscii(1, 'S');
  alpha4.writeDigitAscii(2, 'H');
  alpha4.writeDigitAscii(3, 'U');
  alpha4.writeDisplay();
}

void segR(){
  digitalWrite(SegA, LOW);
  digitalWrite(SegB, HIGH);
  digitalWrite(SegC, HIGH);
  digitalWrite(SegD, HIGH);
  digitalWrite(SegE, LOW);
  digitalWrite(SegF, LOW);
  digitalWrite(SegG, HIGH);
}

void led(){
  int i = 0;
  Serial.println("led function called");
  for(i;i<4;i++){
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(50);
    }
  for(i;i<8;i++){
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
  }
  for(i;i<12;i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(50);
  }
    for(i=0;i<NUM_LEDS;i++){
    leds[i].red = 0;//set to off
    leds[i].green = 0;
    leds[i].blue = 0;
    FastLED.show();
    }
}
