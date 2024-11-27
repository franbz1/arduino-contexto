#include "DFPlayerManager.h"

HardwareSerial DFPlayerManager::mySerial(2);

void DFPlayerManager::init(int rxPin, int txPin) {
  mySerial.begin(9600, SERIAL_8N1, rxPin, txPin);
  Serial.println("DFPlayer Mini inicializado.");
}

void DFPlayerManager::play() {
  mySerial.write(0x0D); // Comando para reproducir
  Serial.println("Reproducción iniciada.");
}

void DFPlayerManager::pause() {
  mySerial.write(0x0E); // Comando para pausar
  Serial.println("Reproducción pausada.");
}

void DFPlayerManager::stop() {
  mySerial.write(0x16); // Comando para detener
  Serial.println("Reproducción detenida.");
}
