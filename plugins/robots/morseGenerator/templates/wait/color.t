while read_color_sensor(@@PORT@@, NONE_COLOR) != @@COLOR@@:
	time.sleep(0.01)
	pass