

//  Library Declarations  //
#include <Wire.h>

//  Software Define Constants //
#define WHEEL_I2C_ADDRESS 0x8
#define packetsize 2

//  Global Variable Declarations  //
byte DataPacket[packetsize];   //  Size 2 BYTE array, note zero index when called
int RPM = 0;    //  Full data converted back from individual packets

//  Startup Code - executes once  //
void setup()
{
  Wire.begin(WHEEL_I2C_ADDRESS);    //  Join I2C bus as slave with address 8
  Wire.onReceive(receiveEvent);   //  Register event for data recieve
  Serial.begin(9600);   //  Start serial communication
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);   //  Illuminate user LED pin 3 to show setup is complete
}

//   Main Program Loop  //
void loop()
{
  delay(500);
}

// This function is registered as an event and is executing when 'called' by the master
void receiveEvent(int howMany)
{
  while (Wire.available())
  {
    for ( int i = 0; i < packetsize; i++)
    {
      DataPacket[i] = Wire.read();    //  Recieve byte
    }

    //  Format Data
    RPM = DataPacket[0];    //  Set first byte as integer in variable Data
    RPM = (RPM << 8) | DataPacket[1];   //  shift c by 8 BITS to correct transmission formatting and OR it with the second BYTE B
  }
  Serial.print("Data Recieved ");    //  Print the result to the serial com port for debug
  Serial.print(RPM);
  Serial.print("\n");   //  Add carriage return to each line to allow for easy reading in com port
}
