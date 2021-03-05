#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Credentials.h"
#include "mqtt/MQTTConnector.h"
#include "json/json.h"
#include <PubSubClient.h>
#include <sstream>

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

String payload;

void apiRest(String url){
    HTTPClient http;    
 
   if (http.begin(url)) //Start connection
   {
      //Problemas para imprimir el código http correcto en el topic.
      MQTTPublish(TOPICSTATUSREQUEST, "(OK)Processing HTTP call...");            
      int httpCode = http.GET();  //GET Request                  
      
      if (httpCode > 0){                     
         char *codeh = "[HTTP] Code: ";  
         switch(httpCode) {             
            case 200: strcat(codeh,"200");         
            break;
            case 404: strcat(codeh,"404");   
            break;            
            case 505: strcat(codeh,"505");   
            break;       
         }
         MQTTPublish(TOPICSTATUSREQUEST, codeh);                                                         
         Serial.printf("(OK)[HTTP] GET... code: %d\n", httpCode);
 
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            //Payload: JSON.
            payload = http.getString();   // Get HTTP response                                                
            DeserializeObject(payload);       
         }
      }
      else {            
            MQTTPublish(TOPICSTATUSREQUEST, "[HTTP] Failed, error: ");         
            MQTTPublish(TOPICSTATUSREQUEST, http.errorToString(httpCode).c_str());                  
      }          
      http.end();
   }
   else {
      MQTTPublish(TOPICSTATUSREQUEST, "[HTTP} Unable to connect."  );               
   }         
}





