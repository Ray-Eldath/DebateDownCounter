#include <SoftwareSerial.h>

SoftwareSerial sSerial(12, 11); // RX, TX

const int buzzer = 10;

void setup() {
  Serial.begin(9600);
  sSerial.begin(9600);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop() {
  int in = sSerial.read();
  if (in == 'a') {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
  }
}
