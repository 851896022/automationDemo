#include "linktestparentwindow.h"
#include "ui_linktestparentwindow.h"

LinkTestParentWindow::LinkTestParentWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LinkTestParentWindow)
{
    ui->setupUi(this);
}

LinkTestParentWindow::~LinkTestParentWindow()
{
    delete ui;
}
