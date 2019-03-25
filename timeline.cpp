#include "timeline.h"
#include "ui_timeline.h"
#include <QDebug>
#include <QSettings>
TimeLine::TimeLine(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeLine)
{
    ui->setupUi(this);

}
void TimeLine::initThis()
{
    QSettings settings(QApplication::applicationDirPath()+QString("/test")+QString::number(day)+QString(".ini"), QSettings::IniFormat);
    {

        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"startTime";
        QString msg=settings.value(str).toString();
        qDebug()<<day<<line<<msg;
        ui->timeEditStart->setTime(QTime::fromString(msg,"hh:mm:ss"));
    }
    {

        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"endTime";
        QString msg=settings.value(str).toString();
        qDebug()<<day<<line<<msg;
        ui->timeEditEnd->setTime(QTime::fromString(msg,"hh:mm:ss"));
    }
    {

        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"powerLevel";
        int msg=settings.value(str).toInt();
        qDebug()<<day<<line<<msg;
        ui->power->setCurrentIndex(msg-1);
    }
    {
        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"isUsing";
        int msg=settings.value(str).toInt();
        qDebug()<<day<<line<<msg;
        ui->checkBosIsUseing->setChecked(msg);
    }

    startTime=ui->timeEditStart->time();
    endTime=ui->timeEditEnd->time();
    powerLevel=ui->power->currentIndex()+1;
    if(ui->checkBosIsUseing->isChecked())
    {
        isUsing=1;
    }
    else
    {
        isUsing=0;
    }
}

TimeLine::~TimeLine()
{
    delete ui;


}

void TimeLine::on_timeEditStart_timeChanged(const QTime &time)
{
    startTime=time;
    QSettings settings(QApplication::applicationDirPath()+QString("/test")+QString::number(day)+QString(".ini"), QSettings::IniFormat);
    {

        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"startTime";
        settings.setValue(str,time.toString("hh:mm:ss"));
    }
}

void TimeLine::on_timeEditEnd_timeChanged(const QTime &time)
{
    endTime=time;
    QSettings settings(QApplication::applicationDirPath()+QString("/test")+QString::number(day)+QString(".ini"), QSettings::IniFormat);
    {

        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"endTime";
        settings.setValue(str,time.toString("hh:mm:ss"));
    }
}

void TimeLine::on_power_currentIndexChanged(int index)
{
    powerLevel=index+1;
    {
        QSettings settings(QApplication::applicationDirPath()+QString("/test")+QString::number(day)+QString(".ini"), QSettings::IniFormat);
        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"powerLevel";
        settings.setValue(str,powerLevel);
    }
}

void TimeLine::on_checkBosIsUseing_stateChanged(int arg1)
{
    if(arg1)
    {
        isUsing=1;
    }
    else
    {
        isUsing=0;
    }
    {
        QSettings settings(QApplication::applicationDirPath()+QString("/test")+QString::number(day)+QString(".ini"), QSettings::IniFormat);
        QString str="TimeSet/d"+QString::number(day)+"l"+QString::number(line)+"isUsing";
        settings.setValue(str,isUsing);
    }
}
