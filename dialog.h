#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDateTime>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QUdpSocket *socket=NULL;

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
