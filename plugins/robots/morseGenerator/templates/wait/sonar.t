while not (read_sonar_sensor(@@PORT@@) @@SIGN@@ @@DISTANCE@@):
	time.sleep(0.01)