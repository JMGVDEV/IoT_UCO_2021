#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Credenciales de la red local
const char* ssid = "****";
const char* password = "****";

//Se asigna pin D0=16
int pinD0Led = 16;

int statepinD0Led = 0;

// WebServer Instantiation
WiFiServer server(80);

//Web Page Variable
String webPage = "";
 
void setup(void){
  //Serial Comunnication and Wifi Connection kick off
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  //Se define led por pin digital y led del microcontrolador
  pinMode(pinD0Led,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("");

  // Wait for connection (Intenta conectarse a la red WiFi)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Los datos de la red en la cual estoy conectado y su ip para la conexión con el servidor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  //Start Web Server and notify via serial comm.
  server.begin();
  Serial.println("HTTP server started");

  //Se apaga led de puerto D0
  digitalWrite(pinD0Led, 0);
}
 
void loop(void){
  //Se instancia cliente WiFi
  WiFiClient client = server.available(); 

  if(!client){
    return;
  }

  while(!client.available()){ 
    delay(1);
  }
  //lee la peticion del cliente
  String peticion = client.readStringUntil('\r'); 
  Serial.println(peticion);
  //limpia la peticion del cliente pero la peticion se mantiene 
  client.flush(); 

  if(peticion.indexOf("LED2=ON") != -1)
  {
    statepinD0Led=1;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if(peticion.indexOf("LED2=OFF") != -1)
  {
    statepinD0Led=0;
    digitalWrite(LED_BUILTIN, LOW);
  }

  //Se le asigna el estado al pin (Dependiendo de las validaciones) prende o apaga
  digitalWrite(pinD0Led, statepinD0Led);

  client.println("HTTP/1.1 200 OK");
  client.println("");
  client.println("");
  client.println("");
  client.println("");
  //pagina del server
  client.println("<!DOCTYPE html>");
  client.println("<html lang='en'>");
  client.println("  <head>");
  client.println("      <meta charset='UTF-8'>");
  client.println("      <meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("      <title>Práctica 1 - IOT</title>");
  client.println("  </head>");
  client.println("  <body");
  client.println("      <div style='border: 2px solid black; margin: 0 auto; width: 800px;'> ");
  client.println("      <br><br>");
  client.println("      <h2 style='color:black; text-align:center;'>Circuito con led integrado y externo</h2>");
  client.println("          <h4 style='color: black; text-align:center;'>Off apaga led y enciende led en microcontrolador</h4><div style='margin: 0 auto; width: 200px'>");
  client.println("          <button style='border-radius: 6px; border: 2px solid #0016b0; background-color: red;'"); 
  client.println("              type='button' onClick=location.href='/LED2=OFF'>APAGAR");
  client.println("          </button>");
  client.println("          <button style='border-radius: 6px; border: 2px solid #0016b0; background-color: #1883ba;'"); 
  client.println("              type='button' onClick=location.href='/LED2=ON'>PRENDER");
  client.println("          </button></div>");
  client.println("      <br>");   
  client.println("      </div>");
  client.println("  </body>");
  client.println("</html>" );
  
  delay(10);

}