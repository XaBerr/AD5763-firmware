
#include "AD5763.h"

AD5763::AD5763(const PINSConfig& _pins, bool _debugMode)
    : message{0},
      pins{_pins},
      debugMode{_debugMode},
      spiSettings(16000000, MSBFIRST, SPI_MODE1) {
  if (debugMode)
    printf("Debug: Started initialization\n");
  SPIClass SPI0(VSPI);
  SPI.begin();
  pinMode(pins.syncNegate, OUTPUT);
  digitalWrite(pins.syncNegate, HIGH);
  if (debugMode)
    printf("Debug: PIN syncNegate: %d\n", pins.syncNegate);
  pinMode(pins.clrNegate, OUTPUT);
  digitalWrite(pins.clrNegate, HIGH);
  if (debugMode)
    printf("Debug: PIN clrNegate: %d\n", pins.clrNegate);
  pinMode(pins.ldacNegate, OUTPUT);
  digitalWrite(pins.ldacNegate, LOW);
  if (debugMode)
    printf("Debug: PIN ldacNegate: %d\n", pins.ldacNegate);
  pinMode(pins.rstinNegate, OUTPUT);
  digitalWrite(pins.rstinNegate, HIGH);
  if (debugMode)
    printf("Debug: PIN rstinNegate: %d\n", pins.rstinNegate);
  if (debugMode)
    printf("Debug: Completed initialization\n");
}

AD5763::~AD5763() {
}

void AD5763::write(Register reg, int dac, const unsigned char* message) {
  this->message[0] = IO::W | reg | dac;
  this->message[1] = message[1];
  this->message[2] = message[0];
  if (debugMode) {
    printf("Debug: Writing: %02x %02x %02x\n", this->message[0], this->message[1], this->message[2]);
    printBin(this->message);
  }
  SPI.beginTransaction(spiSettings);
  digitalWrite(pins.syncNegate, LOW);
  SPI.transfer(this->message, 3);
  digitalWrite(pins.syncNegate, HIGH);
  SPI.endTransaction();
}

const unsigned char* AD5763::read(Register reg, int dac) {
  // REQUEST
  message[0] = IO::R | reg | dac;
  message[1] = 0;
  message[2] = 0;
  if (debugMode) {
    printf("Debug: Reading: %02x %02x %02x\n", message[0], message[1], message[2]);
    printBin(message);
  }
  SPI.beginTransaction(spiSettings);
  digitalWrite(pins.syncNegate, LOW);
  SPI.transfer(message, 3);
  digitalWrite(pins.syncNegate, HIGH);

  // NOP
  message[0] = IO::W;
  message[1] = 0;
  message[2] = 0;
  // if (debugMode) {
  //   printf("Debug: NOP: %02x %02x %02x\n", message[0], message[1], message[2]);
  //   printBin(message);
  // }
  digitalWrite(pins.syncNegate, LOW);
  SPI.transfer(message, 3);
  digitalWrite(pins.syncNegate, HIGH);

  SPI.endTransaction();
  if (debugMode) {
    printf("Debug: Message read: %02x %02x %02x\n", message[0], message[1], message[2]);
    printBin(message);
  }
  return message;
}

void AD5763::printBin(unsigned char* message) {
  for (size_t i = 0; i < 24; i++)
    printf("%d", message[int(i / 8)] >> (7 - (i % 8)) & 0x01);
  printf("\n");
}
