#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDateTime>
#include "timeline.h"
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
    float analogCh[16]=0;
    bool digitalCh[8]=true;
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
private slots:
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
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
