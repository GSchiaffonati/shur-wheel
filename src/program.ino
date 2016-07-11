#include <FastLED.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

FASTLED_USING_NAMESPACE

#define LED_TYPE APA102
#define COLOR_ORDER BGR
#define NUM_LEDS 15
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  50
#define FRAMES_PER_SECOND 120

#define SHIFT 4500
#define LED_MAIN_DISPLAY 12


Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

const int Button1 = A0;
const int Button2 = A1;

const int SegA = 13;
const int SegB = 12;
const int SegC = 11;
const int SegD = 10;
const int SegE = 9;
const int SegF = 6;
const int SegG = 5;



void setup(){
	delay(3000);	//3 second delay for recovery
	Serial.begin(9600);
	FastLED.addLeds<LED_TYPE, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  	FastLED.setBrightness(BRIGHTNESS);

	alpha4.begin(0x70);	// pass in the address for alphanumeric
	alpha4.clear();
	alpha4.writeDisplay();

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
    		leds[i] = CRGB::Black;//set to off
    		FastLED.show();
    	}
}

void loop(){
	int rpm = 0;//CHANGE THIS!
	int gear = 0;
	displayRPM(rpm);
	shiftLights(rpm);
	gearSeg(gear);
}

void gearSeg(int gear){
	switch (gear){
		case 0:
			digitalWrite(SegA, HIGH);
			digitalWrite(SegB, HIGH);
			digitalWrite(SegC, LOW);
  			digitalWrite(SegD, HIGH);
			digitalWrite(SegE, LOW);
			digitalWrite(SegF, HIGH);
			digitalWrite(SegG, LOW);
		case 1:
			digitalWrite(SegA, HIGH);
  			digitalWrite(SegB, LOW);
  			digitalWrite(SegC, LOW);
  			digitalWrite(SegD, HIGH);
			digitalWrite(SegE, HIGH);
  			digitalWrite(SegF, HIGH);
  			digitalWrite(SegG, HIGH);
		case 2:
			digitalWrite(SegA, LOW);
  			digitalWrite(SegB, LOW);
  			digitalWrite(SegC, HIGH);
  			digitalWrite(SegD, LOW);
			digitalWrite(SegE, LOW);
  			digitalWrite(SegF, HIGH);
  			digitalWrite(SegG, LOW);
		case 3:
			digitalWrite(SegA, LOW);
  			digitalWrite(SegB, LOW);
  			digitalWrite(SegC, LOW);
  			digitalWrite(SegD, LOW);
			digitalWrite(SegE, HIGH);
  			digitalWrite(SegF, HIGH);
  			digitalWrite(SegG, LOW);
		case 4:
			digitalWrite(SegA, HIGH);
  			digitalWrite(SegB, LOW);
  			digitalWrite(SegC, LOW);
  			digitalWrite(SegD, HIGH);
			digitalWrite(SegE, HIGH);
  			digitalWrite(SegF, LOW);
  			digitalWrite(SegG, LOW);
		case 5:
			digitalWrite(SegA, HIGH);
  			digitalWrite(SegB, LOW);
  			digitalWrite(SegC, LOW);
  			digitalWrite(SegD, HIGH);
			digitalWrite(SegE, HIGH);
  			digitalWrite(SegF, LOW);
  			digitalWrite(SegG, LOW);
		case 6:
			digitalWrite(SegA, HIGH);
  			digitalWrite(SegB, HIGH);
  			digitalWrite(SegC, LOW);
  			digitalWrite(SegD, LOW);
			digitalWrite(SegE, LOW);
  			digitalWrite(SegF, LOW);
  			digitalWrite(SegG, LOW);
//		default:
//			digitalWrite(SegA, LOW);
//			digitalWrite(SegB, HIGH;
//  			digitalWrite(SegC, HIGH);
//  			digitalWrite(SegD, LOW);
//			digitalWrite(SegE, LOW);
//  			digitalWrite(SegF, LOW);
//  			digitalWrite(SegG, LOW);
	}
}

//Function to take RPM and output to alphanumeric display
void displayRPM(int RR) {

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

	//delay(1);	//removed delay to keep things running fast
}

//Function to take value of Shift and scale to display
void shiftLights(int RS) {
	int i = 0;
	int TRIM = constrain(RS, 0, SHIFT);
  	int numLedsToLight = map(TRIM, 0, SHIFT, 0, LED_MAIN_DISPLAY);
  	FastLED.clear();

	while(i < LED_MAIN_DISPLAY){
		if(i < numLedsToLight){
			if(i < 4){
				leds[i] = CRGB::Green;
			}
			if(i > 3 && i < 8){
				leds[i] = CRGB::Red;
			}
			if(i > 7 && i < LED_MAIN_DISPLAY){
				leds[i] = CRGB::Blue;
			}
		}
		else{
			leds[i] = CRGB::Black;
		}
		i++;
	}
  	FastLED.show();
}


