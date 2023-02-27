#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11);

void setup() {

  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  bluetooth.begin(9600); // set HM10 serial at 9600 baud rate
  pinMode(13, OUTPUT); // onboard LED
  digitalWrite(13, LOW); // switch OFF LED
}

void loop() {
  if (bluetooth.available()) {
    char recieved = bluetooth.read();
    
    Serial.print("Recieved data: ");
    Serial.println(recieved);
  }
  /*
  HM10.listen();  // listen the HM10 port
  while (HM10.available() > 0) {   // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData);  // save the data in string format
    Serial.write(appData);
  }

  if (Serial.available()) {           // Read user input if available.
    delay(10);
    HM10.write(Serial.read());
  }

  Serial.print(inData);
  */
}
