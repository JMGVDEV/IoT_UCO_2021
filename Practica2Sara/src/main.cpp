#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <sstream>
 
const char* ssid = "";
const char* password =  "";
const char* mqttServer = "maqiatto.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* TopicSub = "";
const char* TopicPub = "";
const char* TopicStatusRequest = " ";
const char* TopicJsonStatus = "";
const char* TopicAlive = "";

WiFiClient espClient;
PubSubClient client(espClient);



/********* Setup wifi ***************************
   setup wifi connect to wifi with the constants
   defined up
   while does not connect print a "."
   if connect then print the local ip3
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




// Fecha
 void Date (String dateTime, String dayweek)       {
    int dayOfTheWeek = dayOfTheWeek["day_of_week"];         
    String meses[12] = {"de Enero de ", " de Febrero de ", " de Marzo de ", " de Abril de "," de Mayo de "," de Junio de "," de Julio de "," de Agosto de ", " de Septiembre de "," de Octubre de ", " de Noviembre de "," de Diciembre de "};
    String dias[7] = {"Lunes, ", "Martes ", "Miercoles, ", "Jueves, ","Viernes, ","Sábado, ","Domingo ,"};

    String output;    
    int mes,year,hora1,hora2,hora3; 
    mes = (dateTime[5] - 48)*10 + (dateTime[6] - 48);
    //ESPwdtFeed();         
    year = (dateTime[0] - 48)*1000 + (dateTime[1] - 48)*100 + (dateTime[2] - 48)*10 + (dateTime[3] - 48);  
    hora1 = (dateTime[11] - 48)*10 + (dateTime[12] - 48);
    hora2 = (dateTime[14] - 48); 
    hora3 = (dateTime[15] - 48);

    output = dias[dayOfTheWeek - 1] + dayOfTheWeek + meses[mes-1] + year + " -- "+ hora1 + ":" + hora2 + hora3;
    Serial.println(output.c_str());
    
    const char *variable = output.c_str();
    client.publish(TopicPub, variable);     
}
// funcion de obtener Json

void obtenerJson(String txt){
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, txt);
  if (error) {return;}
  String datetime = doc["datetime"];
  String dayweek = doc["day_of_week"];  

  client.publish(TopicJsonStatus, "Json Processed");    
  Serial.println("Json Processed");
  Date(datetime,dayweek);
}
//Convertir a CHart
const char *ConvertirtoChar(int numero){
  std::stringstream temporal;
  temporal << numero;
  const char *conv = temporal.str().c_str();
  return conv;
}


// Processing URL and access at API
void processResponse(int httpCode, HTTPClient& http)
{
   if (httpCode > 0) {
      Serial.printf("Response code: %d\t", httpCode);
 
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();   
        client.publish(TopicStatusRequest, "OK");      
        Serial.println("Llamada http ok");
        obtenerJson(payload);
      }
      else{
        const char* err = ConvertirtoChar( httpCode );
        client.publish(TopicStatusRequest, err);
      }
      
   }
   else {
      Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
      const char* err = ConvertirtoChar(httpCode);
      client.publish(TopicStatusRequest, err);
   }
   http.end();
}
// function that recive the message

void GetMessage(String id){

  HTTPClient http;
  http.begin( "http://worldtimeapi.org/api/timezone/" + id);
  int httpCode = http.GET();
  processResponse (httpCode, http);

}




/********* MQTT Callback ***************************
   here is defined the logic to execute after
   a messages arrives in the desired
   topic, for this example the variable:
   TopicSub
************************************************/


void callback(char* topic, byte* payload, unsigned int length) {
  String messagetopic;
  //Notify about message arrived 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  //Print the message received
  Serial.print("Message:");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messagetopic += (char)payload[i];
  }

//Prints with visualization purposes
  Serial.println();
  Serial.println("-----------------------");
  GetMessage(messagetopic);
 
}

void setup() {
 
  //Start Serial Communication
  Serial.begin(115200);
  
  //Connect to WiFi
  setup_wifi();

  //Connect to MQTT Broker
  client.setServer(mqttServer, mqttPort);
 
 
  //MQTT Connection Validation
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
      client.publish(TopicAlive,"Connect");
 
    }
    else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }


  
  //Publish to desired topic and subscribe for messages
  client.publish(TopicPub, "Hello from ESP8266");
  client.subscribe(TopicSub);
   client.setCallback(callback);
 
}
 
void loop() {
  //MQTT client loop
  client.loop();
}