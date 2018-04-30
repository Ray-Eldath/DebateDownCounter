#include <SoftwareSerial.h>

SoftwareSerial sSerial(2, 3); // RX, TX

const unsigned char LED_0F[] = {
  // 0    1     2     3     4     5    6     7     8     9     A     b     C     d     E     F     -
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0xFF, 0xbf
};

unsigned int LED[4] = {0, 0, 1, 0}; // 用于LED的4位显示缓存
unsigned int countdown = 10;

const int SCLK = 13;
const int RCLK = 12;
const int DIO = 11;

void updateDigits();
void poll();

void setup() {
  Serial.begin(9600);
  sSerial.begin(9600);
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DIO, OUTPUT);
}

void loop() {
  poll();
  updateDigits();
}

void updateDigits() {
  unsigned long cnt, timecnt;
  cnt = 0;
  for (timecnt = millis(); millis() - timecnt < 1000;)  {
    LED4_Display();
  }
  cnt += 8;
  if (cnt == 80)
    cnt = 0;
  if (countdown > 0)
    minus(1);
}

void poll() {
  // checkpoints:
  // a - 30s; b - 15s; c - 0s
  if (countdown == 8)
    sSerial.print('a');
}

void minus(int value) {
  int k = 0, a = 10;
  for (int i = 2; i >= 0; i--) {
    k += LED[i] * a;
    a *= 10;
  }
  k += LED[3];
  k -= value;
  for (int i = 3; i >= 0; i--) {
    LED[i] = k % 10;
    k /= 10;
  }
  countdown--;
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
