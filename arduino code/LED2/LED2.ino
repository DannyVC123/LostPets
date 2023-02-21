#include <LiquidCrystal.h>

//light sensor
const int sensorPin = A0;
int sensorLow = 0, sensorHigh = 1023;
int sensorValue;

//led strip
const int ledPin = 8; //one led
const int greenPin = 10, redPin = 9, bluePin = 6; //must be PWM pins
int redValue = 255, greenValue = 0, blueValue= 0;

int sensorValues[10];
int svI = 0;

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
  //sensorValue = analogRead(sensorPin);
  //if (sensorValue > sensorHigh) sensorHigh = sensorValue;
  //if (sensorValue < sensorLow) sensorLow = sensorValue;
  for (int i = 0; i < 10; i++) {
    sensorValues[i] = 100;
  }
  

  //set up led strip
  pinMode(ledPin, OUTPUT); //one led
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

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
  sensorValues[svI] = sensorValue;
  svI = (svI + 1) % 10;
  
  double average = 0;
  for (int i = 0; i < 10; i++) {
    average += sensorValues[i];
  }
  average /= 10.0;
  
  Serial.println(sensorValue);

  if (average > 200) {//sensorLow + (sensorHigh - sensorLow) * 0.1) {
    digitalWrite(ledPin, LOW);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else {
    digitalWrite(ledPin, HIGH);
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
  }
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
