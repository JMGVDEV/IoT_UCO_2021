//Librería del microcontr
//Librería para el comportamiento de un cliente MQTT
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 

//Credenciales de la red local.
//Nombre de la red y la contraseña.
const char* ssid = "DARKSOUL";
const char* password =  "******";


//Credenciales para establer una conexión
//Servidor Mqtt, esta info esta en los detalles del broker de mensajería cloudMqtt
//usuario, esta info esta en los detalles de la instancia del broker de mensajería cloudMqtt
//contraseña, esta info esta en los detalles de la instancia del broker de mensajería cloudMqtt
//puerto, esta info esta en los detalles de la instancia del broker de mensajería cloudMqtt
const char* mqttServer = "driver.cloudmqtt.com";
const char* mqttUser = "aspbcaly";
const char* mqttPassword = "HlxL2hvTmTxC";
const int mqttPort = 18815;


//Topicos en los cuales se va suscribir y en los cuales va publicar.
//topicos input y output
//Topico para la conexion con el codigo de python que consume el servicio de World Time Api
const char* TopicSub = "input";
const char* TopicPub2 = "output";
const char* TopicPub = "estado";
const char* TopicPubOutputInterno = "inputData";


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

  //Notify about message arrived 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  //Print the message received
  String data;
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data = data +  ((char)payload[i]);
  }
  
  //Prints with visualization purposes
  Serial.println();
  Serial.println("-----------------------");

  const char* dataInOutPut= (char*)data.c_str();
  String topicInput; 
  topicInput = topic;
  if(topicInput == "input"){
    client.publish(TopicPubOutputInterno, dataInOutPut);
  }
}


void setup() {
 
  //Start Serial Communication
  Serial.begin(115200);
  
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
  client.publish(TopicPub, "Conexion exitosa desde ESP8266");
  client.subscribe(TopicSub);
 
}
 
void loop() {
  //MQTT client loop
  client.loop();
}