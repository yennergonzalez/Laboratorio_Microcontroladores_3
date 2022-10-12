#Código extraído de:
#https://www.learnrobotics.org/blog/arduino-data-logger-csv/?utm_source=youtube&utm_medium=description&utm_campaign=arduino_CSV_data_logger

import serial
import csv

arduino_port = "/tmp/ttyS1" #serial port of Arduino
baud = 9600 #arduino uno runs at 9600 baud
fileName="Arduino_data.csv" #name of the CSV file generated

ser = serial.Serial(arduino_port, baud)
file = open(fileName, "w")

samples = 1000 #how many samples to collect
print_labels = False
line = 0 #start at 0 because our header is 0 (not real data)

while line <= samples:
    getData=ser.readline()
    dataString = getData.decode('utf-8')
    data=dataString[0:][:-2]
    print(data)
    line = line+1

    file = open(fileName, "a")
    file.write(data)

print("Data collection complete!")
file.close()

