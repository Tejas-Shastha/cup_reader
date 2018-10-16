#include "ros/ros.h"
#include "std_msgs/String.h"
#include "hri_package/Sens_Force.h"
#include "sensor_msgs/Imu.h"
#include "rosbag/recorder.h"

#include <fstream>
#include <stdio.h>

#define SAVE_PATH "/home/tejas"

void force_callback(const hri_package::Sens_Force& msg)
{
    ROS_INFO_STREAM("Sensor F : " << msg.forceF);
    ROS_INFO_STREAM("Sensor B : " << msg.forceB);

    std::ofstream  save_file;

    save_file.open("/home/tejas/catkin_ws/data/forceF.txt", std::ios_base::app);
    //Including the std::ios_base::app parameter causes to append to file, else rewrites each time
    save_file << msg.forceF <<"\n";
    save_file.close();

    save_file.open("/home/tejas/catkin_ws/data/forceB.txt", std::ios_base::app);
    save_file << msg.forceB <<"\n";
    save_file.close();
}

//void imu1_cb(const sensor_msgs::Imu& msg)
//{
//    ROS_INFO_STREAM("IMU1 : " << msg  );
//    recorder.run();
//}

//void imu2_cb(const sensor_msgs::Imu& msg)
//{
//    ROS_INFO_STREAM("IMU2 : " << msg  );
//}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "cup_reader");
    ros::NodeHandle nh;
    ros::NodeHandle p_nh("~");

    std::string homepath;
    p_nh.param<std::string>("homepath",homepath,"/home/tejas/catkin_ws");

    rosbag::RecorderOptions options;

    options.prefix = "/home/tejas/catkin_ws/data/cup_reader";

    std::vector<std::string> topics;
    topics.push_back("/imu1/data");
    topics.push_back("/imu2/data");
    topics.push_back("/force_values");

    options.topics = topics;

    rosbag::Recorder* recorder = new rosbag::Recorder(options);


    ROS_INFO_STREAM("Enter 1 to start recording, CTRL+C to finish. Destination :" << homepath );
    int key;
    std::cin >> key;
    if (key==1)
    {
        ros::Subscriber force_sub = nh.subscribe("/force_values", 1000, force_callback);
        recorder->run();
    }
    //ros::Subscriber imu1_sub = nh.subscribe("/imu1/data", 1000, imu1_cb);
    //ros::Subscriber imu2_sub = nh.subscribe("/imu2/data", 1000, imu2_cb);

    ros::spin();

    return 0;
}
