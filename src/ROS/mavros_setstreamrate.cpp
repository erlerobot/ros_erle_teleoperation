#include "mavros_setstreamrate.h"

MAVROS_setStreamRate::MAVROS_setStreamRate()
{
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<mavros::StreamRate>("/mavros/set_stream_rate");
    mavros::StreamRate srv;
    srv.request.stream_id = 0;
    srv.request.message_rate = 100;
    srv.request.on_off = 1;
    if(client.call(srv)){
        ROS_INFO("send ok");
    }else{
        ROS_INFO("Failed to call service");
    }

    ros::ServiceClient cl_get = n.serviceClient<mavros::ParamGet>("/mavros/param/get");
    mavros::ParamGet paramget;
    paramget.request.param_id = "SYSID_MYGCS";

    ros::ServiceClient cl = n.serviceClient<mavros::ParamSet>("/mavros/param/set");
    while(true){
        if(cl_get.call(paramget)){
            ROS_INFO("send ok %d value: %d", paramget.response.success, paramget.response.integer);
            if(paramget.response.integer==1){
                break;
            }else{
                mavros::ParamSet paramset2;
                paramset2.request.param_id = "SYSID_MYGCS";
                paramset2.request.integer = 1;
                paramset2.request.real = 1;
                if(cl.call(paramset2)){
                    ROS_INFO("SYSIS_MYGCS: send ok");
                }else{
                    ROS_INFO("Failed to call service SYSIS_MYGCS");
                }
            }
        }else{
            ROS_ERROR("Failed GET PARAMETER");
        }
    }

//    mavros::ParamSet paramset;
//    paramset.request.param_id = "SYSID_MYGCS";
//    paramset.request.integer = 255;
//    paramset.request.real = 255;
//    if(cl.call(paramset)){
//        ROS_INFO("SYSIS_MYGCS: send ok");
//    }else{
//        ROS_INFO("Failed to call service SYSIS_MYGCS");
//    }

//    mavros::ParamSet paramset2;
//    paramset2.request.param_id = "SYSID_MYGCS";
//    paramset2.request.integer = 1;
//    paramset2.request.real = 1;
//    if(cl.call(paramset2)){
//        ROS_INFO("SYSIS_MYGCS: send ok");
//    }else{
//        ROS_INFO("Failed to call service SYSIS_MYGCS");
//    }

}
