#include <ConsumirAPIRest.hpp>

HTTPClient http; //permite hacer consultas http, crea una instancia del objeto

String consumirAPI(String DominioApi, String peticion) //se hace el llamadoa la Api
{
    String cuerpo_respuesta = "";       //respuesta de la Api
    extern int codigo_respuesta;        //el codigo de respuesta Http que es una variable global
    String URL = DominioApi + peticion; //se une la peticion con el dominio de la Api

    http.begin(URL);                  //la api que va a resivir los datos o enviar los datos
    codigo_respuesta = GetApi();      //se hace la peticion a la api, si se quisiera mandar datos, aqui se llamaria el "pos" y lo demas queda igual
    cuerpo_respuesta = ResponseApi(); //esta funcion determina el cuerpo de la respuesta
                                      //y si la peticion fue correcta
    http.end();                       //se finaliza la conexion http del cliente
    return cuerpo_respuesta;          //retorna la respuesta de la API
}

//se procesa la respuesta de la api
String ResponseApi()
{
    String cuerpo_respuesta = ""; //respuesta de la Api
    extern int codigo_respuesta;  //mira si se proceso adecuadamente la peticion http
    if (codigo_respuesta > 0)     //se mira si el servidor contesto
    {
        Serial.println("Codigo http: " + String(codigo_respuesta)); //se imprime el codigo de respuesta
        if (codigo_respuesta == 200)                                //se mira si se tuvo el codigo 200 que indica que se conecto correctamente con el servidor
        {
            cuerpo_respuesta = http.getString(); //se optiene la respuesta del servidor
            Serial.println("el sevidor respondio: ");
            Serial.print(cuerpo_respuesta); //se imprime la respuesta del servidor
        }
    }
    else
    {
        Serial.println("error!!!. El servidor no responde, codigo: "); //se hay algun error en el envio de datos
        Serial.println(codigo_respuesta);
    }
    return cuerpo_respuesta;
}

//se utiliza para hacer un POST  a una Api
int PostApi(String DominioApi, String datos_a_enviar) //se le pasa os parametros que le quiere enviar
{
    extern int codigo_respuesta;
    codigo_respuesta = http.POST(datos_a_enviar); //se envia el post a la api y se guarda la respuesta
    return codigo_respuesta;
}
int GetApi() //la peticion Get a l api para obtener la feha y hora indicada
{
    int codigo_respuesta = http.GET(); //-->se traen los datos que se le pidio por el URL;
    return codigo_respuesta;
}