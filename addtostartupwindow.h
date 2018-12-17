#ifndef ADDTOSTARTUPWINDOW_H
#define ADDTOSTARTUPWINDOW_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class AddToStartupWindow;
}

class AddToStartupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddToStartupWindow(QWidget *parent = nullptr, QListWidget* list = nullptr);
    ~AddToStartupWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AddToStartupWindow *ui;
    QListWidget* list;
};

#endif // ADDTOSTARTUPWINDOW_H
