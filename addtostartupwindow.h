#ifndef ADDTOSTARTUPWINDOW_H
#define ADDTOSTARTUPWINDOW_H

#include <QDialog>
#include <QListWidget>
#include "Windows.h"

namespace Ui {
class AddToStartupWindow;
}

class AddToStartupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddToStartupWindow(QWidget *parent = nullptr, QListWidget* list = nullptr, HKEY path = NULL);
    ~AddToStartupWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddToStartupWindow *ui;
    QListWidget* list;
    HKEY path;
};

#endif // ADDTOSTARTUPWINDOW_H
