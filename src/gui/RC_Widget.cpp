#include "RC_Widget.h"

RC_Widget::RC_Widget(bool block_Y,
                     int max_up_down, int min_up_down,
                     int max_left_right, int min_left_right,
                     int rev_up_down, int rev_left_right,
                     QWidget *parent) :
    QWidget(parent)
{
    if (pthread_mutex_init(&mutex, NULL) != 0){
        std::cout << "mutex init failed" << std::endl;
    }

    QPalette pal_background;
    pal_background.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal_background);
    setMaximumSize(500, 500);
    setMinimumSize(500, 500);

    press = false;
    automatic = false;
    this->block_Y = block_Y;

    boost::thread workerThread(&RC_Widget::run, this);

    connect(this, SIGNAL(update_GUI()), this, SLOT(on_update_GUI_recieved()));

    p_actual.setX(250);
    if(block_Y)
        p_actual.setY(495);
    else
        p_actual.setY(250);

    this->min_value_up_down = min_up_down;
    this->max_value_up_down = max_up_down;

    this->min_value_left_right = min_left_right;
    this->max_value_left_right = max_left_right;

    this->rev_left_right = rev_left_right;
    this->rev_up_down = rev_up_down;
}

RC_Widget::~RC_Widget()
{
}

void RC_Widget::setAutomatic(bool b)
{
    pthread_mutex_lock( &mutex );
    automatic = b;
    pthread_mutex_unlock( &mutex );
}

void RC_Widget::mousePressEvent(QMouseEvent *e)
{
    pthread_mutex_lock( &mutex );
    if(!automatic){
        p_actual = e->pos();
        press = true;
    }
    update();
    pthread_mutex_unlock( &mutex );
}

void RC_Widget::mouseReleaseEvent(QMouseEvent *e)
{
    pthread_mutex_lock( &mutex );
    if(!automatic){
        p_actual = e->pos();
        press = false;
    }
    update();
    pthread_mutex_unlock( &mutex );
}

void RC_Widget::mouseMoveEvent(QMouseEvent *e)
{
    pthread_mutex_lock( &mutex );
    if(!automatic){
        p_actual = e->pos();
    }
    update();
    pthread_mutex_unlock( &mutex );
//    QPoint p = this->mapFromParent(QPoint(event->x(), event->y() ));
}

int RC_Widget::getXaxis()
{
    int result;
    pthread_mutex_lock( &mutex );
    result = ejeX;
    pthread_mutex_unlock( &mutex );
    return result;
}

int RC_Widget::getYaxis()
{
    int result;
    pthread_mutex_lock( &mutex );
    result = ejeY;
    pthread_mutex_unlock( &mutex );
    return result;
}

void RC_Widget::setYAxis(int a)
{
    pthread_mutex_lock( &mutex );
    p_actual.setY(a);
    pthread_mutex_unlock( &mutex );
}

void RC_Widget::setXAxis(int a)
{
    pthread_mutex_lock( &mutex );
    p_actual.setX(a);
    pthread_mutex_unlock( &mutex );
}


void RC_Widget::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    int cycle_processing = 50;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //
        pthread_mutex_lock( &mutex );
        if(!press && !automatic){
            if(p_actual.x()>250){
                p_actual.setX(p_actual.x() - 10);
                if(p_actual.x()<250)
                    p_actual.setX(250);
            }
            else if(p_actual.x()<250){
                p_actual.setX(p_actual.x() + 10);
                if(p_actual.x()>250)
                    p_actual.setX(250);
            }
            if(!block_Y){
                if(p_actual.y()>250){
                    p_actual.setY(p_actual.y() - 10) ;
                    if(p_actual.y()<250)
                        p_actual.setY(250);
                }
                else if(p_actual.y()<250){
                    p_actual.setY(p_actual.y() + 10) ;
                    if(p_actual.y()>250)
                        p_actual.setY(250);
                }
            }

        }
        pthread_mutex_unlock( &mutex );
        //

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_processing)
            diff = cycle_processing;
        else
            diff = cycle_processing - diff;

        /*Sleep Algorithm*/
        usleep(diff * 1000);
        emit update_GUI();
    }
}

void RC_Widget::on_update_GUI_recieved()
{
    update();
}

void RC_Widget::paintEvent(QPaintEvent*)
{
    QPen pen_red(Qt::red);
    pen_red.setWidth(5);
    QPainter painter(this);
    painter.setPen(pen_red);

    painter.drawLine(0, 250, 500, 250);
    painter.drawLine(250, 0, 250, 500);

    QPen pen_blue(Qt::blue);
    pen_blue.setWidth(3);
    painter.setPen(pen_blue);

    if(p_actual.x()<0)
        p_actual.setX(0);
    if(p_actual.y()<0)
        p_actual.setY(0);

    if(p_actual.x()>500)
        p_actual.setX(500);
    if(p_actual.y()>500)
        p_actual.setY(500);

    painter.drawLine(0, p_actual.y(), 500, p_actual.y());
    painter.drawLine(p_actual.x(), 0, p_actual.x(), 500);

    int y_value = p_actual.y();
    int x_value = p_actual.x();

    if(rev_up_down<0){
        if(y_value>250)
            y_value = 250 - (y_value - 250);
        else
            y_value = 250 + (250 - y_value);
    }

    if(rev_left_right<0){
        if(x_value>250)
            x_value = 250 - (x_value - 250);
        else
            x_value = 250 + (250 - x_value);
    }

    double up_down =    (250 - y_value)*500/250 + ((min_value_up_down    + max_value_up_down)/2);
    double left_right = (250 - x_value)*500/250 + ((min_value_left_right + max_value_left_right)/2);

    ejeX = up_down;
    ejeY = left_right;

//    std::cout << "Repaint: "<< arriba_abajo << " " << izquierda_derecha << " " << p_actual.x() << " " << p_actual.y() << std::endl;
}
