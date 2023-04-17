#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3);

void setup() {

  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  bluetooth.begin(9600); // set HM10 serial at 9600 baud rate
  delay(1000);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
  }
}
