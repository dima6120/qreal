def read_touch_sensor(port):
	data = port.get(timeout=0.1)
	for e in data['range_list']:
		if e < .2:
			return 1
	return 0