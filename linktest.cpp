#include "linktest.h"
#include "ui_linktest.h"

LinkTest::LinkTest(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LinkTest)
{
    ui->setupUi(this);
}

LinkTest::~LinkTest()
{
    delete ui;
}

void LinkTest::on_btnChangeIp_clicked()
{
    if(socket)
    {
        socket->deleteLater();
    }
    port=ui->lineEdit->text().toInt();
    socket=new QUdpSocket;
    socket->bind(port);
    connect(socket,SIGNAL(readyRead()),this,SLOT(onSockedReceibedData()));
    ui->labelIp->setText(QString::number(port));


    setStyleSheet(".LinkTest\n{\nbackground-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.602273 rgba(0, 255, 0, 255), stop:1 rgba(255, 255, 255, 0));\n}");
    count = 0;


    if(timer)
    {
        timer->deleteLater();
    }
    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    timer->start(100);
}
void LinkTest::onSockedReceibedData()
{
    unsigned char ch[10240];

    while(socket->bytesAvailable())
    {
        int realSize=socket->readDatagram((char*)ch,10240);
        //qDebug()<<realSize;
        if(realSize==52)
        {
            if(ch[0]==178&&ch[51]==229)
            {
                ui->labelPow->setText(QString::number((ch[2]*256+ch[3])/10.0));
                if(count > 50)
                {
                    qDebug()<<port<<"数据恢复了";
                }
                count = 0;

            }
            else
            {
                qDebug()<<"It's head error packge! size="<<realSize;
                qDebug()<<QByteArray((char*)ch,realSize).toHex();
            }
        }

    }

}

void LinkTest::onTimeOut()
{
    ++count;

    if(count == 50)
    {
        ui->labelPow->setText("8888");
        setStyleSheet(".LinkTest\n{\nbackground-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.602273 rgba(255, 0, 0, 255), stop:1 rgba(255, 255, 255, 0));\n}");
        qDebug()<<port<<"数据丢失";
    }
}




void LinkTest::on_btnReset_clicked()
{
    setStyleSheet(".LinkTest\n{\nbackground-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.602273 rgba(0, 255, 0, 255), stop:1 rgba(255, 255, 255, 0));\n}");

}
