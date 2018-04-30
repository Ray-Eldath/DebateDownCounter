#include <SoftwareSerial.h>

#define DEBUG

SoftwareSerial sSerial(2, 3); // RX, TX

const unsigned int LED_Z[] = {0, 0, 0, 0};
const unsigned char LED_0F[] = {
  // 0    1     2     3     4     5    6     7     8     9     A     b     C     d     E     F     -
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0xFF, 0xbf
};

unsigned int LED[4] = {0, 0, 1, 0};
unsigned int countdown = 10;

const int start = 10;
const int reset = 8;
const int add15 = 5;
const int add30 = 6;
const int useLast = 7;

const int led = 4;
const int SCLK = 13;
const int RCLK = 12;
const int DIO = 11;

int lastSetValue = 0;

void bl();
void toSet() {
  bl();
  int value = 0;
#ifdef DEBUG
  Serial.println("toSet() in!");
#endif
  while (1) {
    if (digitalRead(start) == HIGH) {
      lastSetValue = value;
      bl();
      break;
    }
    if (digitalRead(reset) == HIGH) {
      value = 0;
      delay(300);
    }
    if (digitalRead(useLast) == HIGH) {
      value = lastSetValue;
      delay(300);
    }
    if (digitalRead(add15) == HIGH) {
      value += 15;
      delay(300);
    }
    if (digitalRead(add30) == HIGH) {
      value += 30;
      delay(300);
    }
    setV(value);
    LED4_Display();
  }
}

void poll() {
  // checkpoints:
  // a - 30s; b - 15s; c - 0s
  if (countdown == 8)
    sSerial.print('a');
  if (digitalRead(reset) == HIGH) {
    zero();
    toSet();
  }
}

void updateDigits() {
  unsigned long timecnt;
  for (timecnt = millis(); millis() - timecnt < 1000;)
    LED4_Display();
  if (countdown > 0)
    minus(1);
}

void bl() {
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
}

// setup() and loop()
void setup() {
  Serial.begin(9600);
  sSerial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DIO, OUTPUT);

  pinMode(start, INPUT);
  pinMode(reset, INPUT);
  pinMode(add15, INPUT);
  pinMode(add30, INPUT);
  pinMode(useLast, INPUT);
}

void loop() {
  poll();
  updateDigits();
}

// LED array logic
int toInteger();
void setArray(int integer);

void zero() {
  for (int i = 0; i <= 3; i++)
    LED[i] = 0;
  countdown = 0;
}

void setV(int value) {
  setArray(value);
  countdown = value;
}

void minus(int value) {
  int k = toInteger();
  k -= value;
  setArray(k);
  countdown -= value;
}

void plus(int value) {
  int k = toInteger();
  k += value;
  setArray(k);
  countdown += value;
}

// Utils
int toInteger() {
  int k = 0, a = 10;
  for (int i = 2; i >= 0; i--) {
    k += LED[i] * a;
    a *= 10;
  }
  k += LED[3];
  return k;
}

void setArray(int integer) {
  int k = integer;
  for (int i = 3; i >= 0; i--) {
    LED[i] = k % 10;
    k /= 10;
  }
}

void LED4_Display(void) {
  unsigned char *led_table;          // 查表指针
  unsigned char i;
  // 第1位
  led_table = LED_0F + LED[3];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x01);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  // 第2位
  led_table = LED_0F + LED[2];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x02);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  // 第3位
  led_table = LED_0F + LED[1];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x04);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  // 第4位
  led_table = LED_0F + LED[0];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x08);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
}

void LED_OUT(unsigned char X) {
  unsigned char i;
  for (i = 8; i >= 1; i--) {
    if (X & 0x80)
      digitalWrite(DIO, HIGH);
    else
      digitalWrite(DIO, LOW);
    X <<= 1;
    digitalWrite(SCLK, LOW);
    digitalWrite(SCLK, HIGH);
  }
}
