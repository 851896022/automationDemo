#ifndef LINKTESTPARENTWINDOW_H
#define LINKTESTPARENTWINDOW_H

#include <QFrame>

namespace Ui {
class LinkTestParentWindow;
}

class LinkTestParentWindow : public QFrame
{
    Q_OBJECT

public:
    explicit LinkTestParentWindow(QWidget *parent = 0);
    ~LinkTestParentWindow();

private:
    Ui::LinkTestParentWindow *ui;
};

#endif // LINKTESTPARENTWINDOW_H
