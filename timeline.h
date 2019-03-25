#ifndef TIMELINE_H
#define TIMELINE_H

#include <QFrame>
#include <QTime>
namespace Ui {
class TimeLine;
}

class TimeLine : public QFrame
{
    Q_OBJECT

public:
    explicit TimeLine(QWidget *parent = 0);
    ~TimeLine();
    int line;
    int day;
    QTime startTime;
    QTime endTime;
    int powerLevel=0;
    int  isUsing=0;
private slots:
    void on_timeEditStart_timeChanged(const QTime &time);

    void on_timeEditEnd_timeChanged(const QTime &time);

    void on_power_currentIndexChanged(int index);

    void on_checkBosIsUseing_stateChanged(int arg1);
public slots:
    void initThis();
private:
    Ui::TimeLine *ui;
};

#endif // TIMELINE_H
