#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addtostartupwindow.h"
#include "changevaluewindow.h"
#include "Windows.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

public:
    Ui::MainWindow *ui;
    AddToStartupWindow* addWindow;
    ChangeValueWindow* changeValueWindow;
    HKEY path;
};

#endif // MAINWINDOW_H
