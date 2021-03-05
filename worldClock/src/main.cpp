#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "Credentials.h"
#include "mqtt/MQTTConnector.h"
#include "rest/REST.h"

#define WIFI_TIMEOUT 1000

void WiFiBegin(const char* ssid, const char* pass)
{
  //Conexión a WiFi.
  WiFi.begin(ssid,pass);
  Serial.println("Waiting connection ...\n");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_TIMEOUT);
    Serial.printf(".");
  }
  IPAddress ip = WiFi.localIP();
  Serial.printf("IP : %d.%d.%d.%d\n", ip[0],ip[1],ip[2],ip[3]);
}

void setup() 
{
  //Iniciar WiFi y llamado a servicio mqtt.
  Serial.begin(115200);    
  WiFiBegin(STA_SSID, STA_PASS);  
  MQTTBegin();      
}

void loop() 
{    
  MQTTLoop();   
  delay(4000);  
}
