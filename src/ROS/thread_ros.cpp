#include "thread_ros.h"

Thread_ROS::Thread_ROS(Shared_Memory* share_memory)
{
    this->share_memory = share_memory;

    ros::NodeHandle n;
    rc_override_pub = n.advertise< mavros::OverrideRCIn >("/mavros/rc/override", 10);
    cl_param = n.serviceClient<mavros::ParamGet>("/mavros/param/get");
    cl_mode = n.serviceClient<mavros::SetMode>("/mavros/set_mode");

    std::vector<int> rc_maxlimtis;
    std::vector<int> rc_minlimtis;

    for(int i = 0; i < 4; i++){
//        std::cout << RC_Param("_MAX", i) << std::endl;
        rc_maxlimtis.push_back(RC_Param(std::string("_MAX"), i));
    }
    for(int i = 0; i < 4; i++){
        std::cout << RC_Param("_MIN", i) << std::endl;
        rc_minlimtis.push_back(RC_Param(std::string("_MIN"), i));
    }
    share_memory->setRC_maxlimits(rc_maxlimtis);
    share_memory->setRC_minlimits(rc_minlimtis);
//    for(int i = 0; i < 4; i++){
//        std::cout << RC_Param("_TRIM", i) << std::endl;
//    }
}

int Thread_ROS::RC_Param(std::string s, int i)
{
    mavros::ParamGet srv;
    std::stringstream ss;
    ss << (i+1);
    std::string param = std::string("RC") + ss.str() + s;
    srv.request.param_id = param;
    if(cl_param.call(srv)){
        ROS_INFO("send ok %d value: %d", srv.response.success, srv.response.integer);
        if(srv.response.success)
            return srv.response.integer;
        else
            return -1;
    }else{
        ROS_ERROR("Failed arming or disarming");
        return -1;
    }
}

void Thread_ROS::updateMode()
{
    if(share_memory->getMode()!=share_memory->getModeChange() && !share_memory->getModeChange().empty()){
        mavros::SetMode srv;
        srv.request.base_mode = 0;
        srv.request.custom_mode = share_memory->getModeChange();
        if(cl_mode.call(srv)){
            ROS_INFO("send ok %d value:", srv.response.success);
        }else{
            ROS_ERROR("Failed SetMode");
            return;
        }
    }

}

void Thread_ROS::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    int cycle_control = 20;

    while(1){

        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

//        share_memory->update();

        updateMode();

        mavros::OverrideRCIn msg_override;
        msg_override.channels[0] = share_memory->getRoll();
        msg_override.channels[1] = share_memory->getPitch();
        msg_override.channels[2] = share_memory->getThrottle();
        msg_override.channels[3] = share_memory->getYaw();
        msg_override.channels[4] = 1100;
        msg_override.channels[5] = 1100;
        msg_override.channels[6] = 1100;
        msg_override.channels[7] = 1100;
        rc_override_pub.publish(msg_override);

        ros::spinOnce();

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_control)
            diff = cycle_control;
        else
            diff = cycle_control - diff;

        usleep(diff * 1000);
    }
}
