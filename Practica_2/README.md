# Practica MQTT + REST

El dispositivo ESP8266 expone un servidor con 2 endpoint, mediante los cuales se recibe el dato de entrada (input) y se publica la respuesta (response) al broker de mensajeria.

El microservicio en Nest.js se encarga de leer los mensajes publicados en input, y con esto intentar obtener la fecha de algún sitio en el mundo, para posteriormente darle formato a la respuesta y mediate una petición POST enviarla al servicio del dispositivo ESP8266, quien es finalmente el que la publica en el tópico output.

## Instalación

Se requiere tener instalado Node.js para la ejecución del microservicio. Una vez clonado el repositorio, desde el directorio backend, se deben instalar los paquetes con el comando:

```bash
$ npm install
```

## Corriendo la aplicación

1. Ejecutar el código en el microcontrolador después de haber cambiado las conexiones de WiFi y MQTT.
2. Configura la conexión al broker MQTT en el archivo [mqtt.config.ts](backend/src/mqtt.config.ts), luego la IP del microcontrolador en [microcontroller.config.ts](backend/src/microcontroller.config.ts) y posteriormente inicia el microservicio con el comando:
3. Correr el backend con `$ npm start`

## Prueba

El servicio que expone el ESP8266, recibe peticiones de tipo `POST` en su endpoint `/input`, estas deben de ser de tipo `multipart/form-data`. Ejemplo (donde 1.1.1.1 se reemplaza por la IP del microcontrolador):

`curl --request POST \ --url http://1.1.1.1/input \ --header 'Content-Type: multipart/form-data' \ --header 'content-type: multipart/form-data; boundary=---011000010111000001101001' \ --form city=America/Bogota`
