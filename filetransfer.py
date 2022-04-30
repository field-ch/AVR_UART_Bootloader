from msilib.schema import Binary    
from pickle import FALSE
from time import sleep

import sys
import serial

comm = serial.Serial()      #create a new serial object and initialize
comm.port = 'COM3'
comm.baudrate = 9600
comm.parity = serial.PARITY_NONE
comm.bytesize = serial.EIGHTBITS
comm.stopbits = serial.STOPBITS_ONE



app = open(sys.argv[1], 'rb')    #open the firmware binary file provided via the command line argument
bytes = app.read()
app.close()
numbytes = len(bytes)   #calculate the file size
appsz = bytearray()
appsz.append(numbytes)

comm.open()
comm.write(appsz)
comm.close()

comm.open()
comm.write(bytes)   #send the firmware update to the AVR
comm.close()
