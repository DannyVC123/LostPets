/*  Hey guys! So the wiring here is really easy.  Your bluetooth module 
    wiring is as follows: Vcc goes to 5v, GND goes to GND, Tx goes to 
    D2, and RX goes to D3. 
*/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
int enR = 3;   //These are the pins for our motor driver IC
int inAR = 4;
int inBR = 5;
int enL = 6;
int inAL = 7;
int inBL = 8;

char dataReceived = '\0';

void setup() 
{
  pinMode(enR, OUTPUT);
  pinMode(inAR, OUTPUT); 
  pinMode(inBR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(inAL, OUTPUT); 
  pinMode(inBL, OUTPUT);
  Serial.begin(9600);    // Set your Serial Monitor is set at 250000
  Dabble.begin(9600);      // This is the baude rate of the HM-10
}

void loop() {
Dabble.processInput();  // This line is crucial in grabbing our data  

  // check if data has been received
  if (dataReceived != '\0') {
    // print data to Serial Monitor
    Serial.print("Data received: ");
    Serial.println(dataReceived);
    dataReceived = '\0'; // reset dataReceived variable
  }
  
  if (GamePad.isUpPressed())       
  {
    Serial.println("Forward");
  }

  else if (GamePad.isDownPressed())
  {
    Serial.println("Backward");
  }

  else if (GamePad.isLeftPressed())
  {
    Serial.println("Left");
  }

  else if (GamePad.isRightPressed())
  {
    Serial.println("Right");
  }
  else 
  {
    Serial.println("Stopped");
  }
}

void serialEvent() {
  while (Serial.available()) {
    char incomingChar = Serial.read();
    if (incomingChar != '\n' && incomingChar != '\r') {
      // save incoming character to dataReceived variable
      dataReceived = incomingChar;
    }
  }
}
