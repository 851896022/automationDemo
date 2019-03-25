#include "timeline.h"
#include "ui_timeline.h"

TimeLine::TimeLine(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeLine)
{
    ui->setupUi(this);
}

TimeLine::~TimeLine()
{
    delete ui;
}

void TimeLine::on_timeEditStart_timeChanged(const QTime &time)
{
    startTime=time;
}

void TimeLine::on_timeEditEnd_timeChanged(const QTime &time)
{
    endTime=time;
}

void TimeLine::on_power_currentIndexChanged(int index)
{
    powerLevel=index+1;
}

void TimeLine::on_checkBosIsUseing_stateChanged(int arg1)
{
    isUsing=arg1;
    qDebug()<<arg1<<isUsing;
}
