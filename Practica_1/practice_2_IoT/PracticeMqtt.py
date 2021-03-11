#Practica 2 - MQTT Y REST
#Se importa la librería de paho mqtt que permite la conexión con un cliente Mqtt
#Se importa la libreria dateutil con el fin de formatear la fecha de tipo string que llega desde el servicio a una datetime
#Se importa la librería time que proporciona varias funciones relacionadas con el tiempo.
import paho.mqtt.client as mqtt 
import dateutil.parser as parser
import time, urllib.request, json


#Se declara una función que será ejecutada por la libreria MQTT cuando se logre una conexión.
#Imprime en consola que el cliente esta conectado para llevar un registro del proceso
#Se publica un mensaje de conexión exitosa con el tópico alive
#Se suscribe al tópico input
def  conexionEstablecida(client, userdata, flags, rc):
        print("El cliente ha establecido una conexión")
        client.publish("estado", "Conexión exitosa al broker de mensajeria")
        client.publish("alive", "Conexión exitosa al broker de mensajeria")
        client.subscribe("inputData/#")
        

#Se declara una funcion que sera ejecutada por la líbreria MQTT cuando se recibe un mensaje
#Imprime el tópico en consola
#Imprime el mensaje en consola (UTF-8 es opcional, simplemente se utiliza para limpiar el mensaje de caracteres errados) 
#Se almacena en una vble la consulta por medio del mensaje desde el broker
#Esta variable va como parametro a la función donde se esta consumiento el servicio
def mensajeRecibido(client, userdata, message):
    print("Tópico subscrito: " +str(message.topic))
    print("Mensaje recibido: " +str(message.payload.decode("utf-8")))
    request = str(message.payload.decode("utf-8"))
    worldTimeApi(request)


#Credenciales para establer una conexión
#Servidor Mqtt, esta info esta en los detalles del broker de mensajería cloudMqtt
#usuario, esta info esta en los detalles de la instancia del broker de mensajería cloudMqtt
#contraseña, esta info esta en los detalles de la instancia del broker de mensajería cloudMqtt
#puerto, esta info esta en los detalles de la instancia del broker de mensajería cloudMqtt
server = "driver.cloudmqtt.com" 
user ="aspbcaly" 
password = "HlxL2hvTmTxC" 
port = 18815 


#Se declara una instacia del cliente MQTT.
#Le digo a la librería que función tiene que ejecutar cuando se complete la conexión.
#Le digo a la librería que función tiene que ejecutar ante la llegada de un mensaje. 
client = mqtt.Client()
client.on_connect = conexionEstablecida;
client.on_message = mensajeRecibido;


#Setemos el usuario y la contresela del broker MQTT (CloudMQTT)
#Nos conectamos al servidor MQTT mediante su respectivo puerto.
client.username_pw_set(user, password=password)
client.connect(server, port = port)


#Inicio del servicio
client.loop_start()
time.sleep(1)


#Esta funcion WorldTimeApi consume el servicio, tiene como parametro la zona horaria que se desea consultar
#Url donde se exponen los datos tipo json, esta en la documentacion de world time api
#Con response y loadData obtengo la informacion de la consulta
#Con loadDataformat tomo la caracteristica 'datetime' del objeto json se capturo
#Se cambia la fecha de tipo string a tipo datetime y posteormente se le asigna el formato.
def worldTimeApi(zonaHoraria):

    
    url = "http://worldtimeapi.org/api/timezone/" + zonaHoraria
    try:
        response = urllib.request.urlopen(url)
        loadData = json.loads(response.read())
        loadDataformat = loadData['datetime']
        formatoPresentacionFecha = parser.parse(loadDataformat)
        print (formatoPresentacionFecha.isoformat())
        print (formatoPresentacionFecha.strftime("%A, %d de %B de %Y -- %H:%M:%S")) 
        client.publish("output", formatoPresentacionFecha.strftime("%A, %d de %B de %Y -- %H:%M:%S"))
        client.publish("statusRequest", "OK 201")
        
    except: client.publish("statusRequest", "Error 404 not fount")


#Se genera un loop infinito para ir solicitando al usuario que ingrese nuevos mensajes
#Se publica un mensaje en el tópico estado de conexion activa para identificar que esta esperando un mensaje
while True:
    time.sleep(1)
client.loop_stop()
client.disconnect()