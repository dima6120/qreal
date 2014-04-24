while read_color_sensor(@@PORT@@) != @@COLOR@@:
	time.sleep(0.01)
	pass