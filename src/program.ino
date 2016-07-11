//####################################################################
//	For the attention of anyone unfortunate enough to have to
//	read/alter this program!
//####################################################################
//The i2c may be setup only as slave mode,
//
//####################################################################

// constant headers
#include <FastLED.h>//APA102 control via SPI
#include <Adafruit_LEDBackpack.h>//led alpha-num display
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Wire.h>//arduino I2C

//FastLED macros
FASTLED_USING_NAMESPACE
#define LED_TYPE APA102
#define COLOR_ORDER BGR
#define NUM_LEDS 15//total number of APA102 LEDs on the SPI bus
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  50
#define FRAMES_PER_SECOND 120

//shift led macros
#define SHIFT 4500//the rpm to change gear at
#define LED_MAIN_DISPLAY 12//number of APA102 LEDs on the display fo the revs

//I2C
#define WHEEL_I2C_ADDRESS 0x8//address to request data from for the wheel rotation
#define packetsize 2
#define LOCAL_I2C_ADDRESS 0x9//set the local I2C address (the i2c address of the wheel)

//Alpha-Numeric 16-segment displays
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
#define ALPHA_NUM_ADDRESS 0x70//set the i2c address for the alpha numeric displays

//define pins
const int Button1 = A0;
const int Button2 = A1;

const int SegA = 13;
const int SegB = 12;
const int SegC = 11;
const int SegD = 10;
const int SegE = 9;
const int SegF = 6;
const int SegG = 5;

//I2C data
byte DataPacket[packetsize];	//Size 2 BYTE array, note zero index when called
int RPM = 0;	//rpm from the wheel
//Full data converted back from individual packets


int ecuRPM = 0;	//rpm from the engine


void setup(){
	delay(3000);	//3 second delay for recovery? (Fast LED requirement,.. maybe??)
	Serial.begin(9600);
	FastLED.addLeds<LED_TYPE, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  	FastLED.setBrightness(BRIGHTNESS);

	alpha4.begin(ALPHA_NUM_ADDRESS);	//pass in the address for alphanumeric
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
    		leds[i] = CRGB::Black;	//set all APA102 LEDs to off
    		FastLED.show();	//update changes to the SPI bus
    	}

	Wire.begin(LOCAL_I2C_ADDRESS);	//Join I2C bus as slave with address LOCAL_I2C_ADDRESS
  	Wire.onReceive(receiveEvent);	//Register event for data recieve

}

void loop(){
	int gear = calcGear();	//CHANGE THIS!
	displayRPM(RPM);
	shiftLights(RPM);
	gearSeg(gear);
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

//Calculate the gear
int calcGear(){
	//gear ratios
	float g1 = 19.194;
	float g2 = 14.050;
	float g3 = 11.000;
	float g4 = 8.821;
	float g5 = 7.153;
	float g6 = 6.029;

	int g = ecuRPM / RPM;	//Data is lost here, I believe it is minimal so can be ignored
	int gVal;
	if(g > g6){
		gVal = 6;
	}
	else if(g > g5){
		gVal = 5;
	}
	else if(g > g4){
		gVal = 4;
	}
	else if(g > g3){
		gVal = 3;
	}
	else if(g > g2){
		gVal = 2;
	}
	else if(g > g1){
		gVal = 1;
	}
	else if (g > 100){	//if ratio > 100 assume it is in neutral
		gVal = 0;
	}
	else{
		gVal = -1;
	}
	return gVal;
}

//Function to take value of Shift and scale to display
void shiftLights(int RS) {
	int i = 0;
	int TRIM = constrain(RS, 0, SHIFT);
  	int numLedsToLight = map(TRIM, 0, SHIFT, 0, LED_MAIN_DISPLAY);
  	FastLED.clear();

	while(i < LED_MAIN_DISPLAY){
		if(i < numLedsToLight){
			//first 4 LEDs are green (low rev. count)
			if(i < 4){
				leds[i] = CRGB::Green;
			}
			//next 4 LEDs red
			if(i > 3 && i < 8){
				leds[i] = CRGB::Red;
			}
			//last 4 LEDs on the primary LED display are blue
			if(i > 7 && i < LED_MAIN_DISPLAY){
				leds[i] = CRGB::Blue;
			}
		}
		//if it isn't an LED that needs to be lit then it will be Black
		else{
			leds[i] = CRGB::Black;
		}
		i++;
	}
  	FastLED.show();//update the SPI bus
}

void receiveEvent(int howMany)
{
  	while (Wire.available())
  	{
    		for ( int i = 0; i < packetsize; i++)
    		{
      			DataPacket[i] = Wire.read();	//Recieve byte
    		}

		//Format Data
    		RPM = DataPacket[0];	//Set first byte as integer in variable Data
    		RPM = (RPM << 8) | DataPacket[1];	//shift c by 8 BITS to correct transmission formatting and OR it with the second BYTE B
  	}
	//Serial.print("Data Recieved ");    //  Print the result to the serial com port for debug
	//Serial.print(RPM);
  	//Serial.print("\n");   //  Add carriage return to each line to allow for easy reading in com port
}

void gearSeg(int gear){
	//switch function for the current gear to be displayed on the 7-segment display
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
		case -1:	//error case (when calcGear returns -1 error code)
			digitalWrite(SegA, LOW);
  			digitalWrite(SegB, HIGH);
  			digitalWrite(SegC, HIGH);
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

