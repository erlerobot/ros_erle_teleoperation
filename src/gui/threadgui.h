#ifndef THREADGUI_H
#define THREADGUI_H

#include <QtGui>

#include <iostream>
#include <sys/time.h>

#include "gui.h"

#include "../shared_memory.h"

#include "../ROS/thread_ros.h"

#define cycle_gui 50 //miliseconds

class threadGUI:public QThread
{
public:
    threadGUI(Shared_Memory* share_memory, Thread_ROS* t_ros);
    ~threadGUI();
//private:
    GUI* gui;

protected:
    void run();
};

#endif // THREADGUI_H
