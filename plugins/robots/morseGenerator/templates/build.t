from morse.builder import *

robot = ATRV()
robot.translate(1.0, 0.0, 0.0)

motion = MotionVW()
robot.append(motion)

pose = Pose()
robot.append(pose)

@@INITSENSORS@@

robot.add_default_interface('socket')

env = Environment(@@ENVIRONMENT@@, fastmode = @@FASTMODE@@)
env.set_camera_location([10.0, -10.0, 10.0])
env.set_camera_rotation([1.05, 0, 0.78])