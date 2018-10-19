#include "ros/ros.h"
#include "std_msgs/String.h"
#include "joint/joint_state.h"
#include "sensor_msgs/JointState.h"
#include "ethercatmaster.hpp"
#include "copleyaem.hpp"
#include "ds402slave.hpp"
#include "ec_state.hpp"
#include "slave.hpp"
int main(int argc, char **argv)
{
  CopleyAEM aem(0);
  aem.doEtherCATStateControl(1000);
  ros::init(argc, argv, "joint_state_pub");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 1000);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    std_msgs::String msg;
    msg.data = "hello world";

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
