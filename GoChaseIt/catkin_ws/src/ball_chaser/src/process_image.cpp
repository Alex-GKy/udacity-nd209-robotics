#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "geometry_msgs/Twist.h"
#include <iostream>

using namespace std;

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float angular_z, float linear_x)
{
    ROS_INFO("Request to move received");

    ball_chaser::DriveToTarget srv; 
    srv.request.angular_z = angular_z; 
    
    // TODO set this
    srv.request.linear_x = linear_x; 
    
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
 
}


// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    // ROS_INFO_STREAM("image");
    int white_pixel = 255;
    bool ball_found = false;
    float angular_z = 0;

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
                int y_position = i % img.step;

                // ball is to the left (so we turn left)
                if (y_position < img.step / 3)
                    angular_z = 1; 

                // ball is at the center
                else if (y_position >= img.step / 3 && y_position < img.step / 3 * 2)
                    angular_z = 0;
                
                // ball is to the right
                else if (y_position >= img.step / 3 * 2 && y_position < img.step)
                    angular_z = -1; 

                cout << ("angular_z is ") << angular_z << "\n"; 

                drive_robot(angular_z, 0.2); 

                ball_found = true;
                // ROS_INFO_STREAM("Ball!");
            }
        
        i++; 
    }

    if (!ball_found) {
        
        // stop the robot
        drive_robot(0,0); 
        cout << "no ball\n";
    }
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