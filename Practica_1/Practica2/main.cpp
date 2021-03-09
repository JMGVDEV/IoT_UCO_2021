#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <locale.h>
#include <stdio.h>

//Variables de conexión
const char* ssid = "****";
const char* password =  "*****";
const char* mqttServer = "****";
const int mqttPort = 18815;
const char* mqttUser = "****";
const char* mqttPassword = "****";

//Variables de cominicación en MQTT
const char* TopicSub = "input";
const char* TopicSub2 = "outputservicio";
const char* TopicPub = "inputservicio";
const char* TopicPub2 = "output";

WiFiClient espClient;
PubSubClient client(espClient);


/********* Setup wifi ***************************
   setup wifi connect to wifi with the constants
   defined up
   while does not connect print a "."
   if connect then print the local ip
************************************************/

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print(F("Connecting to ")) ;
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/********* MQTT Callback ***************************
   here is defined the logic to execute after
   a messages arrives in the desired
   topic, for this example the variable:
   TopicSub
************************************************/
void callback(char* topic, byte* payload, unsigned int length) {

  String mensaje;
  //Notify about message arrived 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  //Print the message received
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    mensaje = mensaje + ((char)payload[i]);
    Serial.print((char)payload[i]);
  }
  
  //Prints with visualization purposes
  Serial.println();
  Serial.println("-----------------------");
  //Imprime el mensaje según el tópico que reciba
  const char* mensajeFinal = (char*)mensaje.c_str();
  char* mensajeFinal1 = (char*)mensaje.c_str();
  String topic1 = topic;
  if(topic1 == "input"){
    client.publish(TopicPub, mensajeFinal);
  }
  else if(topic1 == "outputservicio"){ 
    client.publish(TopicPub2, mensajeFinal);
  }
  
}

void setup() {
 
  //Start Serial Communication
  Serial.begin(9600);
  
  //Connect to WiFi
  setup_wifi();

  //Connect to MQTT Broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  //MQTT Connection Validation
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
  //Publish to desired topic and subscribe for messages
  client.publish("alive", "ESP8266 activo");
  client.subscribe(TopicSub);
  client.subscribe(TopicSub2);
}
 
void loop() {
  //MQTT client loop
  client.loop();
}