#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addtostartupwindow.h"
#include "changevaluewindow.h"
#include "about.h"
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

    void on_actionNew_program_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

public:
    Ui::MainWindow *ui;
    AddToStartupWindow* addWindow;
    ChangeValueWindow* changeValueWindow;
    About* aboutWindow;
    HKEY path;
};

#endif // MAINWINDOW_H
