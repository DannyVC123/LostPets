#include <LiquidCrystal.h>

//light sensor
const int sensorPin = A0;
int sensorLow = 1023, sensorHigh = 0;
int sensorValue;

//led strip
const int ledPin = 8; //one led
//const int redPin = 0, greenPin = 0, bluePin = 0; //must be PWM pins
//int redValue, greenValue, blueValue;

//lcd
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
unsigned long previousTimeLCDChange = millis();
long LCDChangeTimeInterval = 2000;

//text on lcd
String textList[100];
int textListCurrInd = 0, listSize = 0;

void setup() {
  Serial.begin(9600);

  //set up light sensor
  sensorValue = analogRead(sensorPin);
  if (sensorValue > sensorHigh) sensorHigh = sensorValue;
  if (sensorValue < sensorLow) sensorLow = sensorValue;

  //set up led strip
  pinMode(ledPin, OUTPUT); //one led
  /*
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  */

  //set up LCD
  lcd.begin(16, 2);
  add("Pet Name: Bailey");
  add("Owner Name: Danny Huo");
  add("Address: 9889 Wade Blvd, Frisco, TX 75035");
  add("Phone Number: 469-123-1234");
}

void loop() {
  unsigned long currentTime = millis();
  
  detectLight();

  if (currentTime - previousTimeLCDChange > LCDChangeTimeInterval) {
    previousTimeLCDChange = currentTime;
    updateLCD();
  }
}

void detectLight() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  //one led
  if (sensorValue < 100) digitalWrite(ledPin, HIGH);
  else digitalWrite(ledPin, LOW);

  /*
  if (sensorValue < 100) {
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
  } else {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }
  */
}

void updateLCD() {
  String currText = get(textListCurrInd);
  int lastInd = currText.lastIndexOf(' ', 15);
  
  lcd.clear();
  lcd.print(currText.substring(0, lastInd));
  
  lcd.setCursor(0, 1);
  lcd.print(currText.substring(lastInd + 1));
  
  textListCurrInd = (textListCurrInd + 1) % listSize;
}



//----------Array List Implementation----------

void add(String text) {
  textList[listSize++] = text;
}

String get(int i) {
  return textList[i];
}

void remove(int i) {
  String ret = textList[i];
  textList[i] = "";
  
  int x = i + 1;
  while (textList[x] != "") {
    textList[x - 1] = textList[x++];
  }
  textList[x-1] = "";

  return ret;
}
