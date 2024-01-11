import serial
import time
import paho.mqtt.publish as publish
import json
from datetime import datetime

uart_baudrate = 115200 #debe ser = que el configurado en el tx, este caso la EK
freno = 0
# LINUX
# Para listar dispositvos conectados en la terminal: $ lsusb 
#con cable USB
uart_serial = serial.Serial('/dev/ttyUSB0',115200, timeout=10,
parity=serial.PARITY_NONE,
stopbits=serial.STOPBITS_ONE,
bytesize=serial.EIGHTBITS)


if uart_serial.is_open:
    while True:
        size = uart_serial.inWaiting()
        if size:
            data = uart_serial.read(size)
 # El dato recibido está en formato de bytes
            received_data_string = data.decode('utf-8')
            print (received_data_string)
            with open('registros.txt','a') as archivo:
                fecha_hora_actual = datetime.now()
                cadena_fecha_hora = fecha_hora_actual.strftime("%Y-%m-%d %H:%M:%S")
                cadena_fecha_horaa = fecha_hora_actual.strftime("%H:%M:%S")
                archivo.write(f'{cadena_fecha_hora}  : {received_data_string} \n') 
                try:
                    valor = int(received_data_string)
                except ValueError:
                    valor = 0
                


            if valor>=12288 and valor<=16384:
                freno = freno +1
                if freno == 1 :
                    #1. definir el topic 
                    topic = "pot"
                    # 2. definir el hostname
                    mensaje= {
                        "timestamp": cadena_fecha_horaa,
                        "value": valor,
                    }
                    mensaje_json= json.dumps(mensaje)
                    hostname = "grupo10" #hostname de vuestra RPi
                    publish.single(topic=topic, payload=mensaje_json, qos=1, hostname=hostname,keepalive=60)
                    print("Pub done")      
            else:
                freno = 0      
else:
    print ("Puerto serie no abierto")



