#include <Arduino.h>
#include <SoftwareSerial.h>
#include "AD5763.h"

void serialTrigger(String message);

PINSConfig pins;
AD5763 ad5763(pins);
unsigned char input[2]  = {0x0F, 0x0F};
unsigned char input2[2] = {0x00, 0x00};

void setup() {
  Serial.begin(115200);
}

void loop() {
  serialTrigger(F("Press to continue to send a write message."));
  ad5763.write(Register::data, DAC::B, input);
  serialTrigger(F("Press to continue to send a read message."));
  ad5763.read(Register::data, DAC::B);
}

void serialTrigger(String message) {
  Serial.println();
  Serial.println(message);
  Serial.println();

  while (!Serial.available())
    ;

  while (Serial.available())
    Serial.read();
}