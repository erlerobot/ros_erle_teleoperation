#include "subscribe_mavros_state.h"

Subscribe_mavros_state::Subscribe_mavros_state(Shared_Memory* shared_memory)
{
    this->shared_memory = shared_memory;
}

void Subscribe_mavros_state::mavrosStateCb(const mavros_msgs::StateConstPtr &msg)
{
    if(msg->mode == std::string("CMODE(0)"))
        return;
    ROS_INFO("I heard: [%s] [%d] [%d]", msg->mode.c_str(), msg->armed, msg->guided);
    this->mode = msg->mode;
    this->guided = msg->guided==128;
    this->armed = msg->armed==128;
    this->shared_memory->setArmed(this->armed);
    this->shared_memory->setMode(this->mode);

}
