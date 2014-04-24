while not (read_light_sensor(@@PORT@@) @@SIGN@@ @@PERCENTS@@):
	time.sleep(0.01)
