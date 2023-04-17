#include <LiquidCrystal_I2C.h>

//light sensor
const int sensorPin = A5;
int sensorLow = 0, sensorHigh = 1023;
int sensorValue;

//led strip
const int ledPin = 8; //one led
const int greenPin = 12, redPin = 11, bluePin = 10; //must be PWM pins
int redValue = 0, greenValue = 255, blueValue = 255;

//lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);
//LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
unsigned long previousTimeLCDChange = millis();
long LCDChangeTimeInterval = 5000;

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
  lcd.init();
  lcd.clear();
  lcd.backlight();

  //add messages
  add("Pet Name: Bailey");
  add("Owner Name: Danny Huo");
  add("Address: 9889 Wade Blvd");
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

  if (sensorValue > 80) {
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
      textList[j + 1] = textList[j];
    }
    textList[i] = text;
    listSize++;
    return;
  }
  
  int numDelims = 1;
  for (int j = delimInd + 1; j < text.length(); j++) {
    if (text.charAt(j) == '`') numDelims++;
  }
  for (int j = listSize - 1; j >= i; j--) {
    textList[j + numDelims + 1] = textList[j];
  }
  
  while (delimInd > -1) {
    textList[i++] = text.substring(0, delimInd);
    text = text.substring(delimInd + 1);
    delimInd = text.indexOf('`');
  }
  textList[i] = text;
  
  listSize += numDelims + 1;
}

String set (int i, String text) {
  if (i < 0 || i >= listSize)
    return "";
  
  Serial.println(i + " " + text);
  String ret = get(i);
  textList[i] = text;
  return ret;
}

String remove(int i) {
  if (i < 0 || i >= listSize)
    return "";
  
  String ret = textList[i];
  int j = i;
  for (int j = i; j < listSize; j++) {
    textList[j] = textList[j + 1];
  }
  
  listSize--;
  return ret;
}

String get(int i) {
  return textList[i];
}
