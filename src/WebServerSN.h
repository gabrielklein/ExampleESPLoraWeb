#ifndef WEBSERVERSN_H
#define WEBSERVERSN_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

class WebServerSN {

public:
WebServerSN();
~WebServerSN();
void loop();
void setup();
String macId;

private:
ESP8266WebServer *webServer;
void servFiles();
void send(int responseCode, String content, String mime);
String getMime(String name);
};

#endif
