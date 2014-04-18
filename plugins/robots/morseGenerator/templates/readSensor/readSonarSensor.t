def read_sonar_sensor(port):
	data = port.get()
	dis = 10.0 
	for e in data['range_list']:
		dis = min(dis, e)
	return dis