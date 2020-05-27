#include "dialog.h"
#include "ui_dialog.h"
#include "QMessageBox"
#include <QThread>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    initUi();
    ui->btnStop->setEnabled(false);
    ui->groupBoxSetIp->setEnabled(false);
    ui->groupBoxSetTime->setEnabled(false);
    ui->btnSendYxt->setEnabled(false);
    WaitForSetTimeRentrn.setSingleShot(true);
    connect(&WaitForSetTimeRentrn,SIGNAL(timeout()),this,SLOT(onRentrnErrorTimer()));
    WaitForSetYxtRentrn.setSingleShot(true);
    connect(&WaitForSetYxtRentrn,SIGNAL(timeout()),this,SLOT(onRentrnErrorYxt()));
    connect(&refTimeCount,SIGNAL(timeout()),this,SLOT(refTime()));
    refTimeCount.start(200);
}
void Dialog::initUi()
{
    //======show===========
    NameListA<<"发射功率（KW）"
            <<"反射功率（KW）"
           <<"电源电流（A）"
          <<"电源电压（V）"
         <<"射频驱动（V）"
        <<"网络驻波（V）"
       <<"天线驻波（V）"
      <<"+22V（V）"
     <<"+8V（V）"
    <<"预留"
    <<"预留"
    <<"预留"
    <<"-22V"
    <<"-8V"
    <<"预留"
    <<"预留";
    NameListD<<"激励"
            <<"缓冲"
           <<"中放"
          <<"功放"
         <<"网络驻波比"
        <<"天线驻波比"
       <<"AD转换错误"
      <<"射频欠激";
    NameListState<<"无效状态"
                <<"高功率"
               <<"中功率"
              <<"低功率"
             <<"关机";
    vBoxA=new QVBoxLayout;
    vBoxD=new QVBoxLayout;
    for(int i=0;i<16;i++)
    {
        QLabel * name=new QLabel;
        QLabel * data=new QLabel;
        QHBoxLayout * hbox=new QHBoxLayout;
        name->setText(NameListA.at(i));
        aName.append(name);
        aData.append(data);
        hBoxListA.append(hbox);
        hbox->addWidget(name);
        hbox->addWidget(data);
        vBoxA->addLayout(hbox);
    }
    for(int i=0;i<8;i++)
    {
        QLabel *name = new QLabel;
        QLabel *data= new QLabel;
        QHBoxLayout * hbox=new QHBoxLayout;
        name->setText(NameListD.at(i));
        DName.append(name);
        DData.append(data);
        hBoxListD.append(hbox);
        hbox->addWidget(name);
        hbox->addWidget(data);
        vBoxD->addLayout(hbox);
    }
    ui->groupBoxA->setLayout(vBoxA);
    ui->groupBoxD->setLayout(vBoxD);

    //=======YXT===========




    vBoxYxt[7]=new QVBoxLayout;
    for(int i=0;i<7;i++)
    {
        gBox[i]=new QGroupBox;
        vBoxYxt[i]=new QVBoxLayout;
        for(int j=0;j<5;j++)
        {
            timeLine[i][j]=new TimeLine();
            timeLine[i][j]->day=i;
            timeLine[i][j]->line=j;
            timeLine[i][j]->initThis();
            vBoxYxt[i]->addWidget(timeLine[i][j]);

        }
        gBox[i]->setLayout(vBoxYxt[i]);
        vBoxYxt[7]->addWidget(gBox[i]);
    }


    gBox[0]->setTitle("星期一");
    gBox[1]->setTitle("星期二");
    gBox[2]->setTitle("星期三");
    gBox[3]->setTitle("星期四");
    gBox[4]->setTitle("星期五");
    gBox[5]->setTitle("星期六");
    gBox[6]->setTitle("星期日");

    ui->scrollAreaWidgetContents->setLayout(vBoxYxt[7]);

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
        ui->btnSendYxt->setEnabled(true);
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
    ui->btnSendYxt->setEnabled(false);
    WaitForSetIpRentrn.stop();
    WaitForSetTimeRentrn.stop();
    WaitForSetYxtRentrn.stop();

}

void Dialog::on_btnSendIp_clicked()
{
    /*
    第0字节: 帧头(固定为0xB2)
    第1 - 4字节：网关
    第5 - 8字节：子网掩码
    第9 - 12字节：设备的IP
    第13 - 16字节：上位计算机的IP
    第17 - 18字节：设备端口号
    第19 - 20字节：目的端口号
    第21字节: 帧尾(固定为0xE5)
    */

    char data[23];
    data[0]=(char)0xB2;
    data[1]=(char)0x17;
    {
        QStringList tmp=ui->txtGate->text().split(".");
        data[2]=(char)tmp.at(0).toInt();
        data[3]=(char)tmp.at(1).toInt();
        data[4]=(char)tmp.at(2).toInt();
        data[5]=(char)tmp.at(3).toInt();
    }
    {
        QStringList tmp=ui->txtMask->text().split(".");
        data[6]=(char)tmp.at(0).toInt();
        data[7]=(char)tmp.at(1).toInt();
        data[8]=(char)tmp.at(2).toInt();
        data[9]=(char)tmp.at(3).toInt();
    }
    {
        QStringList tmp=ui->txtEquIp->text().split(".");
        data[10]=(char)tmp.at(0).toInt();
        data[11]=(char)tmp.at(1).toInt();
        data[12]=(char)tmp.at(2).toInt();
        data[13]=(char)tmp.at(3).toInt();
    }
    {
        QStringList tmp=ui->txtServerIp->text().split(".");
        data[14]=(char)tmp.at(0).toInt();
        data[15]=(char)tmp.at(1).toInt();
        data[16]=(char)tmp.at(2).toInt();
        data[17]=(char)tmp.at(3).toInt();
    }
    {
        data[18]=(char)(ui->txtEquPort->text().toInt()/256);
        data[19]=(char)(ui->txtEquPort->text().toInt()%256);
    }
    {
        data[20]=(char)(ui->txtServerPort->text().toInt()/256);
        data[21]=(char)(ui->txtServerPort->text().toInt()%256);
    }
    data[22]=(char)0xE5;
    SendData(QByteArray(data,23));

    QMessageBox::information(this,"成功","发送成功");
}

void Dialog::onRentrnErrorIp()
{

}
void Dialog::on_btnSendTime_clicked()
{
    /*
    第1字节：帧头（0xB2）
    第2字节：本包字节总数（0x0A）
    第3字节：“年”的后两位（例如：2019年，则本字节为十进制数“19”）
    第4字节：“月”
    第5字节：“日”
    第6字节：“时”
    第7字节：“分”
    第8字节：“秒”
    第9字节：“星期”
    第10字节：帧尾（0xE5）
    */
    char ch[10];
    ch[0]=(char)0xB2;
    ch[1]=(char)0x0A;
    {
        int tmp=ui->txtYear->text().toInt();
        if(tmp>=0&&tmp<99)
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
        if(tmp>=0&&tmp<25)
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
        if(tmp>=0&&tmp<60)
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
        if(tmp>=0&&tmp<60)
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
void Dialog::on_btnSendYxt_clicked()
{
    ui->btnSendYxt->setEnabled(false);
    sendYxt(0);

}
void Dialog::sendYxt(int week)
{
    char ch[44];
    ch[0]=(char)0xB2;
    ch[1]=(char)0x2C;
    for(int i=0;i<5;i++)
    {
        ch[i*6+2]=timeLine[week][i]->startTime.hour();
        ch[i*6+3]=timeLine[week][i]->startTime.minute();
        ch[i*6+4]=timeLine[week][i]->startTime.second();

        ch[i*6+5]=timeLine[week][i]->endTime.hour();
        ch[i*6+6]=timeLine[week][i]->endTime.minute();
        ch[i*6+7]=timeLine[week][i]->endTime.second();

        ch[32+i]=timeLine[week][i]->powerLevel;
        ch[37+i]=timeLine[week][i]->isUsing;
    }
    ch[42]=(char)week+1;
    ch[43]=(char)0xE5;
    SendData(QByteArray(ch,44));
    WaitForSetYxtRentrn.start(2000);

}
void Dialog::onRentrnErrorYxt()
{
    QMessageBox::information(this,"错误","运行图下发失败！");
    ui->btnSendYxt->setEnabled(true);
}
void Dialog::onSockedReceibedData()
{
    unsigned char ch[10240];

    while(socket->bytesAvailable())
    {
        int realSize=socket->readDatagram((char*)ch,10240);
        //qDebug()<<realSize;
        if(realSize==5)
        {
            if(ch[0]==178&&ch[4]==229)
            {
                if(ch[2]==0x0A)
                {
                    //time
                    WaitForSetTimeRentrn.stop();
                    QMessageBox::about(NULL,"成功","修改成功！");
                    ui->groupBoxSetTime->setEnabled(true);
                    qDebug()<<"It's good packge! size="<<realSize;
                    qDebug()<<QByteArray((char*)ch,realSize).toHex();

                }
                else if(ch[2]==0x2C)
                {
                    //yxt

                    if(ch[3]<7)
                    {
                        WaitForSetYxtRentrn.stop();
                        sendYxt(ch[3]);
                        WaitForSetYxtRentrn.start(2000);
                        qDebug()<<"It's good packge! size="<<realSize;
                        qDebug()<<QByteArray((char*)ch,realSize).toHex();
                    }
                    else if(ch[3]==7)
                    {
                        WaitForSetYxtRentrn.stop();
                        QMessageBox::about(this,"成功","运行图下发成功！");
                        ui->btnSendYxt->setEnabled(true);
                        qDebug()<<"It's good packge! size="<<realSize;
                        qDebug()<<QByteArray((char*)ch,realSize).toHex();
                    }
                    else
                    {
                        qDebug()<<"It's data error packge! size="<<realSize;
                        qDebug()<<QByteArray((char*)ch,realSize).toHex();
                    }
                }
                else
                {
                    qDebug()<<"It's data error packge! size="<<realSize;
                    qDebug()<<QByteArray((char*)ch,realSize).toHex();
                }
            }
            else
            {
                qDebug()<<"It's head error packge! size="<<realSize;
                qDebug()<<QByteArray((char*)ch,realSize).toHex();
            }
        }
        else if(realSize==52)
        {
            if(ch[0]==178&&ch[27]==229)
            {
                QString msg;
                for(int i=0;i<12;i++)
                {
                    if(i==2||i==3)
                    {
                        tsData.analogCh[i]=ch[i+2];
                        msg+=QString::number(tsData.analogCh[i])+"|";
                        aData[i]->setText(QString::number(tsData.analogCh[i]));
                    }
                    else
                    {
                        tsData.analogCh[i]=ch[i+2]/10.0;
                        msg+=QString::number(tsData.analogCh[i])+"|";
                        aData[i]->setText(QString::number(tsData.analogCh[i]));
                    }


                }
                ui->powerBar->setValue(tsData.analogCh[0]*10);
                for(int i=12;i<16;i++)
                {
                    tsData.analogCh[i]=0-(ch[i+2]/10.0);
                    msg+=QString::number(tsData.analogCh[i])+"|";
                    aData[i]->setText(QString::number(tsData.analogCh[i]));
                }
                for(int i=0;i<8;i++)
                {
                    if(ch[i+18]==1)
                    {
                        tsData.digitalCh[i]=false;
                    }
                    else if(ch[i+18]==0)
                    {
                        tsData.digitalCh[i]=true;
                    }
                    else
                    {
                        qDebug()<<"It's data error packge! size="<<realSize;
                        qDebug()<<QByteArray((char*)ch,realSize).toHex();
                        TsData tmp;
                        tsData=tmp;
                        break;
                    }
                    if(tsData.digitalCh[i])
                    {
                        msg+="TRUE|";
                        DData[i]->setText("正常");
                    }
                    else
                    {
                        msg+="FALSE|";
                        DData[i]->setText("异常");
                    }

                }
                tsData.state=State(ch[26]);
                ui->labelTsState->setText(NameListState.at(tsData.state));
                msg+=QString::number(tsData.state)+"|";
                qDebug()<<msg;

            }
            else
            {
                qDebug()<<"It's head error packge! size="<<realSize;
                qDebug()<<QByteArray((char*)ch,realSize).toHex();
            }
        }
        else
        {
            qDebug()<<"It's unkuow packge! size="<<realSize;
            qDebug()<<QByteArray((char*)ch,realSize).toHex();
        }
    }
    /*
    else
    {
        int realSize=socket->readDatagram((char*)ch,10240);
        qDebug()<<"It's ???? packge! size="<<realSize;
        qDebug()<<QByteArray((char*)ch,realSize).toHex();
    }
    */
}
void Dialog::SendData(QByteArray ba)
{
    if(socket)
    {
        socket->writeDatagram(ba,QHostAddress( ui->txtNowIp->text()),ui->txtNowPort->text().toInt());
    }
}



