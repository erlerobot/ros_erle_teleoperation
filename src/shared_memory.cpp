#include "shared_memory.h"

Shared_Memory::Shared_Memory()
{
    if (pthread_mutex_init(&mutex, NULL) != 0){
        std::cout << "mutex init failed" << std::endl;
    }
}

std::string Shared_Memory::getModeChange()
{
    std::string result;
    pthread_mutex_lock( &mutex );
    result = this->modeChange;
    pthread_mutex_unlock( &mutex );
    return result;
}

void Shared_Memory::setModeChange(std::string s)
{
    pthread_mutex_lock( &mutex );
    this->modeChange = s;
    pthread_mutex_unlock( &mutex );

}

void Shared_Memory::setMode(std::string s)
{
    pthread_mutex_lock( &mutex );
    this->mode = s;
    pthread_mutex_unlock( &mutex );
}

std::string Shared_Memory::getMode()
{
    std::string result;
    pthread_mutex_lock( &mutex );
    result = this->mode;
    pthread_mutex_unlock( &mutex );
    return result;
}

void Shared_Memory::setArmed(bool b)
{
    pthread_mutex_lock( &mutex );
    this->armed = b;
    pthread_mutex_unlock( &mutex );
}

bool Shared_Memory::getArmed()
{
    bool result;
    pthread_mutex_lock( &mutex );
    result = this->armed;
    pthread_mutex_unlock( &mutex );
    return result;
}

void Shared_Memory::setRC_maxlimits(std::vector<int> v)
{
    pthread_mutex_lock( &mutex );
    this->rc_max_limits = v;
    pthread_mutex_unlock( &mutex );
}

void Shared_Memory::setRC_minlimits(std::vector<int> v)
{
    pthread_mutex_lock( &mutex );
    this->rc_min_limits = v;
    pthread_mutex_unlock( &mutex );
}

std::vector<int> Shared_Memory::getRC_maxlimits()
{
    std::vector<int> result;
    pthread_mutex_lock( &mutex );
    result = this->rc_max_limits;
    pthread_mutex_unlock( &mutex );
    return result;
}

std::vector<int> Shared_Memory::getRC_minlimits()
{
    std::vector<int> result;
    pthread_mutex_lock( &mutex );
    result = this->rc_min_limits;
    pthread_mutex_unlock( &mutex );
    return result;
}


void Shared_Memory::setPitch(int var)
{
    pthread_mutex_lock( &mutex );
    this->pitch = var;
    pthread_mutex_unlock( &mutex );
}

void Shared_Memory::setRoll(int var)
{
    pthread_mutex_lock( &mutex );
    this->roll = var;
    pthread_mutex_unlock( &mutex );
}

void Shared_Memory::setYaw(int var)
{
    pthread_mutex_lock( &mutex );
    this->yaw = var;
    pthread_mutex_unlock( &mutex );
}
void Shared_Memory::setThrottle(int var)
{
    pthread_mutex_lock( &mutex );
    this->throttle = var;
    pthread_mutex_unlock( &mutex );
}

int Shared_Memory::getPitch()
{
    int result;
    pthread_mutex_lock( &mutex );
    result = this->pitch;
    pthread_mutex_unlock( &mutex );
    return result;
}

int Shared_Memory::getRoll()
{
    int result;
    pthread_mutex_lock( &mutex );
    result = this->roll;
    pthread_mutex_unlock( &mutex );
    return result;
}

int Shared_Memory::getYaw()
{
    int result;
    pthread_mutex_lock( &mutex );
    result = this->yaw;
    pthread_mutex_unlock( &mutex );
    return result;
}

int Shared_Memory::getThrottle()
{
    int result;
    pthread_mutex_lock( &mutex );
    result = this->throttle;
    pthread_mutex_unlock( &mutex );
    return result;
}
