#include "dialog.h"
#include <QApplication>
#include "qlog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(QLog::messageHandler);
    Dialog w;
    w.show();

    return a.exec();
}
