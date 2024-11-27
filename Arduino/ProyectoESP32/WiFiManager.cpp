#include "WiFiManager.h"

void WiFiManager::connectWiFi(const char* ssid, const char* password) {
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConexión WiFi establecida.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}
