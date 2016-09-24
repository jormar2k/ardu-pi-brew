import spidev
import time
import zmq
import json

# ZeroMQ Context
context = zmq.Context()
# Define the socket using the "Context"
sock = context.socket(zmq.REP)
sock.bind("tcp://127.0.0.1:5678")

spi = spidev.SpiDev()
spi.open(0,0)

hltSetTemp = 0

while True:
	#resp = spi.readbytes(1)
	#print resp
	#time.sleep(1)
	recievedData = sock.recv()
	recievedJsonData = json.loads(json.loads(recievedData))
	
	if recievedJsonData['msg'] == 'GET_BREWERY_TEMPS':
		data = {}
		data['boilTemp'] = '98.4'
		data['hltTemp'] = '66.2'
		data['meshTemp'] = '66.1'
		jsonData = json.dumps(data)

		sock.send_json(jsonData)
	elif recievedJsonData['msg'] == 'SET_HLT_SETPOINT':
		hltSetTemp = recievedJsonData['hltSetTemp']
		data = {}
		data['hltSetTemp'] = hltSetTemp
		jsonData = json.dumps(data)
		sock.send_json(jsonData);