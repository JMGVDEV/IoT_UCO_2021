#include <ESP8266HTTPClient.h>

String consumirAPI(String DominioApi, String peticion);
String ResponseApi();
int PostApi(HTTPClient http, String DominioApi, String datos_a_enviar);
int GetApi();