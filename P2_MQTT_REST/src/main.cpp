#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <sstream>



const char* ssid = "****";
const char* password =  "****";
const char* mqttServer = "maqiatto.com";
const int mqttPort = 1883;
const char* mqttUser = "****";
const char* mqttPassword = "****";
// Tópicos MQTT a utilizar
const char* TopicInput = "jeffer.bg@gmail.com/input";
const char* TopicAlive = "jeffer.bg@gmail.com/alive";
const char* TopicStatusRequest = "jeffer.bg@gmail.com/StatusRequest";
const char* TopicJsonStatus = "jeffer.bg@gmail.com/JsonStatus";
const char* TopicOutPut = "jeffer.bg@gmail.com/OutPut";

String host="http://worldtimeapi.org/api/timezone/"; 

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



//********** Convertir int en const char* *************//
const char *ConvtoChar(int num){
  std::stringstream tmp;
  tmp << num;
  const char *conv = tmp.str().c_str();
  return conv;
}

//************ Organizar fecha en el formato indicado *****************//
void DeserializeObject(String payload){    
    //Se publica en el tópico JsonStatus
    client.publish(TopicJsonStatus, "Processing Json...");     
    StaticJsonDocument<300> doc;
    deserializeJson(doc, payload);

    //Configurar día de la semana como texto.    
    String DateTime = doc["datetime"];       
    int DayWeek = doc["day_of_week"];         
    String Months[12] = {" Enero ", " Febrero ", " Marzo ", " Abril "," Mayo "," Junio "," Julio "," Agosto ", " Septiembre "," Octubre ", " Noviembre "," Diciembre "};
    String Days[7] = {"Lunes, ", "Martes ", "Miercoles, ", "Jueves, ","Viernes, ","Sábado, ","Domingo ,"};

    // Se obtiene el año, el mes, la hora y los minutos  
    int Year = (DateTime[0] - 48)*1000 + (DateTime[1] - 48)*100 + (DateTime[2] - 48)*10 + (DateTime[3] - 48); 
    int Month = (DateTime[5] - 48)*10 + (DateTime[6] - 48); 
    int Hours = (DateTime[11] - 48)*10 + (DateTime[12] - 48);
    int M1 = (DateTime[14] - 48); 
    int M2 = (DateTime[15] - 48);

    // Se concatenan los resultados obtenidos para la fecha
    String Date = Days[DayWeek - 1]  + (DayWeek+7) + " de " + Months[Month-1] + " de " + Year + " -- "+ Hours + ":" + M1 + M2;
    Serial.println(Date.c_str());
    
    // Se publica la fecha
    const char *FechaObtenida = Date.c_str();
    client.publish(TopicOutPut, FechaObtenida);     
}



void processResponse(int httpCode, HTTPClient& http)
{
   if (httpCode > 0) {
      Serial.printf("Response code: %d\t", httpCode);
 
      if (httpCode == HTTP_CODE_OK) {
        // Se almacena el texto en la API
        String payload = http.getString();  
        // Confirma en el tópico StatusRequeste el llamado HTTP   
        client.publish(TopicStatusRequest, "OK"); 
        DeserializeObject(payload);
      }
      else{
        const char* error = ConvtoChar(httpCode);
        client.publish(TopicStatusRequest, error);
      }
      
   }
   else {
      Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
      const char* error = ConvtoChar(httpCode);
      client.publish(TopicStatusRequest, error);
   }
   http.end();
}

// Se obtiene la información de la API
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
      client.publish(TopicAlive, "Message from ESP8266");       // Se publica la conexión exitosa al broker
 
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