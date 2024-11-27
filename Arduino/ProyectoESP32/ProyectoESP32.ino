#include "WiFiManager.h"
#include "ServerManager.h"
#include "DFPlayerManager.h"

void setup() {
  Serial.begin(115200);
  WiFiManager::connectWiFi("Palacios", "30732322");
  ServerManager::setup("192.168.1.123", 5000);
  DFPlayerManager::init(27, 26);
}

void loop() {
  String command = ServerManager::handleClient(); // Recibe comandos
  if (!command.isEmpty()) {
    if (command == "pause") {
      DFPlayerManager::pause();
    } else if (command == "play") {
      DFPlayerManager::play();
    } else if (command == "stop") {
      DFPlayerManager::stop();
    } else {
      Serial.println("Comando no reconocido.");
    }
  }
}
