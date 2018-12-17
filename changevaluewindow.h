#ifndef CHANGEVALUEWINDOW_H
#define CHANGEVALUEWINDOW_H

#include <QDialog>
#include "Windows.h"


namespace Ui {
class ChangeValueWindow;
}

class ChangeValueWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeValueWindow(QWidget *parent = nullptr, QString key = "", QString value = "", HKEY path = NULL);
//    explicit ChangeValueWindow(QWidget *parent = nullptr);
    ~ChangeValueWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ChangeValueWindow *ui;
    QString key;
    QString value;
    HKEY path;
};

#endif // CHANGEVALUEWINDOW_H
