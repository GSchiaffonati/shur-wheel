#include <FastLED.h>

#include <Adafruit_LEDBackpack.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN 11
#define CLK_PIN 13
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
}

void loop(){
  Serial.println("main start");
  alphaDis();
  segR();
  
}

void alphaDis(){
  alpha4.writeDigitAscii(0, 'S');
  alpha4.writeDigitAscii(1, 'H');
  alpha4.writeDigitAscii(2, 'U');
  alpha4.writeDigitAscii(3, ' ');
  alpha4.writeDisplay();
}

void segR(){
  digitalWrite(SegA, HIGH);
  digitalWrite(SegB, LOW);
  digitalWrite(SegC, LOW);
  digitalWrite(SegD, LOW);
  digitalWrite(SegE, HIGH);
  digitalWrite(SegF, HIGH);
  digitalWrite(SegG, LOW);
}

