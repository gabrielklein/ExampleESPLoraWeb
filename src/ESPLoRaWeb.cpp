#include <ESP8266WiFi.h>
#include "Settings.h"
#include <WebSocketsServer.h>
#include <WebServerSN.h>
#include <RH_RF95.h>

WebSocketsServer webSocket = WebSocketsServer(81);
RH_RF95 rf95(RFM95_CS, RFM95_INT);
WebServerSN webServer;

// Please note that part of this code was found in other places.

/**
 * Receive websocket events.
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

        switch(type) {
        case WStype_TEXT:
        {
                Serial.printf("[%u] Send Text: %s\n", num, payload);

                unsigned long start = millis();
                //webSocket.sendTXT(num, "Sending text on LoRa");
                rf95.send(payload, length);
                rf95.waitPacketSent();
                unsigned long duration = millis() - start;

                String s("Sent (");
                s += String(duration);
                s += String(" ms): ");
                s += String((char *)payload);
                webSocket.sendTXT(num, s);
        }
        break;
        case WStype_DISCONNECTED:
                Serial.printf("[%u] Disconnected!\n", num);
                break;
        case WStype_CONNECTED:
        {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                webSocket.sendTXT(num, "Hello! You are connected.");
        }
        break;
        case WStype_BIN: // Not really supported
        {
                Serial.printf("[%u] get binary length: %u\n", num, length);
                hexdump(payload, length);
                // webSocket.sendBIN(num, payload, length);
        }
        break;
        }

}

/**
 * Setup.
 * - Connect to wireless network.
 * - Initialize LoRa module
 */
void setup()
{
        Serial.begin ( 115200 );
        Serial.println ("\nStarting ESPLoRaWeb\n");

        // Setup wireless access and create websockets on port 81
        {

                String ssidS = CLI_SSID;
                String passwordS = CLI_PASSWORD;

                char ssid[ssidS.length()+1];
                char password[passwordS.length()+1];

                ssidS.toCharArray(ssid, ssidS.length()+1);
                passwordS.toCharArray(password, passwordS.length()+1);

                WiFi.begin (ssid, password);
                while ( WiFi.status() != WL_CONNECTED ) {
                        delay(500);
                        Serial.println("Not yet connected");
                }

                #ifndef CLI_DHCP

                IPAddress ip;
                IPAddress subnet;
                IPAddress gateway;
                IPAddress dns;

                ip.fromString(CLI_IP);
                subnet.fromString(CLI_SUBNET);
                gateway.fromString(CLI_GATEWAY);
                dns.fromString(CLI_DNS);

                if (!WiFi.config(ip, gateway, subnet, dns)) {
                        Serial.println("Problems configuring WiFi access (with dns)");
                        return;
                }
                #endif

                IPAddress myIP = WiFi.localIP();
                Serial.println(myIP);

                //hub = new Hub();
                //hub->setup();

                webSocket.begin();
                webSocket.onEvent(webSocketEvent);

                webServer.setup();

        }

        // Setup RF95
        {

                pinMode(RFM95_RST, OUTPUT);
                digitalWrite(RFM95_RST, HIGH);

                // manual reset
                digitalWrite(RFM95_RST, LOW);
                delay(10);
                digitalWrite(RFM95_RST, HIGH);
                delay(10);

                while (!rf95.init()) {
                        Serial.println("LoRa radio init failed");
                        delay(100);
                }

                // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
                if (!rf95.setFrequency(RF95_FREQ)) {
                        Serial.println("setFrequency failed");
                        while (1) ;
                }

                rf95.setTxPower(13, false);

                IPAddress myIP = WiFi.localIP();
                String s = "New module: " + myIP.toString();
                rf95.send((uint8_t *)s.c_str(), s.length());
        }
}

/**
 * Main loop, if we receive messages from LoRa, forward them the webSocket and Serial.
 */
void loop()
{

        webSocket.loop();
        webServer.loop();


        // If we receive messages from LoRa, forward thems
        if (rf95.available()) {
                uint8_t buf[RH_RF95_MAX_MESSAGE_LEN+1];
                for (int i=0; i<RH_RF95_MAX_MESSAGE_LEN; i++) {
                        buf[i] = 0;
                }
                uint8_t len = RH_RF95_MAX_MESSAGE_LEN;
                while (rf95.recv(buf, &len)) {
                        String s;
                        s += (char *)buf;
                        Serial.println(s);
                        webSocket.broadcastTXT(s);
                }

        }
}
