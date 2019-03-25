#ifndef TIMELINE_H
#define TIMELINE_H

#include <QFrame>

namespace Ui {
class TimeLine;
}

class TimeLine : public QFrame
{
    Q_OBJECT

public:
    explicit TimeLine(QWidget *parent = 0);
    ~TimeLine();

private:
    Ui::TimeLine *ui;
};

#endif // TIMELINE_H
