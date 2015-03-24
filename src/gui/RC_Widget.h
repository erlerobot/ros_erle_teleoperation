#ifndef RC_Widget_H
#define RC_Widget_H

//Qt
#include <QtGui>

//Standard
#include <sys/time.h>
#include <iostream>
#include <pthread.h>

#include <boost/thread.hpp>

class RC_Widget : public QWidget
{    
    Q_OBJECT
public:
    RC_Widget(bool block_Y, int max_up_down,
              int min_up_down, int max_left_right,
              int min_left_right, int rev_up_down,
              int rev_left_right, QWidget *parent=0);
    ~RC_Widget();

    int getXaxis();
    int getYaxis();

    void setYAxis(int a);
    void setXAxis(int a);

    void setAutomatic(bool b);

private:
    QPoint p_actual;
    pthread_mutex_t mutex;
    bool press;
    bool automatic;
    bool block_Y;
    void run();

    int ejeX;
    int ejeY;

    int min_value_up_down;
    int max_value_up_down;
    int min_value_left_right;
    int max_value_left_right;
    int rev_up_down;
    int rev_left_right;

signals:
    void update_GUI();

public slots:
    void on_update_GUI_recieved();
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // RC_Widget_H
