#ifndef LINKTEST_H
#define LINKTEST_H

#include <QFrame>
#include <QDialog>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QDebug>
#include <QGroupBox>
#include <QFrame>
namespace Ui {
class LinkTest;
}

class LinkTest : public QFrame
{
    Q_OBJECT

public:
    explicit LinkTest(QWidget *parent = 0);
    ~LinkTest();
    QUdpSocket *socket=NULL;
    QTimer *timer=NULL;
    int count = 0;
    int port = 0;
private slots:
    void on_btnChangeIp_clicked();
    void onSockedReceibedData();
    void onTimeOut();
    void on_btnReset_clicked();

public slots:

private:
    Ui::LinkTest *ui;
};

#endif // LINKTEST_H
