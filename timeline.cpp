#include "timeline.h"
#include "ui_timeline.h"

TimeLine::TimeLine(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeLine)
{
    ui->setupUi(this);
}

TimeLine::~TimeLine()
{
    delete ui;
}
