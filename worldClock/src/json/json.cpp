#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Credentials.h"
#include "mqtt/MQTTConnector.h"
#include <PubSubClient.h>

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

void DeserializeObject(String payload){    
    //Procesamiento de la trama JSON.
    MQTTPublish(TOPICJSONSTATUS, "Processing JSON frame...");     
    StaticJsonDocument<300> doc;
    deserializeJson(doc, payload);

    //Configurar día de la semana como texto.    
    String dateTime = doc["datetime"];       
    int dayOfTheWeek = doc["day_of_week"];         
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
    MQTTPublish(TOPICOUTPUT, variable);     
}