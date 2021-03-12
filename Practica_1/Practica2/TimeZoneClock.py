import json
import time
import urllib.request
import sys
import paho.mqtt.client as mqtt
import dateutil.parser as parser

#Se conecta y suscribe al topico input
def  on_connect(client, userdata, flags, rc):
        print("El cliente esta conectado")
        client.publish("alive", "Conexión exitosa")
        client.subscribe("inputservicio/#")

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
        client.publish("outputservicio", formato_hora(data['datetime']))          
    except: client.publish("StatusRequest", str(sys.exc_info()[0]))

#Cambia el formato que viene del Json al formato (Jueves, 18 de Febrero de 2021 -- 17:00)    
def formato_hora(stringHora):
        horaJSON = parser.parse(stringHora)
        diaDiccionario = {"Monday": "lunes", "thuesday": "Martes", "Wednesday": "Miércoles", "thursday" : "Jueves", "Friday": "Viernes", "Saturday": "Sábado", "Sunday": "Domingo"}
        mesDiccionario = {"January": "Enero", "February": "Febrero","March": "Marzo", "April": "Abril", "May": "Mayo", "June": "Junio", "July": "Julio", "August": "Agosto","September": "Septiembre", "October": "Octubre", "November": "Noviembre", "December": "Diciembre"}     
        dia = diaDiccionario[horaJSON.strftime("%A")]
        mes = mesDiccionario[horaJSON.strftime("%B")]
        nuevoFormatoHora = dia + ", " + horaJSON.strftime("%d") + " de " + mes + horaJSON.strftime(" de %Y -- %H:%M") 
        return(nuevoFormatoHora)

#Parametros de conexión
servidormqtt = "****"
usuario ="****"
password = "*****"
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
