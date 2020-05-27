#include "dialog.h"
#include <QApplication>
#include "qlog.h"
#include "linktestparentwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(QLog::messageHandler);
    //Dialog w;
    //w.show();
    LinkTestParentWindow l;
    l.show();
    return a.exec();
}
