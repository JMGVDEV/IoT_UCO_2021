#ifndef ARDUINO_CREDENTIALS_H
#define ARDUINO_CREDENTIALS_H

/* WiFi Credentials to connect Internet */

#define STA_SSID "xxxx"
#define STA_PASS "xxxx"

/* Provide MQTT broker credentials as denoted in maqiatto.com. */
#define MQTT_BROKER       "xxxx"
#define MQTT_BROKER_PORT  1883
#define MQTT_USERNAME     "xxxx"
#define MQTT_KEY          "xxxx"


/* Provide topic as it is denoted in your topic list. 
 * For example mine is : cadominna@gmail.com/topic1
 * To add topics, see https://www.maqiatto.com/configure
 */
#define TOPICINPUT          "bmontoyaosorios@gmail.com/input"
#define TOPICALIVE          "bmontoyaosorios@gmail.com/alive"
#define TOPICSTATUSREQUEST  "bmontoyaosorios@gmail.com/StatusRequest"
#define TOPICOUTPUT         "bmontoyaosorios@gmail.com/Output"
#define TOPICJSONSTATUS     "bmontoyaosorios@gmail.com/JsonStatus"

//Suscripciones a tópicos.
#define TOPICSUB            "bmontoyaosorios@gmail.com/input"

#endif /* ARDUINO_CREDENTIALS_H */
