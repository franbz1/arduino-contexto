#ifndef DFPLAYERMANAGER_H
#define DFPLAYERMANAGER_H

#include "HardwareSerial.h"

class DFPlayerManager {
  public:
    static void init(int rxPin, int txPin);
    static void play();
    static void pause();
    static void stop();
  private:
    static HardwareSerial mySerial;
};

#endif
