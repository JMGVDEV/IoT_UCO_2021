import paho.mqtt.client as mqtt
import time
import urllib.request
import json

#Se conecta y suscribe al topico input
def  on_connect(client, userdata, flags, rc):
        print("El cliente esta conectado")
        client.publish("alive", "Conexión exitosa")
        client.subscribe("input/#")

#Recibe el mensaje al topico suscrito e imprime (mensaje, topico)
def on_message(client, userdata, message):   
    print("Topic "+str(message.topic))
    print("Mensaje recibido " + str(message.payload.decode("utf-8")))
    mensaje = str(message.payload.decode("utf-8"))
    enviar_hora(mensaje)

#Consulta el Json segun la zona dada y manda la hora en MQTT
def enviar_hora(localidad):    
    url = "http://worldtimeapi.org/api/timezone/" + localidad
    try:
        response = urllib.request.urlopen(url)
        client.publish("StatusRequest", "OK")        
        data = json.loads(response.read())
        client.publish("JsonStatus", str(data))
        client.publish("output", data['datetime'])
        print (data['datetime'])
        
    except: client.publish("StatusRequest", "Error 404")
    

    


#Parametros de conexión
servidormqtt = "driver.cloudmqtt.com"
usuario ="aspbcaly"
password = "HlxL2hvTmTxC"
port = 18815 

#Conexión al cliente de cloudMQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(usuario, password=password)
client.connect(servidormqtt, port = port)

#nicio del servicio
client.loop_start()
time.sleep(1)
#Validar la conectividad del servidor
while True:
    client.publish("ValidarConexión", "Conexión activa")
    time.sleep(30)
client.loop_stop()
client.disconnect()