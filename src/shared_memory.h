#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <vector>
#include <pthread.h>
#include <iostream>

class Shared_Memory
{
public:
    Shared_Memory();

    bool getArmed();
    void setArmed(bool b);
    std::string getMode();
    void setMode(std::string s);

    std::string getModeChange();
    void setModeChange(std::string s);

    void setPitch(int var);
    void setRoll(int var);
    void setYaw(int var);
    void setThrottle(int var);

    int getPitch();
    int getRoll();
    int getYaw();
    int getThrottle();

    void setRC_maxlimits(std::vector<int>);
    void setRC_minlimits(std::vector<int>);

    void setOverride(bool b);
    bool getOverride();

    std::vector<int> getRC_maxlimits();
    std::vector<int> getRC_minlimits();
private:
    pthread_mutex_t mutex;

    int roll;
    int pitch;
    int yaw;
    int throttle;

    std::vector<int> rc_max_limits;
    std::vector<int> rc_min_limits;

    std::string mode;
    std::string modeChange;
    bool armed;

    bool override;
};

#endif // SHARED_MEMORY_H
