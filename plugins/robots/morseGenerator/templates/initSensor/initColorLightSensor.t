@@PORT@@ = VideoCamera()
@@PORT@@.translate(x=0.7, z=1.0)
@@PORT@@.properties(cam_width=20)
@@PORT@@.properties(cam_height=20)
@@PORT@@.add_interface('socket')
robot.append(@@PORT@@)