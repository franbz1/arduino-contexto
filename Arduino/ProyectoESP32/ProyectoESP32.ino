#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <esp_system.h> // Biblioteca para funciones del sistema, incluida ESP.restart()

// Pines para el DFPlayer Mini
#define PIN_MP3_TX 26  // Conecta al RX del módulo
#define PIN_MP3_RX 27  // Conecta al TX del módulo

DFRobotDFPlayerMini player;

// Constantes para mensajes JSON
const char *MSG_DFPLAYER_CONECTADO = "{\"estado\":\"DFPlayer conectado.\"}";
const char *MSG_DFPLAYER_ERROR = "{\"error\":\"Fallo al conectar con DFPlayer Mini.\"}";
const char *MSG_COMANDO_NO_RECONOCIDO = "{\"error\":\"Comando no reconocido.\"}";

int pistaG = 0;

// Prototipos de funciones
void procesarComando(const String &comando);
void comandoPlay(int pista);
void comandoPause();
void comandoResume();
void comandoVolumen(int volumen);
void comandoStatus();
void comandoReset();
void manejarEventosDFPlayer();

void setup() {
  Serial.begin(115200); // Inicializa el puerto serie para depuración y comandos
  Serial2.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX); // Inicializa Serial2 para el DFPlayer Mini

  if (player.begin(Serial2)) { // Conecta al DFPlayer Mini
    Serial.println(MSG_DFPLAYER_CONECTADO);
    player.volume(20); // Configura el volumen inicial (rango 0-30)
  } else {
    Serial.println(MSG_DFPLAYER_ERROR);
  }

  Serial.setTimeout(100); // Configura un tiempo de espera para recibir comandos incompletos
}

void loop() {
  // Procesar comandos desde el puerto serie
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim(); // Elimina espacios innecesarios
    procesarComando(comando);
  }

  // Manejar eventos emitidos por el DFPlayer Mini
  manejarEventosDFPlayer();
}

void procesarComando(const String &comando) {
  if (comando.startsWith("play")) {
    String numeroPista = comando.substring(5); // Extrae el número de pista
    if (numeroPista.toInt() > 0 || numeroPista == "0") { // Valida que sea un número válido
      int pista = numeroPista.toInt();
      pistaG = pista;
      comandoPlay(pista);
    } else {
      Serial.println("{\"error\":\"Número de pista inválido.\"}");
    }
  } else if (comando == "pause") {
    comandoPause();
  } else if (comando == "resume") {
    comandoResume();
  } else if (comando.startsWith("volumen")) {
    String nivelVolumen = comando.substring(8); // Extrae el nivel de volumen
    if (nivelVolumen.toInt() >= 0 || nivelVolumen == "0") { // Valida que sea numérico
      int volumen = nivelVolumen.toInt();
      comandoVolumen(volumen);
    } else {
      Serial.println("{\"error\":\"Nivel de volumen inválido.\"}");
    }
  } else if (comando == "status") {
    comandoStatus();
  } else if (comando == "reset") {
    comandoReset();
  } else {
    Serial.println(MSG_COMANDO_NO_RECONOCIDO);
  }
}

// Implementación de los comandos

void comandoPlay(int pista) {
  if (pista > 0) {
    player.loop(pista); // Reproduce la pista en bucle
    Serial.println("{\"estado\":\"Reproduciendo\",\"pista\":" + String(pista) + "}");
  } else {
    Serial.println("{\"error\":\"Número de pista no válido.\"}");
  }
}

void comandoPause() {
  player.pause(); // Pausa la reproducción
  Serial.println("{\"estado\":\"Pausa\"}");
}

void comandoResume() {
  player.start(); // Reanuda la reproducción
  Serial.println("{\"estado\":\"Reanudando reproducción\"}");
}

void comandoVolumen(int volumen) {
  if (volumen >= 0 && volumen <= 30) { // Rango válido de volumen
    player.volume(volumen); // Ajusta el volumen
    Serial.println("{\"estado\":\"Volumen ajustado\",\"nivel\":" + String(volumen) + "}");
  } else {
    Serial.println("{\"error\":\"Nivel de volumen no válido (0-30).\"}");
  }
}

void comandoStatus() {
  int volumenActual = player.readVolume(); // Lee el volumen actual
  Serial.println("{\"estado\":\"Conectado\",\"volumen\":" + String(volumenActual) + "}");
}

void comandoReset() {
  Serial.println("{\"estado\":\"Reiniciando dispositivo...\"}");
  delay(100); // Pausa para permitir que el mensaje se envíe antes de reiniciar
  ESP.restart(); // Reinicia el ESP32
}

void manejarEventosDFPlayer() {
  if (player.available()) { // Comprueba si hay eventos del DFPlayer Mini
    uint8_t tipo = player.readType(); // Tipo de evento
    int valor = player.read();        // Valor asociado al evento

    switch (tipo) {
      case DFPlayerPlayFinished: // Evento: Pista terminada
        Serial.println("{\"evento\":\"Reproducción terminada\",\"pista\":" + String(valor) + "}");
        player.play(pistaG);
        break;
      case DFPlayerError: // Evento: Error en el DFPlayer Mini
        Serial.println("{\"error\":\"Error en el DFPlayer\"}");
        break;
      default: // Otros eventos
        Serial.println("{\"evento\":\"Desconocido\",\"tipo\":" + String(tipo) + "}");
        break;
    }
  }
}
