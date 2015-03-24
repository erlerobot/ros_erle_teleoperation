#ifndef GUI_H
#define GUI_H

//Qt
#include <QtGui>

//standard
#include <iostream>

//ROS
#include <ros/ros.h>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../shared_memory.h"

#include "RC_Widget.h"

class GUI:public QWidget
{
    Q_OBJECT

public:
    GUI(Shared_Memory* share_memory);
    void updateThreadGUI();


private:
    Shared_Memory* share_memory;
    void config_RC();
    void config_mode();

    QGridLayout* mainLayout;

    QLabel* label_mode;
    QComboBox* combo_mode;

    RC_Widget* channel12;
    RC_Widget* channel34;

    QLabel* label_pitch;
    QLabel* label_roll;
    QLabel* label_throttle;
    QLabel* label_yaw;

    ///
signals:
    void signal_updateGUI();

public slots:
    void on_updateGUI_recieved();
    void on_combo_mode_changed();
};

#endif // GUI_H
