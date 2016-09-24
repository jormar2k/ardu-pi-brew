from flask import Flask
from flask import request

import json
import zmq

# ZeroMQ Context
context = zmq.Context()

# Define the socket using the "Context"
sock = context.socket(zmq.REQ)
sock.connect("tcp://127.0.0.1:5678")

app = Flask(__name__)
		
@app.route('/temp', methods=['GET', 'POST'])
def brewery():

	if request.method =='GET':
		data = {}
		data['msg'] = 'GET_BREWERY_TEMPS'
		jsonData = json.dumps(data)
		sock.send_json(jsonData)
		
		return sock.recv_json()
                   
	elif request.method == 'POST':
		jsonFromRequest = request.get_json()
		
		data = {}
		data['msg'] = 'SET_HLT_SETPOINT'
		data['hltSetTemp'] = jsonFromRequest['hltSetTemp']
		jsonData = json.dumps(data)
		sock.send_json(jsonData)
		return sock.recv_json()