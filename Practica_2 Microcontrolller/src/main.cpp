#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include <bits/stdc++.h>

const char* ssid = "myWifi";
const char* pass = "myWifiPassword";
const char* brokerUser = "myUser";
const char* brokerPassword = "myPassword";
const char* broker = "myBrokerServer";
const char* inputTopic = "input";
const char* outputTopic = "output";
const char* aliveTopic = "alive";
const char* backendInputTopic = "BackendInput";
const char* backendOutputTopic = "BackendOutput";

WiFiClient espClient;
PubSubClient client(espClient);

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

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to ");
    Serial.println(broker);

    if (client.connect("test", brokerUser, brokerPassword)) {
      Serial.println("Connected to");
      Serial.println(broker);
      if (client.connected()) {
        client.publish(aliveTopic, "Succesful Connection");
        client.subscribe(inputTopic);
        client.subscribe(backendOutputTopic);
      }
    }
    else {
      Serial.println("Trying to connect again");
      delay(5000);
    }
  }
}
 
void callback(char* topic, byte* payload, unsigned int length){
  Serial.println("Message obtained: ");
  Serial.println(topic);
  String message = "";
  String topico = "";
  topico += topic;
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  char arr[message.length() + 1];
  strcpy(arr, message.c_str());

  char arr2[topico.length() + 1];
  strcpy(arr2, topico.c_str());

  if (topico == backendOutputTopic) {
    client.publish(outputTopic, (char*)arr);
  } 
  if (topico == inputTopic) {
    client.publish(backendInputTopic, (char*)arr);
  }
}
 
void setup() {
    Serial.begin(115200);
    setupWifi();
    client.setServer(broker, 18815);
    client.setCallback(callback);
}
 
void loop() {
    if (!client.connected()){
      reconnect();
    }
    client.loop();
}

