#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_srvs/Trigger.h"
// #include<cstdlib>
using namespace std;
ros::Publisher velocity_publisher;
ros::Publisher angle_publisher;
ros::Subscriber distance_subscriber;
ros::Subscriber heading_subscriber;
float distance_pose;
float heading_pose;
std_msgs::Float32 vel_msg;
std_msgs::Float32 rot_msg;
// std_msgs::Float32 ang_msg,vel_msg;
void distanceCallback(const std_msgs::Float32::ConstPtr &distance_message);
void headingCallback(const std_msgs::Float32::ConstPtr &heading_message);
float pid(float error);
void move();
float i;
float d;
float lp;
float error;
float kp=2;
float ki=0;
float kd=50;

void distanceCallback(const std_msgs::Float32::ConstPtr &distance_message){
    distance_pose=distance_message->data;
    ROS_INFO("%f",distance_pose);
}

float pid( float error){   
    i+=error;
    d=error-lp;
    lp=error;
    return kp*error+kd*d+ki*i;
}

void headingCallback(const std_msgs::Float32::ConstPtr &heading_message){
    heading_pose=heading_message->data;
    ROS_INFO("%f",heading_pose);
    rot_msg.data =pid(heading_pose);
}
void move(){
    int count =0;
    while (ros::ok()){
        std_msgs::Float32 vel_msg;
        // std_msgs::Float32 rot_msg;
        vel_msg.data=10;
        angle_publisher.publish(rot_msg);
        velocity_publisher.publish(vel_msg);
        cout<<"sachin";
        ros::spinOnce();
        ros::Rate loop_rate(10);
        loop_rate.sleep();
        ++count;
    }
}
int main(int argc, char **argv){
    ros::init(argc,argv,"torpedo_distance");
    ros::NodeHandle n;
    velocity_publisher=n.advertise<std_msgs::Float32>("/torpedo/propeller/velocity",1000);
    angle_publisher=n.advertise<std_msgs::Float32>("/torpedo/rudder/angle",1000);
    distance_subscriber=n.subscribe("/torpedo/hydrophones/distance",100,distanceCallback);
    heading_subscriber=n.subscribe("/torpedo/hydrophones/headings",100,headingCallback);
    
    ros::ServiceClient client=n.serviceClient<std_srvs::Trigger>("/torpedo/warhead/detonate");
    std_srvs::Trigger srv;
    if(distance_pose<10){
        if(client.call(srv)){
            cout<<srv.response.success<<" "<<srv.response.message;
        }
        else{
            cout<<"not in range"<<endl;
            return 1;
        }
    }
    ROS_INFO("\n\n\n******START TESTING************\n");

	ros::Rate loop_rate(10);
    move();
    ros::MultiThreadedSpinner spinner(4);
    spinner.spin();
    return 0;
}


