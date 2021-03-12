#include <BrokerMqtt.hpp>
WiFiClient espClient;
PubSubClient Client(espClient);
// se crea una instancia de la libreria mqtt y se le pasa por donde se va a conectar (espClient)

void setupBrokerMqtt(const char *mqttServer, const int mqttPort) //inicializa la comunicacion Mqtt
{
    Client.setServer(mqttServer, mqttPort); //Al cliente mqtt se le pasa el servidor y el puerto de comunicacion
    Client.setCallback(callback);           //cuando le llegue un mensaje, llama a "callback"
}

//esta funcion mira si esta conectado el Broker Mqtt y si no lo conecta
bool VerificarConexionMqtt(const char *mqttUser, const char *mqttPassword, String TopicSub[], String TopicRaiz, int NumTopicosSub)
{
    while (!ConectarMqtt(mqttUser, mqttPassword, TopicSub, TopicRaiz, NumTopicosSub)) //mientras no este conectado, se queda intentando conectar
    {
        delay(1000);
    }
    return true; //retorna verdadero, cuando logra conectar el broker Maqiatto
}

bool VerificarSiHayMensajes() //devuelve true, si el cliente ha solicitado una hora
{
    return Client.loop(); //este se tiene que estar ejecutando siempre, para que resiva los mensajes y no se
                          //pierdan o acumulen
                          //el codido de la funcion "void loop" no debe ser muy largo pa que se ejecute constantemente "client.loop"
}

//conecta al broker
bool ConectarMqtt(const char *mqttUser, const char *mqttPassword, String TopicSub[], String TopicRaiz, int NumTopicosSub)
{
    bool conectado = true;      //si ya esta conectado, de una me retorna un true
    while (!Client.connected()) //si no esta conectado, se me queda intentando conectar
    {
        Serial.println("Intentando conexion con mqtt...");
        String clientId = "emanuel";
        String clientId1 = clientId + String(random(0xffff), HEX); //Si se cae la conexion genera un nuevo id para que lo deje conectar
        //se conecta el esp8266 con el broker mqtt y si la conexion sale exitosa, el if se pone en true
        if (Client.connect(clientId1.c_str(), mqttUser, mqttPassword)) //conecta al cliente y convierte a string el ID
        {
            Serial.println("Conectado!!");
            SuscribirTopicos(TopicSub, TopicRaiz, NumTopicosSub); //se suscribe al topico "Input"-->resive peticion usuario
        }
        else //si no se logra conectar al broker mqtt
        {
            Serial.println("Falló, con error -> "); //imprime porque no ha podido conectar
            Serial.println(Client.state());         //imprime el error que se produjo
            Serial.println("Nuevo intento en 5 seg");
        }
        delay(5000); //para no saturar el broker mqtt
    }
    return conectado;
}

//se suscribe a los topicos que se le indiquen
void SuscribirTopicos(String TopicSub[], String TopicRaiz, int NumTopicosSub)
{
    String TopicoSub;

    for (int i = 0; i < NumTopicosSub; i++) //recorre el vector de topicos de suscripcion
    {
        TopicoSub = TopicRaiz + TopicSub[i];     //junta el topico raiz con el que quiere suscribirse
        if (Client.subscribe(TopicoSub.c_str())) //se suscribe al topico input y con el "c_str" convierte a caracter la cadena
        {
            Serial.println("Suscripcion ok en el topico: ");
            Serial.print(TopicoSub);
        }
        else
        {
            Serial.println("Suscripcion fallida al topico: ");
            Serial.print(TopicoSub);
        }
    }
}

//para publicar en los topicos
void PublicarTopicos(String TopicPub, String TopicRaiz, String Mensaje)
{
    String TopicoPub = TopicRaiz + TopicPub;
    Client.publish(TopicoPub.c_str(), Mensaje.c_str()); //se suscribe al topico input
    Serial.println(TopicoPub);
    delay(200);
}

//resive el topico, el mensaje o peticion de usuario y el tamaño del mensaje que llega
void callback(char *topic, byte *payload, unsigned int length)
{
    extern String peticion; //variable global, en donde se guarda cual es la peticion del usuario
    String incoming = "";
    Serial.println("Mensaje resivido desde -> ");
    Serial.print(topic); //imprime el topico que resivio
    Serial.println("");
    for (unsigned int i = 0; i < length; i++) //convierte a char el mensaje de tipo Byte
    {                                         //va guardando en "incomming" la cadena "payload" como caracter
        incoming += (char)payload[i];         //se va concatenando
    }
    incoming.trim();                          //elimina los espacios en blanco de la cadena
    Serial.println("Mensaje -> " + incoming); //imprime el mensaje

    peticion = incoming; //se indica la peticion del usuario
}
