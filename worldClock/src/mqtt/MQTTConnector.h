#ifndef ARDUINO_MQTTCONNECTOR_H
#define ARDUINO_MQTTCONNECTOR_H

#include <Arduino.h>

void    MQTTBegin();
void    MQTTLoop();
boolean MQTTPublish(const char *topic, const char *payload);
boolean MQTTSubscribe(const char* topicToSubscribe);
boolean MQTTIsConnected();
const char * convToChar(int num);
void callback(char* topic, byte* payload, unsigned int length);

#endif /* ARDUINO_MQTTCONNECTOR_H */
