#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <sstream>



const char* ssid = "";
const char* password =  "";
const char* mqttServer = "";
const int mqttPort = *****;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* TopicInput = "johjan_castano98@hotmail.com/input";
const char* TopicOutput = "johjan_castano98@hotmail.com/Output";
const char* TopicAlive = "johjan_castano98@hotmail.com/alive";
const char* TopicStatReq = "johjan_castano98@hotmail.com/StatusRequest";
const char* TopicJson = "johjan_castano98@hotmail.com/JsonStatus";

String host="http://worldtimeapi.org/api/timezone/";    //URL api

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



//********** Covertir int en const char* *************//
const char *ConvtoChar(int num){
  std::stringstream tmp;
  tmp << num;
  const char *conv = tmp.str().c_str();
  return conv;
}

//************ Organizar fecha en el formato indicado *****************//
void OrgDate(String datetime, String dayweek){
  String dweek[7] = {"Lunes","Martes","Miércoles","Jueves","Viernes","Sábado","Domingo"};
  String months[12] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};
  int day = dayweek[0]-48;   //Se resta 48, al cambiar el tipo de dato a int devuelve el valor en código ASCII
  int mon = (datetime[5]-48)*10 + datetime[6]-48;
  int year = (datetime[0]-48)*1000 + (datetime[1]-48)*100 + (datetime[2]-48)*10 + datetime[3]-48;
  int hour = (datetime[11]-48)*10 + datetime[12]-48;
  int minute = (datetime[14]-48)*10 + datetime[15]-48;

  String fecha = dweek[day-1] + ", " + day + " de " + months[mon-1] + " de " + year + " -- " + hour + ":" + minute;

  Serial.println(fecha);
  const char* fechaC=fecha.c_str();
  client.publish(TopicOutput, fechaC);
}

//************** Convertir texto plano en Json *****************//
void obtJson(String txt){
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, txt);
  if (error) {return;}
  String datetime = doc["datetime"];
  String dayweek = doc["day_of_week"];  

  client.publish(TopicJson, "Json Processed");    // Se publica en JsonStatus que el Json se procesó correctamente.
  Serial.println("Json Processed");
  OrgDate(datetime,dayweek);
}



void processResponse(int httpCode, HTTPClient& http)
{
   if (httpCode > 0) {
      Serial.printf("Response code: %d\t", httpCode);
 
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();       //Se guarda el texto dentro de la API
        client.publish(TopicStatReq, "OK");       // Publicar en StatusRequest confirmación de llamada http
        Serial.println("Llamada http ok");
        obtJson(payload);
      }
      else{
        const char* err = ConvtoChar(httpCode);
        client.publish(TopicStatReq, err);
      }
      
   }
   else {
      Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
      const char* err = ConvtoChar(httpCode);
      client.publish(TopicStatReq, err);
   }
   http.end();
}

//***************** Ingresar y obtener la información de la API *********************//
void GetItem(String id)
{
   HTTPClient http;
   http.begin(host + id);
   int httpCode = http.GET();
   processResponse(httpCode, http);
}

/********* MQTT Callback ***************************
   here is defined the logic to execute after
   a messages arrives in the desired
   topic, for this example the variable:
   TopicSub
************************************************/
void callback(char* topic, byte* payload, unsigned int length) {

  String req;
  //Notify about message arrived 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  //Print the message received
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    req += (char)payload[i];
  }
    
  //Prints with visualization purposes
  Serial.println();
  Serial.println("-----------------------");
  GetItem(req);
  Serial.println(req);
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
      client.publish(TopicAlive, "ESP8266 is connected");       // Se publica la conexión exitosa al broker
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
  //Publish to desired topic and subscribe for messages
  client.subscribe(TopicInput);  
  client.setCallback(callback);
}

void loop() {
  //MQTT client loop
  client.loop();
}