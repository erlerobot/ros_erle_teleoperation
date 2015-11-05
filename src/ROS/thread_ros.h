#ifndef THREAD_ROS_H
#define THREAD_ROS_H

#include <qthread.h>
#include <iostream>
#include <sys/time.h>

#include <ros/ros.h>
#include "../shared_memory.h"
#include "mavros_msgs/OverrideRCIn.h"
#include "mavros_msgs/ParamGet.h"
#include "mavros_msgs/SetMode.h"

class Thread_ROS:public QThread
{
public:
    Thread_ROS(Shared_Memory *share_memory);
    ~Thread_ROS();

private:
    Shared_Memory* share_memory;
    ros::Publisher rc_override_pub;
    ros::ServiceClient cl_param;
    ros::ServiceClient cl_mode;

    int RC_Param(std::string s, int i);
    void updateMode();

    bool stop;

protected:
    void run();
};

#endif // THREAD_ROS_H
