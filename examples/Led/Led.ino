#include <ShiftRegister.h>

#define SER_PIN 2
#define SCK_PIN 4
#define RCK_PIN 6
// Arduino Uno SS_PIN
// (https://www.arduino.cc/en/Reference/SPI)
#define SS_PIN 10

/* Create instance of ShiftRegister for digital pin*/
ShiftRegister sr(SER_PIN, SCK_PIN, RCK_PIN, 3);
/* for SPI */
//ShiftRegister sr(SS_PIN, 3);
/*
 * Arduino  | ShiftRegister
 * ---------+--------------
 * MOSI     | SER
 * MISO     | NC
 * SCK      | SRCLK
 * SS       | RCLK
 */

void setup() {
  
}

int num = 0;
int last = 0;
int a = 1;
void loop() {
  delay(100);
  if (num > 23) {num = 23; a = -1;}
  if (num < 0) {num = 0; a = 1;}
  sr.digitalWrite(num, HIGH);
  sr.digitalWrite(last, LOW);
  last = num;
  num += a;
}
