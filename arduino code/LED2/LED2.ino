#include <LiquidCrystal.h>

//light sensor
const int sensorPin = A5;
int sensorLow = 0, sensorHigh = 1023;
int sensorValue;

//led strip
const int ledPin = 8; //one led
const int greenPin = 10, redPin = 9, bluePin = 6; //must be PWM pins
int redValue = 255, greenValue = 0, blueValue= 0;

//lcd
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
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
  Serial.println(sensorValue);

  if (sensorValue > 80) {//sensorLow + (sensorHigh - sensorLow) * 0.1) {
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
  lcd.clear();
  
  String currText = get(textListCurrInd);
  if (currText.length() <= 16) {
    lcd.print(currText.substring(0, 16));
    textListCurrInd = (textListCurrInd + 1) % listSize;
    return;
  }
  
  int lastInd = currText.lastIndexOf(' ', 16);
  for (int i = 15; i > lastInd && i > 0; i--) {
    if (!isPunct(currText.charAt(i))) continue;
    lastInd = i;
    break;
  }
  
  if (lastInd == -1) {
    lcd.print(currText.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print(currText.substring(16));
  } else if (currText.charAt(lastInd) == ' ') {
    lcd.print(currText.substring(0, lastInd));
    lcd.setCursor(0, 1);
    lcd.print(currText.substring(lastInd + 1));
  } else {
    lcd.print(currText.substring(0, lastInd + 1));
    lcd.setCursor(0, 1);
    bool nextSpace = lastInd + 1 < currText.length() && currText.charAt(lastInd + 1) == ' ';
    lcd.print(currText.substring(lastInd + (nextSpace ? 2 : 1)));
  }
  
  textListCurrInd = (textListCurrInd + 1) % listSize;
}



//----------Array List Implementation----------

void add(String text) {
  int delimInd = text.indexOf('`');
  
  if (delimInd == -1) {
    textList[listSize++] = text;
    return;
  }
  
  while (delimInd > -1) {
    textList[listSize++] = text.substring(0, delimInd);
    text = text.substring(delimInd + 1);
    delimInd = text.indexOf('`');
  }
  textList[listSize++] = text;
}

void add(int i, String text) {
  if (i < 0 || i > listSize)
    return;
  if (i == listSize) {
    add(text);
    return;
  }
  
  int delimInd = text.indexOf('`');
  
  if (delimInd == -1) {
    for (int j = listSize - 1; j >= i; j--) {
      Serial.println(textList[j]);
      textList[j + 1] = textList[j];
      Serial.println(textList[j+1]);
    }
    Serial.println(textList[i]);
    textList[i] = text;
    Serial.println(textList[i]);
    listSize++;
    return;
  }
  
  int numDelims = 1;
  for (int j = delimInd + 1; j < text.length(); j++) {
    if (text.charAt(i) == '`') numDelims++;
  }
  for (int j = listSize - 1; j >= i; j--) {
    textList[i + numDelims + 1] = textList[i];
  }
  
  while (delimInd > -1) {
    textList[i++] = text.substring(0, delimInd);
    text = text.substring(delimInd + 1);
    delimInd = text.indexOf('`');
  }
  textList[listSize++] = text;
}

String get(int i) {
  return textList[i];
}

String remove(int i) {
  if (i < 0 || i > listSize)
    return;
  String ret = textList[i];
  textList[i] = "";
  
  for (int j = listSize; j > i; j++) {
    textList[j - 1] = textList[j];
  }
  
  listSize--;
  return ret;
}
