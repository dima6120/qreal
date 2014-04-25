import time
import sys

sys.path.append('@@MORSEROOT@@/Lib/site-packages')

import random
from consts import *
from readfunctions import *

try:
    from pymorse import Morse
except ImportError:
    print("you need first to install pymorse, the Python bindings for MORSE!")
    sys.exit(1)
	
isUsed = [False, False, False]
LPower = 0
RPower = 0

def setMotorSpeed(port, power):
	global RPower
	global LPower
	global isUsed
	global motion
	if port == PORT_C:
		if not isUsed[PORT_A]:
			LPower = power
			isUsed[PORT_C] = True
		elif not isUsed[PORT_B]:
			RPower = power
			isUsed[PORT_C] = True
	elif port == PORT_A:
		if isUsed[PORT_C]: 
			if isUsed[PORT_B]:
				isUsed[PORT_C] = False
			else:
				RPower = LPower
		LPower = power
		isUsed[PORT_A] = True
	elif port == PORT_B:
		if isUsed[PORT_C]: 
			if isUsed[PORT_A]:
				isUsed[PORT_C] = False
		RPower = power
		isUsed[PORT_B] = True
		
	ls = 2 * LPower / 100
	rs = 2 * RPower / 100
	v = (rs + ls) / 2
	w = rs - ls
	motion.publish({"v": v, "w": w})

def stopMotor(port):
	global RPower
	global LPower
	global isUsed
	global motion
	if port == PORT_C:
		if not isUsed[PORT_A]:
			LPower = 0
		elif not isUsed[PORT_B]:
			RPower = 0
	elif port == PORT_A:
		LPower = 0
	elif port == PORT_B:
		RPower = 0
	isUsed[port] = False
	ls = 2 * LPower / 100
	rs = 2 * RPower / 100
	v = (rs + ls) / 2
	w = rs - ls
	motion.publish({"v": v, "w": w})

simu = Morse()
	
motion = simu.robot.motion

@@GETSENSORS@@

@@VARIABLES@@

@@SUBPROGRAMS@@

#Main task
def main():
@@MAIN_CODE@@

main()