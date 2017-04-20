/* Brain-sucking ROMbie
 * Based on the tutorial and sample code here:
 * https://www.nycresistor.com/2012/07/07/stick-a-straw-in-its-brain-and-suck-how-to-read-a-rom/
 *
*/


#include <stdint.h>
// Set MAX_ADDR to the largest address you need
// to read. For example, for the 27C512 chips,
// you'll want to use a MAX_ADDR of 65536.
// (That's 512 * 1024 / 8.)

// A 2708 would be 8 kilobits, or 8 * 1024 / 8 =
// 1024.
#define MAX_ADDR 2048L

// On my board, I've connected pins 26-41
// to the Address0-A15 lines, and pins 2-10 to the
// Data0-Data7 lines. You'll want to change these
// pin choices to match your setup.
#define Address0 0
#define Data0 12
#define LEDPin 11



// When you're all wired up, hit the reset button
// to start dumping the hex codes.

void setup() {

  pinMode(LEDPin, OUTPUT);



  for (int i = Address0; i < Address0 + 11; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  }
  for (int i = Data0; i < Data0 + 8; i++) {
    digitalWrite(i, HIGH);
    pinMode(i, INPUT);
  }
  Serial.begin(300);
}

void writeAddr(uint32_t addr) {
  uint32_t mask = 0x01;
  for (int i = Address0; i < Address0 + 11; i++) {
    if ((mask & addr) != 0) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
    mask = mask << 1;
  }
}


uint8_t readByte() {
  uint8_t data = 0;
  uint8_t mask = 0x1;

  uint8_t data2 = 0;
  uint8_t mask2 = 0x1;

  // digitalWrite(LEDPin, HIGH);

  delay(30);

  for (int i = Data0; i < Data0 + 8; i++) {
    //for (int i = Data0 + 7; i > Data0 - 1 ; i--) { // for systems where the bit order is reversed
    if (digitalRead(i)) {
      data |= mask;
    }
    mask = mask << 1;
  }

  delay(20);

  for (int i = Data0; i < Data0 + 8; i++) {
    //for (int i = Data0 + 7; i > Data0 - 1 ; i--) {
    if (digitalRead(i)) {
      data2 |= mask2;
    }
    mask2 = mask2 << 1;
  }
  // digitalWrite(LEDPin, LOW);
  if (data == data2) {
    return data;
  } else {
    Serial.print("*");
    readByte(); // try again
  }


  // read byte twice. if they don't match, ERROR
}

void loop() {
  uint32_t addr = 0;
  while (addr < MAX_ADDR) {
    for (int i = 0; i < 8; i++) {
      writeAddr(addr);
      uint8_t b = readByte();
      if (b < 16) Serial.print("0");
      Serial.print(b, HEX);
      Serial.print(" ");
      addr++;
      delay(30);
    }
    Serial.println("");
  }
  while (1) {}


}
