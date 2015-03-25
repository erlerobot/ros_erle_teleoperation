#include "gui.h"

GUI::GUI(Shared_Memory *share_memory, Thread_ROS* t_ros)
{
    this->t_ros = t_ros;
    this->share_memory = share_memory;

    mainLayout = new QGridLayout();

    config_RC();

    config_mode();

    setLayout(mainLayout);

    setVisible(true);

    connect(this, SIGNAL(signal_updateGUI()), this, SLOT(on_updateGUI_recieved()));

    show();
//    setAttribute(Qt::WA_DeleteOnClose);
//    setAttribute(Qt::WA_QuitOnClose);
    end_thread = false;

    connect(this, SIGNAL(destroyed(QObject*)), this, SLOT(on_closed_event()));
}

void GUI::on_closed_event()
{
    std::cout << "on_closed_event" << std::endl;
    end_thread = true;
    delete t_ros;
}

GUI::~GUI()
{
    end_thread = true;
    delete t_ros;
    std::cout << "Destructor GUI" << std::endl;
}

bool GUI::getEnd_thread()
{
    return end_thread;
}

void GUI::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_O)
        check_override->setChecked(!check_override->isChecked());
    std::cout << "keyPressEvent" << std::endl;

}

void GUI::config_mode()
{
    QGroupBox* boxMode = new QGroupBox("Mode");
    QGridLayout* layMode = new QGridLayout();
    boxMode->setLayout(layMode);

    label_mode = new QLabel("");
    combo_mode = new QComboBox();
    combo_mode->addItem("STABILIZE");
    combo_mode->addItem("ALT_HOLD");
    combo_mode->addItem("LOITER");
    combo_mode->addItem("LAND");
    combo_mode->addItem("RTL");
    combo_mode->addItem("GUIDED");
    combo_mode->addItem("AUTO");

    layMode->addWidget(label_mode, 0, 0);
    layMode->addWidget(combo_mode, 0, 1);

    mainLayout->addWidget(boxMode, 0, 0);
    connect(combo_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_mode_changed()));
}

void GUI::on_combo_mode_changed()
{
    std::cout << "Mode: " << combo_mode->currentText().toAscii().data() << std::endl;
    share_memory->setModeChange(std::string(combo_mode->currentText().toAscii().data()));
}

void GUI::config_RC()
{
    QGroupBox* boxRC = new QGroupBox("Radio Control");
    QGridLayout* layRC = new QGridLayout();
    boxRC->setLayout(layRC);

    std::vector<int> rc_maxlimits = share_memory->getRC_maxlimits();
    std::vector<int> rc_minlimits = share_memory->getRC_minlimits();

    channel12 = new RC_Widget(false,
                              rc_maxlimits[0], rc_minlimits[0],
                              rc_maxlimits[1], rc_minlimits[1],
                              1, -1);
    channel34 = new RC_Widget(true,
                              rc_maxlimits[2], rc_minlimits[2],
                              rc_maxlimits[3], rc_minlimits[3],
                              1, -1);

    label_pitch = new QLabel();
    label_roll = new QLabel();
    label_yaw = new QLabel();
    label_throttle = new QLabel();

    check_override = new QCheckBox("Override RC");

    QGridLayout* laychannel12 = new QGridLayout();
    QGridLayout* laychannel34 = new QGridLayout();
    QGridLayout* layoverride = new QGridLayout();

    laychannel12->addWidget(label_throttle, 0, 0);
    laychannel12->addWidget(label_yaw, 0, 1);

    laychannel34->addWidget(label_pitch, 0, 0);
    laychannel34->addWidget(label_roll, 0, 1);

    layoverride->addWidget(check_override, 0, 0);

    layRC->addWidget(channel12, 0, 1);
    layRC->addLayout(laychannel12, 1, 0);
    layRC->addWidget(channel34, 0, 0);
    layRC->addLayout(laychannel34, 1, 1);
    layRC->addLayout(layoverride, 2, 0);

    connect(check_override, SIGNAL(stateChanged(int)), this, SLOT(on_check_override_changed()));

    mainLayout->addWidget(boxRC, 1, 0);
}

void GUI::on_check_override_changed()
{
    share_memory->setOverride(check_override->isChecked());
}

void GUI::updateThreadGUI()
{
    emit signal_updateGUI();
}

void GUI::on_updateGUI_recieved()
{
    label_throttle->setText(QString("Throttle: %1").arg(channel34->getXaxis()));
    label_yaw->setText(QString("Yaw: %1").arg(channel34->getYaxis()));

    label_roll->setText(QString("Roll: %1").arg(channel12->getYaxis()));
    label_pitch->setText(QString("Pitch: %1").arg(channel12->getXaxis()));

    share_memory->setPitch(channel12->getXaxis());
    share_memory->setRoll(channel12->getYaxis());

    share_memory->setThrottle(channel34->getXaxis());
    share_memory->setYaw(channel34->getYaxis());

    std::string state;

    if(share_memory->getArmed())
        state = share_memory->getMode() + std::string(" - Armed");
    else
        state = share_memory->getMode() + std::string(" - Disarmed");

    label_mode->setText(state.c_str());

}
