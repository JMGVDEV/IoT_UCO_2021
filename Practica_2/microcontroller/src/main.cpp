#include <Arduino.h>
#include "string.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "PubSubClient.h"

// Define WiFi credentials
const char* ssid = "network";
const char* pass = "password";

// Define MQTT broker credentials
const char* brokerUser = "myuser";
const char* brokerPassword = "mypassword";
const char* broker = "driver.somemqtt.com";

// WiFi and Broker clients to communicate between backends
WiFiClient espClient;
PubSubClient client(espClient);

// HTTP Server
ESP8266WebServer server(80);

// Connects to WiFi using the credentials provided
void setupWifi() {
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("...");
  }

  Serial.println("Connected to ");
  Serial.println(ssid);
}

// Connects to the MQTT Broker
void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to ");
    Serial.println(broker);

    if (client.connect("test", brokerUser, brokerPassword)) {
      Serial.println("Connected to");
      Serial.println(broker);
    }
    else {
      Serial.println("Trying to connect again");
      delay(5000);
    }
  }
}

// Handles the requests on POST /input to then publish the value on the input topic
void handleInput() {
  Serial.println("Attempting to fetch date for:");
  Serial.println(server.arg(String("city")));
  String city = server.arg(String("city"));
  client.publish("input", city.c_str());
  server.send(200, "text/plain", "Received input");
}

// Handles the requests on POST /response to publish the value on the output topic
void handleResponse() {
  Serial.println(server.arg(String("response")));
  String response = server.arg(String("response"));
  client.publish("output", response.c_str());
  server.send(200, "text/plain", "Received response");
}

// Configures the ESP8266 Web server and its 2 endpoints
void setupWebServer() {
  server.on("/input", HTTP_POST, handleInput);
  server.on("/response", HTTP_POST, handleResponse);
  server.begin();
  Serial.println("Listening on: ");
  Serial.println(WiFi.localIP());
}

// Configures the microcontroller
void setup() {
  Serial.begin(115200);
  setupWifi();
  setupWebServer();
  client.setServer(broker, 18820);
}

// Loops handling requests and sending messages through the MQTT client
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  server.handleClient();
}
