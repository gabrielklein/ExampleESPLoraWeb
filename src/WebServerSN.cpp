#include "WebServerSN.h"

/**
 * Construct the webserver
 */
WebServerSN::WebServerSN() {
        this->webServer = new ESP8266WebServer(80);
        SPIFFS.begin();

        uint8_t mac[WL_MAC_ADDR_LENGTH];
        WiFi.softAPmacAddress(mac);

        String m = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
        this->macId = m;
}

/**
 * Destroy the webserver
 */
WebServerSN::~WebServerSN() {
        delete(this->webServer);
}

/**
 * Launch the server (call it in the setup phase!)
 */
void WebServerSN::setup() {
        Serial.println("WebServerSN: Starting webserver");
        this->webServer->onNotFound([&] () {
                this->servFiles();
        });
        this->webServer->begin();

        // Start MDNS
        String mDNSName = "LoRa-" + macId;

        char mDNSNameChar[mDNSName.length() + 1];
        memset(mDNSNameChar, 0, mDNSName.length() + 1);

        for (int i = 0; i < mDNSName.length(); i++)
                mDNSNameChar[i] = mDNSName.charAt(i);

        if (MDNS.begin(mDNSNameChar)) {
                MDNS.addService("http", "tcp", 80);
                Serial.println("mDNS, respond to name SensorNode: " + mDNSName);
        }
        else {
                Serial.println("Error setting up MDNS responder!");
        }
}

/**
 * Loop the server (call it in the loop phase!)
 */
void WebServerSN::loop() {
        this->webServer->handleClient();
};

/**
 * Send a response and write an header that will not be stored in cache.
 */
void WebServerSN::send(int responseCode, String content, String mime) {

        // Open the file and send the content
        String name(this->webServer->uri());

        // Avoid caching
        this->webServer->sendHeader("Expires","Sun, 01 Jan 2017 00:00:00 GMT");
        this->webServer->sendHeader("Cache-Control","no-store, no-cache, must-revalidate");
        this->webServer->sendHeader("Pragma","no-cache");

        this->webServer->setContentLength(content.length());
        this->webServer->send(responseCode, mime, content);
};

/**
 * Get mime
 */
String WebServerSN::getMime(String name) {

        if (name.equals("/")) {
                return "text/html";
        }
        else if (name.endsWith(".png")) {
                return "image/png";
        }
        else if (name.endsWith(".jpg")) {
                return "image/jpeg";
        }
        else if (name.endsWith(".js")) {
                return "application/javascript";
        }
        else if (name.endsWith(".css")) {
                return "text/css";
        }
        else if (name.endsWith(".html")) {
                return "text/html";
        }
        else if (name.endsWith(".txt")) {
                return "text/plain";
        }
        else if (name.endsWith(".json")) {
                return "application/json";
        }
        else if (name.endsWith(".xml")) {
                return "application/xml";
        }

        return "text/html";
};


/*
 * Serve files that are stored locally.
 * Please note that all files should be in lower case.
 * Please note that only alphanumeric, digital, ., /, _ and - are accepted to avoid risks of hack
 */
void WebServerSN::servFiles() {

        // Open the file and send the content
        String name(this->webServer->uri());

        if (name.equals("/")) {
                name += "index.html";
        }

        String nameLC = String(name);
        nameLC.toLowerCase();


        // Check the chars in the name
        for (int i=0; i<name.length(); i++) {
                char c = name.charAt(i);
                if (isAlphaNumeric(c) || c == '.' || c == '/'|| c == '-' || c == '_'|| isDigit(c)) {
                        // Success
                }
                else {
                        Serial.println("WebServerSN: Forbidden access to this file!");
                        String s2 = "<html><head><title>No access</title></head><body>Forbidden char in the name!</body></html>";
                        this->send(403, s2, "text/html");
                        return;
                }
        }

        // Open Files
        File webFile = SPIFFS.open(name, "r");

        if (webFile) {
                String mime = getMime(name);
                this->webServer->streamFile(webFile, mime);
                webFile.close();
                return;
        }


        Serial.print("WebServerSN: File not found! ");
        Serial.println(name);
        String s2 = "<html><head><title>File not found</title></head><body>Sorry this file does not exists.</body></html>";
        this->send(404, s2, "text/html");

};
