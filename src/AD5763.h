#ifndef AD75763_H
#define AD75763_H

#include <Arduino.h>
#include <SPI.h>
#include "printf.h"

enum IO {
  R = 1 << 7,
  W = 0 << 7
};

enum Register {
  function   = 0 << 3,
  data       = 2 << 3,
  coarseGain = 3 << 3,
  fineGain   = 4 << 3,
  offset     = 5 << 3
};

enum DAC {
  A  = 0,
  B  = 1,
  AB = 4
};

struct PINSConfig {
  int syncNegate  = 25;
  int sclk        = 5;
  int sdin        = 18;
  int sdo         = 19;
  int clrNegate   = 16;
  int ldacNegate  = 17;
  int rstinNegate = 21;
};

class AD5763 {
  unsigned char message[3];
  PINSConfig pins;
  bool debugMode;
  SPISettings spiSettings;
  void printBin(unsigned char* message);

 public:
  AD5763(const PINSConfig& _pins, bool _debugMode = true);
  ~AD5763();
  void write(Register reg, int dac, const unsigned char* message);
  const unsigned char* read(Register reg, int dac);
};

#endif  // AD75763_H