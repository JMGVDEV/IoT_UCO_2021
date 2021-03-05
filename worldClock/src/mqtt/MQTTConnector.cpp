#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTTConnector.h"
#include "Credentials.h"
#include <sstream>
#include "rest/Rest.h"

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

boolean mqttInitCompleted = false;
String clientId = "IoTPractice-" + String(ESP.getChipId());

/* Incoming data callback. */
void dataCallback(char* topic, byte* payload, unsigned int length)
{
  char payloadStr[length + 1];
  memset(payloadStr, 0, length + 1);
  strncpy(payloadStr, (char*)payload, length);    
}

void performConnect()
{
  //Realizar conexión al broker con un cliente.
  uint16_t connectionDelay = 5000;
  while (!mqttClient.connected())
  {
    Serial.printf("Intentando conexion MQTT...\n");
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
      Serial.printf("Conectado al Broker.\n");
      /* Subscription to your topic after connection was succeeded.*/
      MQTTSubscribe(TOPICINPUT);      
      MQTTPublish(TOPICALIVE, "I'm alive!, successful connection to broker.");
    }
    else
    {     
      Serial.printf("Error!  : Conexion MQTT fallida, rc = %d\n", mqttClient.state());
      Serial.printf("Trace   : Intentado de nuevo en %d msec.\n", connectionDelay);
      delay(connectionDelay);
    }
  }
}

boolean MQTTPublish(const char *topic,const char *payload)
{
  //Publica en un tópico.
  boolean retval = false;
  if (mqttClient.connected())
    retval = mqttClient.publish(topic, payload);  
  
  return retval;
}

boolean MQTTSubscribe(const char* topicToSubscribe)
{
  //Suscripción a tópico.
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.subscribe(topicToSubscribe);
  }
  return retval;
}

boolean MQTTIsConnected()
{
  //Verifica que el cliente MQTT esté conectado.
  return mqttClient.connected();
}

void MQTTBegin()
{
  //Configurar parámetros(broker y puerto) para conexión al broker MQTT.
  mqttClient.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  mqttClient.setCallback(dataCallback);
  mqttInitCompleted = true;

  //Llamado para iniciar conexión MQTT.
  if(mqttInitCompleted)
  {
    if (!MQTTIsConnected())
    {
      performConnect();
    }    
    mqttClient.loop();
  }
}

void MQTTLoop()
{
  /* .loop()":This should be called regularly to allow the client to 
  process incoming messages and maintain its connection to the server.*/
  mqttClient.loop();  
  mqttClient.setCallback(callback);
  delay(10);  
}

const char *convToChar(int num){
  //Convertir de int a const char *.
   std::stringstream tmp;
   tmp << num;
   const char* conv = tmp.str().c_str();
   return conv;
}

void callback(char* topic, byte* payload, unsigned int length) {

  //Notify about message arrived 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  String message = "";    
  Serial.print("Message:");
  //Conversion de entrada tipo byte a String. 
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
    Serial.print((char)payload[i]);
  }   

  //Concatenar url con zona horario ingresada en el tópico.
  String url = "http://worldtimeapi.org/api/timezone/";  
  url = url + message;

  Serial.println();
  Serial.println("-----------------------");               
  Serial.println(url);
  //Llamado a api.
  apiRest(url);

  //Prints with visualization purposes  
  Serial.println("-----------------------");        
}

