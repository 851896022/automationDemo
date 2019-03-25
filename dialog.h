#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDateTime>
#include "timeline.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include <QDebug>
#include <QGroupBox>
#include <QFrame>
namespace Ui {
class Dialog;
}
enum State
{
    Error,
    HighPower,
    MidPower,
    LowPower,
    Close

};

class TsData
{
public:
    TsData() {}
    float analogCh[16];
    bool digitalCh[8];
    State state=Error;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QUdpSocket *socket=NULL;
    TsData tsData;
    QTimer WaitForSetIpRentrn;
    QTimer WaitForSetTimeRentrn;
    QTimer WaitForSetYxtRentrn;
    QTimer refTimeCount;
    QStringList NameListA;
    QStringList NameListD;
    QStringList NameListState;

    QList<QLabel*> aName;
    QList<QLabel*> DName;
    QList<QLabel*> aData;
    QList<QLabel*> DData;
    QList<QHBoxLayout*> hBoxListA;
    QList<QHBoxLayout*> hBoxListD;
    QVBoxLayout *vBoxA;
    QVBoxLayout *vBoxD;

    QVBoxLayout *vBoxYxt[8];
    TimeLine *timeLine[7][5];
    QGroupBox *gBox[7];
private slots:
    void initUi();
    void on_btnLink_clicked();

    void on_btnStop_clicked();

    void on_btnSendIp_clicked();

    void onSockedReceibedData();

    void on_btnSendTime_clicked();

    void onRentrnErrorTimer();
    void onRentrnErrorYxt();
    void onRentrnErrorIp();

    void refTime();
    void SendData(QByteArray ba);
    void on_btnSendYxt_clicked();
    void sendYxt(int week);
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
