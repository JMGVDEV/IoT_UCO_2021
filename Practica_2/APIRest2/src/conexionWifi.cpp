#include <conexionWifi.hpp>

bool ConectarWifi(const char *RED, const char *Password) //se pasan las credenciales para conectar a la red
{
    WiFi.begin(RED, Password);        //iniciando la conexion
    Serial.println("Conectando a: "); //se imprime la red a la que se conecta
    Serial.print(RED);

    while (WiFi.status() != WL_CONNECTED) //se queda en este ciclo hasta que se conecte
    {
        delay(500);
        Serial.print("*");
    }
    Serial.println("Red Conectada"); //se imprime que la red se conecto con exito
    Serial.println("La Direccion IP es: ");
    Serial.print(WiFi.localIP()); //se imprime la ip que se le asigno
    return true;                  //retorna un bool en true, indicando que se conecto a la red
}

bool VerificarConexionWifi(const char *RED, const char *Password) //si la conexion se cayó, reconecta la red
{
    if (WiFi.status() != WL_CONNECTED) //mira si hay conexion a la red o no
    {
        return ConectarWifi(RED, Password); //retorna true cuando hay conexion a la red
    }
    else
    {
        return true; //si la red ya esta conectada, retorna true
    }
}