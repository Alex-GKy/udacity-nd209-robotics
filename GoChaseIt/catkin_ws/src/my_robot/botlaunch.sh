#/bin/bash

rosservice call gazebo/delete_model my_robot
roslaunch my_robot robot.launch
