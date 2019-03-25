#include "dialog.h"
#include "ui_dialog.h"
#include "QMessageBox"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    ui->btnStop->setEnabled(false);
    ui->groupBoxSetIp->setEnabled(false);
    ui->groupBoxSetTime->setEnabled(false);
    WaitForSetTimeRentrn.setSingleShot(true);
    connect(&WaitForSetTimeRentrn,SIGNAL(timeout()),this,SLOT(onRentrnErrorTimer()));
    WaitForSetYxtRentrn.setSingleShot(true);
    connect(&WaitForSetYxtRentrn,SIGNAL(timeout()),this,SLOT(onRentrnErrorYxt()));



    connect(&refTimeCount,SIGNAL(timeout()),this,SLOT(refTime()));
    refTimeCount.start(200);
}

Dialog::~Dialog()
{
    delete ui;

}

void Dialog::on_btnLink_clicked()
{

    int port=ui->txtNowPort->text().toInt();
    if(port<65535&&port>0)
    {
        ui->btnLink->setEnabled(false);
        ui->txtNowIp->setEnabled(false);
        ui->txtNowPort->setEnabled(false);
        socket=new QUdpSocket;
        socket->bind(port);
        connect(socket,SIGNAL(readyRead()),this,SLOT(onSockedReceibedData()));
        ui->btnStop->setEnabled(true);
        ui->groupBoxSetIp->setEnabled(true);
        ui->groupBoxSetTime->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"错误","参数设置有误！");
    }
}

void Dialog::on_btnStop_clicked()
{
    ui->btnStop->setEnabled(false);
    ui->groupBoxSetIp->setEnabled(false);
    ui->groupBoxSetTime->setEnabled(false);

    disconnect(socket,SIGNAL(readyRead()),this,SLOT(onSockedReceibedData()));
    socket->deleteLater();
    socket=NULL;
    ui->btnLink->setEnabled(true);
    ui->txtNowIp->setEnabled(true);
    ui->txtNowPort->setEnabled(true);
    WaitForSetIpRentrn.stop();
    WaitForSetTimeRentrn.stop();
    WaitForSetYxtRentrn.stop();

}

void Dialog::on_btnSendIp_clicked()
{
    QMessageBox::information(this,"成功","发送成功");
}

void Dialog::onRentrnErrorIp()
{

}
void Dialog::on_btnSendTime_clicked()
{


    char ch[10];
    ch[0]=(char)0xB2;
    ch[1]=(char)0x0A;
    {
        int tmp=ui->txtYear->text().toInt();
        if(tmp>0&&tmp<99)
        {
           ch[2]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误","年份有误！");
            return;
        }

    }
    {
        int tmp=ui->txtMon->text().toInt();
        if(tmp>0&&tmp<13)
        {
           ch[3]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误","月份有误！");
            return;
        }

    }
    {
        int tmp=ui->txtDay->text().toInt();
        if(tmp>0&&tmp<32)
        {
           ch[4]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误"," 日 有误！");
            return;
        }

    }

    {
        int tmp=ui->txtHour->text().toInt();
        if(tmp>0&&tmp<25)
        {
           ch[5]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误"," 时 有误！");
            return;
        }

    }
    {
        int tmp=ui->txtMIn->text().toInt();
        if(tmp>0&&tmp<60)
        {
           ch[6]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误"," 分 有误！");
            return;
        }

    }
    {
        int tmp=ui->txtSec->text().toInt();
        if(tmp>0&&tmp<60)
        {
           ch[7]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误"," 秒 有误！");
            return;
        }

    }
    {
        int tmp=ui->txtWeek->text().toInt();
        if(tmp>0&&tmp<8)
        {
           ch[8]=(char)tmp;
        }
        else
        {
            QMessageBox::information(this,"错误"," 星期 有误！");
            return;
        }

    }




    ch[9]=(char)0xE5;

    SendData(QByteArray(ch,10));
    ui->groupBoxSetTime->setEnabled(false);
    WaitForSetTimeRentrn.start(2000);

}
void Dialog::onRentrnErrorTimer()
{

    ui->groupBoxSetTime->setEnabled(true);
    QMessageBox::information(this,"错误","修改失败！");
}
void Dialog::refTime()
{
    if(ui->checkBoxAutoRefTime->isChecked())
    {
        QDateTime time=QDateTime::currentDateTime();
        ui->txtYear->setText(time.toString("yy"));
        ui->txtMon->setText(time.toString("MM"));
        ui->txtDay->setText(time.toString("dd"));
        ui->txtWeek->setText(QString::number(time.date().dayOfWeek()));
        ui->txtHour->setText(time.toString("hh"));
        ui->txtMIn->setText(time.toString("mm"));
        ui->txtSec->setText(time.toString("ss"));
    }
}

void Dialog::onRentrnErrorYxt()
{

}
void Dialog::onSockedReceibedData()
{

}
void Dialog::SendData(QByteArray ba)
{
    if(socket)
    {
        socket->writeDatagram(ba,QHostAddress( ui->txtNowIp->text()),ui->txtNowPort->text().toInt());
    }
}

