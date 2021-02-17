#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

const char* ssid = "****";
const char* password = "****";

const int LED2 = 5;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  setupPins();
  connectToWiFi();
  setupRoutes();
}

void loop() {
  server.handleClient();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("*");
  }

  Serial.println("");
  Serial.println("Conexion Wifi exitosa");
  Serial.println("La IP del ESP8266 es:");
  Serial.println(WiFi.localIP());
}

void setupRoutes() {
  server.on("/", handleLED1);
  server.on("/switch-led-1", handleLED1);
  server.on("/switch-led-2", handleLED2);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void setupPins() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void handleLED1() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("Switch del LED 1 (integrado)");
  server.send(200, "text/html", sendHTML());
}

void handleLED2() {
  digitalWrite(LED2, !digitalRead(LED2));
  Serial.println("Switch del LED 2 (externo)");
  server.send(200, "text/html", sendHTML());
}

String sendHTML() {
  String page = "<!DOCTYPE html>\n";
  page += "<html>\n";
  page += "<head>\n";
  page += "   <title>Control de LED</title>\n";
  page += "   <link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Roboto:300,300italic,700,700italic\">\n";
  page += "   <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/normalize/8.0.1/normalize.css\">\n";
  page += "   <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/milligram/1.4.1/milligram.css\">\n";
  page += "</head>\n";
  page += "<body>\n";
  page += "<div class=\"container\">\n";
  page += "   <div class=\"row\">\n";
  page += "       <div class=\"column\">\n";
  page += "           <p><a class=\"button\" href=\"/switch-led-1\">Switch LED 1</a></p>\n";
  page += "       </div>\n";
  page += "       <div class=\"column\">\n";
  page += "           <p><a class=\"button\" href=\"/switch-led-2\">Switch LED 2</a></p>\n";
  page += "       </div>\n";
  page += "   </div>\n";
  page += "</div>\n";
  
  page += "</body>\n";
  page += "</html>\n";
  return page;
}