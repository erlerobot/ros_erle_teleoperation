#include "threadgui.h"

threadGUI::threadGUI(Shared_Memory *share_memory, Thread_ROS *t_ros)
{
    gui = new GUI(share_memory, t_ros);
    gui->show();
}

threadGUI::~threadGUI()
{
    std::cout << "~threadGUI" << std::endl;
}

void threadGUI::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        gui->updateThreadGUI();

        if(gui->getEnd_thread()){
            break;
        }

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_gui)
            diff = cycle_gui;
        else
            diff = cycle_gui - diff;

        /*Sleep Algorithm*/
        usleep(diff * 1000);
    }
}
