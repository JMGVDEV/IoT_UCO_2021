#include <Arduino.h>
#include <conexionWifi.hpp>
#include <PubSubClient.h>
#include <ConsumirAPIRest.hpp>
#include <BrokerMqtt.hpp>
#include <ManejoJson.hpp>

//******************************Credenciales de wifi************************
const char *RED = "MARIO JARAMILLO";
const char *Password = "****";
//****************************credenciales de mqtt**************************
const char *mqttServer = "maqiatto.com"; //direccion ip del host o broker

const int mqttPort = 1883;                       // puerto por el que se va a comunicar-->Este es sin la forma segura (SSL/TLS)
const char *mqttUser = "emmanuel84jg@gmail.com"; //autentificacion de broker
const char *mqttPassword = "*****";
//**********************************Topicos********************************
//topico raiz--> es decir que todos descienden de este
String TopicRaiz = "emmanuel84jg@gmail.com/";
//Topicos de subscripcion
String TopicSub[] = {"input"};
int NumTopicosSub = 1;
//Topicos de publicacion
String TopicPub[] = {"output", "alive", "StatusRequest", "JsonStatus"};
//input-->se suscribe para obtener  el lugar de donde se quiere obtener la hora
//output--> se publica la hora solicitada
//alive-->publica si hubo exito en la conexion del broker Mqtt
//StatusRequest--> Publica si se proceso con exito la peticion Http
//JsonStatus--> Publica si se proceso con exito el Json (Rest)
//********************************************************************************
//variables globales
extern String peticion = "";          //se guarda el lugar de donde se quiere saber la hora
extern int codigo_respuesta = 0;      //se guarda el codigo de respuesta a peticion http
extern String ProcesamientoJson = ""; //dice si se procesó o no el Json (rest)
//*********************************************************************************
String DominioApi = "http://worldtimeapi.org/api/timezone/";
String DatosResponseApi = ""; //se guarda la respuesta de la Api
String fecha = "";            //se guarda la fecha del lugar solicitado
//**********************************************************************************
void setup()
{
  delay(10);
  Serial.begin(115200);                  //iniciando la velocidad de comunicación serial
  ConectarWifi(RED, Password);           //se conecta el esp8266 a la red
  setupBrokerMqtt(mqttServer, mqttPort); //inicializa los parametros de conexion al broker
}
void loop()
{
  if (VerificarConexionWifi(RED, Password))
  { //mira si hay conexion a la red para poder conectarse al broker mqtt

    if (VerificarConexionMqtt(mqttUser, mqttPassword, TopicSub, TopicRaiz, NumTopicosSub)) //mira si esta conectado al broker mqtt y si no, lo conecta
    {
      VerificarSiHayMensajes(); //verifica si hay peticion del usuario
      if (peticion != "")       //mira si el usuario hizo una peticion
      {

        PublicarTopicos(TopicPub[1], TopicRaiz, "Conexion al Broker Maqiatto-->OK");
        DatosResponseApi = consumirAPI(DominioApi, peticion); //hace la peticion a la Api
        if (codigo_respuesta > 0)                             //para publicar si se proceso la peticion
        {
          PublicarTopicos(TopicPub[2], TopicRaiz, "Peticion Http procesada-->OK");
        }
        else
        {
          PublicarTopicos(TopicPub[2], TopicRaiz, "Error en la peticion Http ");
        }
        ConvertirString_Jseon(DatosResponseApi);        //obtiene el Json que devuelve la Api (Rest)
        fecha = SacarParametrosFecha();                 //le da estructura a la fecha
        PublicarTopicos(TopicPub[0], TopicRaiz, fecha); //se publica la fecha
        if (ProcesamientoJson != "")                    //mira si se proceso el Json
        {
          PublicarTopicos(TopicPub[3], TopicRaiz, ProcesamientoJson);
          ProcesamientoJson = "";
        }
        peticion = ""; //se resetea la peticion
      }
    }
  }
}
