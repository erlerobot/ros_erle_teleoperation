#include "mavros_setstreamrate.h"

MAVROS_setStreamRate::MAVROS_setStreamRate()
{
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<mavros_msgs::StreamRate>("/mavros/set_stream_rate");
    mavros_msgs::StreamRate srv;
    srv.request.stream_id = 0;
    srv.request.message_rate = 100;
    srv.request.on_off = 1;

    if (client.call(srv)) {
        ROS_INFO("Send OK");
    } else {
        ROS_INFO("Failed to call service");
    }

    ros::ServiceClient cl_get = n.serviceClient<mavros_msgs::ParamGet>("/mavros/param/get");
    mavros_msgs::ParamGet paramget;
    paramget.request.param_id = "SYSID_MYGCS";

    ros::ServiceClient cl = n.serviceClient<mavros_msgs::ParamSet>("/mavros/param/set");
    for(;;) {
        if (cl_get.call(paramget)) {
            ROS_INFO("Send OK %d Value: %ld", paramget.response.success, paramget.response.value.integer);
            if (paramget.response.value.integer == 1) {
                break;
            } else {
                mavros_msgs::ParamSet paramset2;
                paramset2.request.param_id = "SYSID_MYGCS";
                paramset2.request.value.integer = 1;
                paramset2.request.value.real = 1;
                if (cl.call(paramset2)) {
                    ROS_INFO("SYSIS_MYGCS: send ok");
                } else {
                    ROS_INFO("Failed to call service SYSIS_MYGCS");
                }
            }
        } else {
            ROS_ERROR("Failed GET PARAMETER");
        }
    }
}
