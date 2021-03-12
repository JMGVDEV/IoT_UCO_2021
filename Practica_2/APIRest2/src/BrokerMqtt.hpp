//#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

bool VerificarSiHayMensajes();
void callback(char *topic, byte *payload, unsigned int length);
bool ConectarMqtt(const char *mqttUser, const char *mqttPassword, String TopicSub[], String TopicRaiz, int NumTopicosSub);
void setupBrokerMqtt(const char *mqttServer, const int mqttPort);
void SuscribirTopicos(String TopicSub[], String TopicRaiz, int NumTopicosSub);
void PublicarTopicos(String TopicPub, String TopicRaiz, String Mensaje);
bool VerificarConexionMqtt(const char *mqttUser, const char *mqttPassword, String TopicSub[], String TopicRaiz, int NumTopicosSub);