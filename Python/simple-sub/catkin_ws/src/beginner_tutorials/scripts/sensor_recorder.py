#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that listens to std_msgs/Strings published 
## to the 'chatter' topic

import rospy
import rosbag
from datetime import datetime
from std_msgs.msg import String
from sensor_msgs.msg import LaserScan
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Image

dt = datetime.timestamp(datetime.now())

odometry_topic = "/odom"
scan_topic = "/scan"
image_topic = "/camera/rgb/image_raw"

scan_bag = rosbag.Bag("{topic}-{timestamp}.bag".format(timestamp=dt, topic=scan_topic.replace('/','')), 'w')
odometry_bag = rosbag.Bag("{topic}-{timestamp}.bag".format(timestamp=dt, topic=odometry_topic.replace('/','')),'w')
image_bag = rosbag.Bag("{topic}-{timestamp}.bag".format(timestamp=dt, topic=image_topic.replace('/','')), 'w')

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)

def scan_callback(data, topic_name): 
    rospy.loginfo(rospy.get_caller_id() + 'Ranges: {0}'.format(data.ranges))
    scan_bag.write(topic_name, data)

def odometry_callback(data, topic_name): 
    rospy.loginfo(rospy.get_caller_id() + 'Odometry received')
    odometry_bag.write(topic_name, data)

def image_callback(data, topic_name):
    rospy.loginfo(rospy.get_caller_id() + 'Image received')
    image_bag.write(topic_name, data)
    
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('sensor_recorder', anonymous=True)

    # rospy.Subscriber('chatter', String, callback)
    #rospy.Subscriber(scan_topic, LaserScan, scan_callback, scan_topic)
    #rospy.Subscriber(odometry_topic, Odometry, odometry_callback, odometry_topic)
    rospy.Subscriber(image_topic, Image, image_callback, image_topic)

    rospy.on_shutdown(shutdown_hook)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def shutdown_hook(): 
    scan_bag.close()
    odometry_bag.close()

if __name__ == '__main__':
    listener()
