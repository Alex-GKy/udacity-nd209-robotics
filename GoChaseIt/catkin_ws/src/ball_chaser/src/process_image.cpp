#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <iostream>

using namespace std;

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    // ROS_INFO_STREAM("image");
    int white_pixel = 255;
    bool ball_found = false;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    int i = 0; 
    while (!ball_found && i < img.height * img.step){

        // ROS_INFO_STREAM(img.data[i]);
            // cout << img.data[i];

            if (img.data[i] == white_pixel)
            {
                int y_position = i % 800;
                if (y_position < 800 / 3)
                    cout << ("left\n");
                else if (y_position >= 800 / 3 && y_position < 800 / 3 * 2)
                    cout << ("center\n");
                else if (y_position >= 800 / 3 * 2 && y_position < 800)
                    cout << ("right\n");

                ball_found = true;
                // ROS_INFO_STREAM("Ball!");
            }
        
        i++; 
    }

    if (!ball_found) cout << "no ball\n";
    
}

int main(int argc, char **argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;
    ROS_INFO("ROS started");
    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}