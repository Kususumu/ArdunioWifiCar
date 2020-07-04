#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket(81);

const char *ssid  = "softAPI";  // You will connect your phone to this Access Point
const char *pw    = "thereisnospoon"; // and this is the password

IPAddress ip(192, 168, 4, 1);
IPAddress netmask(255, 255, 255, 0);

void startWebSocket() { // Start a WebSocket server
  webSocket.begin(); // start the websocket server
  webSocket.onEvent(webSocketEvent); // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED: // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: { // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        if ( payload != NULL) {
          Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        }
      }
      break;
    case WStype_TEXT: // if new text data is received,
      if (payload != NULL) {
        // I used an if statement here for the payload because when the payload was NULL, I was getting some weird errors from the ESP8266 and the device was resetting itself.
        Serial.printf("[%u] command: %s\n", num, payload);
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);

  delay(1000);

  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pw); // configure ssid and password for softAP
  startWebSocket();
  Serial.printf("Soft AP created: %s", WiFi.softAPIP().toString().c_str());
}


void loop() {
  webSocket.loop();
}
