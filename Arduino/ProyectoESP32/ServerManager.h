#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <WiFiClient.h>

class ServerManager {
  public:
    static void setup(const char* serverIP, int serverPort);
    static String handleClient();
  private:
    static WiFiClient client;
    static const char* ip;
    static int port;
};

#endif
