#include "ServerManager.h"

WiFiClient ServerManager::client;
const char* ServerManager::ip;
int ServerManager::port;

void ServerManager::setup(const char* serverIP, int serverPort) {
  ip = serverIP;
  port = serverPort;
  Serial.println("Conectando al servidor...");
  if (client.connect(ip, port)) {
    Serial.println("Conexión exitosa al servidor.");
  } else {
    Serial.println("No se pudo conectar al servidor.");
  }
}

String ServerManager::handleClient() {
  if (client.connected() && client.available()) {
    String command = client.readStringUntil('\n'); // Leer comando completo
    command.trim(); // Eliminar espacios o saltos de línea
    Serial.print("Comando recibido: ");
    Serial.println(command);
    return command; // Devolver comando al archivo principal
  }

  // Reconexión automática si es necesario
  if (!client.connected()) {
    Serial.println("Reconectando al servidor...");
    client.connect(ip, port);
  }

  return ""; // Si no hay comando disponible, devuelve vacío
}
