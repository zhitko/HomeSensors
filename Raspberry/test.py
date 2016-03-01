#!/usr/bin/python
import os
import subprocess
import time
import datetime
import serial

def run():
    while(True):
        ser = serial.Serial("/dev/ttyAMA0", 9600)
        line = ser.readline().strip()
        splitList = line.split(":")

    	values=dict(zip(splitList[0::2],splitList[1::2]))

        print("Got data %s" % values)
run()
