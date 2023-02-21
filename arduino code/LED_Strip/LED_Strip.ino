#include <LiquidCrystal.h>

const int greenPin = 10, redPin = 9, bluePin = 6; //must be PWM pins
int fadeSpeed = 5;

//lcd
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Hello world!");
}

void loop() {
  int r, g, b;

  for (r = 0; r < 256; r++) { 
    analogWrite(redPin, r);
    delay(fadeSpeed);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(bluePin, b);
    delay(fadeSpeed);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(greenPin, g);
    delay(fadeSpeed);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(redPin, r);
    delay(fadeSpeed);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(bluePin, b);
    delay(fadeSpeed);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(greenPin, g);
    delay(fadeSpeed);
  } 
}
