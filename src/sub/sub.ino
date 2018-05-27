#include <SoftwareSerial.h>

SoftwareSerial sSerial(12, 11); // RX, TX

const int buzzer = 10;
const int r = 4;
const int g = 3;
const int b = 2;

void setup() {
  Serial.begin(9600);
  sSerial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  digitalWrite(buzzer, LOW);
  bi();
  bla();
}

void loop() {
  int in = sSerial.read();
  // checkpoints:
  // a - 60s; b - 30s; c - 15s
  if (in == 'a') {
    bi();
    bl(g);
  } else if (in == 'b') {
    bi();
    delay(400);
    bi();
    bl(b);
  } else if (in == 'c') {
    bi();
    delay(400);
    biLong();
    bl(r);
  }  else if (in == '0')
    bla();
}

void bi() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
}

void biLong() {
  digitalWrite(buzzer, HIGH);
  delay(800);
  digitalWrite(buzzer, LOW);
}

void bl(int p) {
  digitalWrite(p, HIGH);
  delay(1000);
  digitalWrite(p, LOW);
}

void bla() {
  digitalWrite(r, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(b, HIGH);
  delay(200);
  digitalWrite(r, LOW);
  digitalWrite(g, LOW);
  digitalWrite(b, LOW);
}
